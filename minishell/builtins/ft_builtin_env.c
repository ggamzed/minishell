/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <eecegokcece@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 18:48:12 by egokce            #+#    #+#             */
/*   Updated: 2025/09/25 18:48:12 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
