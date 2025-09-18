#include "../minishell.h"

// pipe zincirindeki her komutun input/output bağlantılarını ayarlar.
/*
int *pipefd → Şu anki komut ile sonraki arasındaki pipe
int prev_fd → Önceki pipe'ın okuma ucu (önceki komuttan gelen data)
t_cmd *cmd → Şu anki komut
*/
static void	ft_setup_pipe_connections(int *pipefd, int prev_fd, t_cmd *cmd)
{
	if (prev_fd != -1) // Önceki pipe'dan gelen input
	{
		dup2(prev_fd, STDIN_FILENO); // STDIN'i önceki pipe'a bağla
		close(prev_fd); // prev_fd'yi kapat
	}
	if (cmd->next) // Sonraki komut için pipe var
	{
		dup2(pipefd[1], STDOUT_FILENO); // STDOUT'u pipe'ın yazma ucuna bağla
		close(pipefd[1]); // yazma ucunu kapat
		close(pipefd[0]); // okuma ucunu da kapat (child bu ucu kullanmaz)
	}
	// Eğer cmd->next yoksa ve pipefd varsa, pipe fd'lerini kapat
	else if (pipefd)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

static void	ft_execute_builtin_in_child(t_shell *shell, t_cmd *cmd)
{
   int	exit_code;

   exit_code = ft_execute_builtin(shell, cmd, 1);
   exit(exit_code);
}

static void	ft_execute_external_in_child(t_shell *shell, t_cmd *cmd)
{
	char	*executable;
	char	**envp;

	executable = ft_find_executable(cmd->expanded_argv[0], shell->env_list, shell); //çalıştırılabilir path
	if (!executable)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->expanded_argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_mem_tracker(shell->mem_tracker);
		free(shell);
		exit(127);
	}
	envp = ft_env_to_array(shell->env_list, shell);
	execve(executable, cmd->expanded_argv, envp);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->expanded_argv[0], 2);
	ft_putstr_fd(": execution failed: ", 2);
	perror("");

	exit(126);
}

int	ft_execute_child_process(t_shell *shell, t_cmd *cmd, int *pipefd, int prev_fd)
{
	// default_signals(); // Sinyal fonksiyonu henüz yok
	ft_setup_pipe_connections(pipefd, prev_fd, cmd);
	if (ft_handle_redirections(cmd) != 0) // şuan bu fonksiyon yok
		exit(1);
	if (!cmd->expanded_argv || !cmd->expanded_argv[0])
		exit(1);
	if (ft_is_builtin(cmd->expanded_argv[0])) //multiple_command fonksiyonu da bu fonksiyonu çağıracak o yüzden bu satır gerekli
		ft_execute_builtin_in_child(shell, cmd);
	else
		ft_execute_external_in_child(shell, cmd);
	return (0); // Buraya hiç ulaşmaz, exit() ile çıkar
}
