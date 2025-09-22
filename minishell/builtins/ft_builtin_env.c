#include "../minishell.h"

static int ft_env_with_clean(char **argv, t_shell *shell)
{
	char	*clean_envp[3];  // PWD, SHLVL, NULL için 3 eleman
	char	*pwd;
	pid_t	pid;
	int		status;

	if (!argv[2])
		return (0);
	
	// PWD ayarla
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		clean_envp[0] = ft_strjoin("PWD=", pwd, shell);
		free(pwd);
	}
	else
	{
		clean_envp[0] = ft_strdup("PWD=/", shell);
	}
	
	// SHLVL=1 ayarla (env -i durumu)
	clean_envp[1] = ft_strdup("SHLVL=1", shell);
	clean_envp[2] = NULL;
	
	pid = fork();
	if (pid == 0)
	{
		if (execve(argv[2], &argv[2], clean_envp) == -1)
		{
			perror("env");
			ft_free_mem_tracker(shell->mem_tracker);
			free(shell);
			exit(1);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}

int	ft_builtin_env(char **argv, t_env *env_list, t_shell *shell)
{
	t_env	*current;

	// Parametreleri kontrol et
	if (argv && argv[1])
	{
		if (ft_strcmp(argv[1], "-i") == 0)
			return ft_env_with_clean(argv, shell);
		else
		{
			ft_putstr_fd("env: invalid option\n", 2);
			return (1);
		}
	}

	// Parametresiz env - tüm environment değişkenlerini listele
	current = env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}