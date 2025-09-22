#include "../minishell.h"

static int ft_env_with_clean(char **argv, t_shell *shell)
{
	char	*clean_envp[4];
	char	*pwd;
	pid_t	pid;
	int		status;

	if (!argv[2])
		return (0);
	
	// Minimal temiz environment hazırla
	pwd = getcwd(NULL, 0);
	clean_envp[0] = NULL;
	if (pwd)
	{
		clean_envp[0] = ft_strjoin("PWD=", pwd, shell);
		clean_envp[1] = ft_strdup("SHLVL=1", shell);
		clean_envp[2] = NULL;
		//free(pwd);
	}
	else
	{
		clean_envp[0] = ft_strdup("SHLVL=1", shell);
		clean_envp[1] = NULL;
	}
	
	pid = fork();
	if (pid == 0)
	{
		if (execve(argv[2], &argv[2], clean_envp) == -1)
		{
			perror("env");
			exit(1);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		// Memory temizliği
		// if (clean_envp[0])
		// 	free(clean_envp[0]);
		// if (clean_envp[1])
		// 	free(clean_envp[1]);
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
		if (current->value && ft_strcmp(current->key, "MINISHELL_LEVEL") != 0)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}