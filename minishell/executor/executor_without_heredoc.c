/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <eecegokcece@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/08/08 XX:XX:XX by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

/* Child process'te komut çalıştırır (fork sonrası)
 * Pipe, redirection ve builtin/external komut işleme
 * Parametreler: shell, cmd, pipefd, prev_fd
 * Dönüş: exit() ile çıkar */
static int	execute_child_process(t_shell *shell, t_cmd *cmd, int *pipefd, int prev_fd)
{
	char	*executable;
	char	**argv;
	char	**envp;

	// default_signals(); // Sinyal fonksiyonu henüz yok
	
	// Önceki pipe'dan gelen input
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	
	// Sonraki pipe için output
	if (cmd->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	
	// Redirection'ları ayarla (heredoc dahil)
	if (setup_redirections(cmd) != 0)
		exit(1);
	
	// Token'ları char** argv'ye çevir
	argv = ft_expand_tokens(cmd->args, shell);
	if (!argv || !argv[0])
		exit(1);
	
	// Builtin komut kontrolü
	if (ft_is_builtin(argv[0]))
		exit(ft_execute_builtin(shell, cmd, 1)); // pipe modunda
	
	// External komut çalıştır
	executable = find_executable(argv[0], shell->env_list);
	if (!executable)
	{
		printf("minishell: %s: command not found\n", argv[0]);
		exit(127);
	}
	
	envp = env_to_array(shell->env_list);
	execve(executable, argv, envp);
	
	printf("minishell: %s: execution failed\n", argv[0]);
	exit(126);
}

/* Pipeline komutlarını çalıştırır (pipe ile bağlı komutlar)
 * Her komut için fork yapır ve pipe ile bağlar
 * Parametreler: shell - shell yapısı
 * Dönüş: son komutun exit kodu */
int	execute_pipeline(t_shell *shell)
{
	t_cmd	*current;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	int		last_status;

	current = shell->cmd_list;
	prev_fd = -1;
	last_status = 0;
	
	while (current)
	{
		// Sonraki komut varsa pipe oluştur
		if (current->next && pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}
		
		// Her komut için fork
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			return (1);
		}
		
		// Child process
		if (pid == 0)
			execute_child_process(shell, current, pipefd, prev_fd);
		
		// Parent process - pipe'ları temizle
		if (prev_fd != -1)
			close(prev_fd);
		
		if (current->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		
		current = current->next;
	}
	
	// ignore_signals(); // Sinyal fonksiyonu henüz yok
	
	// Tüm child process'leri bekle
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}
	
	return (last_status);
}

/* Tek komut çalıştırır (pipe yok)
 * Builtin ise parent'ta, external ise fork ile çalıştırır
 * Parametreler: shell, cmd
 * Dönüş: komutun exit kodu */
int	execute_single_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	char	**argv;

	// Token'ları char** argv'ye çevir
	argv = ft_expand_tokens(cmd->args, shell);
	if (!argv || !argv[0])
		return (1);
	
	// BUILTIN komut - parent process'te çalıştır (FORK YOK!)
	if (ft_is_builtin(argv[0]))
	{
		// Redirection backup (shell'in stdin/stdout'u koru)
		int original_stdin = dup(STDIN_FILENO);
		int original_stdout = dup(STDOUT_FILENO);
		int result;
		
		// Redirection'ları ayarla (heredoc dahil)
		if (setup_redirections(cmd) != 0)
		{
			close(original_stdin);
			close(original_stdout);
			return (1);
		}
		
		result = ft_execute_builtin(shell, cmd, 0); // pipe yok, fork yok
		
		// Redirection'ları geri al
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		
		return (result);
	}
	
	// EXTERNAL komut - fork ile çalıştır
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	
	if (pid == 0)
		execute_child_process(shell, cmd, NULL, -1);
	
	// ignore_signals(); // Sinyal fonksiyonu henüz yok
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	
	return (0);
}

/* Ana execute fonksiyonu - komut sayısına göre pipeline/single çağırır
 * Parametreler: shell - shell yapısı
 * Dönüş: komutların exit kodu */
int	execute_commands(t_shell *shell)
{
	int	cmd_count;

	if (!shell->cmd_list || !shell->cmd_list->args || !shell->cmd_list->args->value)
		return (0);
	
	cmd_count = count_commands(shell->cmd_list);
	
	if (cmd_count == 1)
		return (execute_single_command(shell, shell->cmd_list));
	else
		return (execute_pipeline(shell));
}

/* PATH'te executable arar veya absolute path kontrol eder
 * Parametreler: cmd - komut adı, env_list - environment listesi
 * Dönüş: executable path (malloc'lu) veya NULL */
char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	// Absolute/relative path varsa direkt kontrol et
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	
	// PATH environment'tan yol listesini al
	path_env = ft_get_env_value("PATH", env_list);
	if (!path_env)
		return (NULL);
	
	// PATH'i ':' ile böl
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	
	i = 0;
	while (paths[i])
	{
		// Her PATH dizininde komut var mı kontrol et
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		
		free(full_path);
		i++;
	}
	
	ft_free_split(paths);
	return (NULL);
}

/* Komut için input/output redirection'ları ayarlar
 * < input.txt, > output.txt, >> append.txt, heredoc
 * Parametreler: cmd - komut yapısı
 * Dönüş: 0 başarılı, 1 hatalı */
int	setup_redirections(t_cmd *cmd)
{
	int	fd;

	// Input redirection: < file
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			printf("minishell: %s: No such file or directory\n", cmd->input_file);
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
	// Heredoc: << delimiter (main'de önceden işlenmiş)
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	
	// Output redirection: > file veya >> file
	if (cmd->output_file)
	{
		if (cmd->append_mode)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd == -1)
		{
			printf("minishell: %s: Permission denied\n", cmd->output_file);
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (0);
}

/* Komut listesindeki komut sayısını sayar
 * Parametreler: cmd_list - komut listesi
 * Dönüş: komut sayısı */
int	count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* Environment listesini char** array'e çevirir (execve için)
 * Parametreler: env_list - environment listesi
 * Dönüş: char** envp array (malloc'lu) */
char	**env_to_array(t_env *env_list)
{
	char	**envp;
	t_env	*current;
	char	*temp;
	int		count;
	int		i;

	// Environment sayısını say
	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	
	// Array oluştur
	envp = ft_malloc(sizeof(char*) * (count + 1));
	
	// Her environment'ı "KEY=VALUE" formatında ekle
	current = env_list;
	i = 0;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		envp[i] = ft_strjoin(temp, current->value ? current->value : "");
		free(temp);
		current = current->next;
		i++;
	}
	envp[i] = NULL;
	
	return (envp);
}
