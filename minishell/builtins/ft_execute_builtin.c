/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:02 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:02 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, ".") == 0)
		return (1);
	return (0);
}

static int	ft_execute_builtin_function(char **argv, t_shell *shell,
									int in_pipe)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_builtin_echo(argv));
	else if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_builtin_cd(argv, shell->env_list, shell));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_builtin_pwd(argv));
	else if (ft_strcmp(argv[0], "export") == 0)
		return (ft_builtin_export(argv, &shell->env_list, shell));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_builtin_unset(argv, &shell->env_list, shell));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (ft_builtin_env(argv, shell->env_list));
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_builtin_exit(argv, shell, in_pipe));
	return (1);
}

int	ft_execute_builtin(t_shell *shell, t_cmd *cmd, int in_pipe)
{
	if (!cmd->expanded_argv || !cmd->expanded_argv[0])
		return (1);
	if (!ft_is_builtin(cmd->expanded_argv[0]))
		return (1);
	if (ft_strcmp(cmd->expanded_argv[0], ".") == 0)
	{
		if (!cmd->expanded_argv[1])
		{
			ft_putstr_fd("minishell: .: filename argument required\n", 2);
			return (2);
		}
		return (1);
	}
	return (ft_execute_builtin_function(cmd->expanded_argv, shell, in_pipe));
}
