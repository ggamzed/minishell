#include "../minishell.h"

static int	ft_execute_with_clean_env(char **argv, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(argv[2], &argv[2], NULL) == -1)
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

// static int	ft_env_with_clean(char **argv, t_shell *shell)
// {
// 	char	*clean_envp[3];
// 	char	*pwd;

// 	if (!argv[2])
// 		return (0);
// 	pwd = getcwd(NULL, 0);
// 	if (pwd)
// 	{
// 		clean_envp[0] = ft_strjoin("PWD=", pwd, shell);
// 		free(pwd);
// 	}
// 	else
// 		clean_envp[0] = ft_strdup("PWD=/", shell);
// 	clean_envp[2] = ft_strdup("SHLVL=1", shell); //bu kapandığında da doğru çalışıyor, test et
// 	clean_envp[1] = NULL;
// 	return (ft_execute_with_clean_env(argv, clean_envp, shell));
// }

int	ft_builtin_env(char **argv, t_env *env_list, t_shell *shell)
{
	t_env	*current;

	if (argv && argv[1])
	{
		if (ft_strcmp(argv[1], "-i") == 0)
			return (ft_execute_with_clean_env(argv, shell));
		else
		{
			ft_putstr_fd("env: invalid option\n", 2);
			return (1);
		}
	}
	current = env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
