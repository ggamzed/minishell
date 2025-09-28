/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_child_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:31 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:31 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

static void	ft_execute_builtin_in_child(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	exit_code = ft_execute_builtin(shell, cmd, 1);
	ft_cleanup_and_exit(shell, exit_code);
}

static void	ft_execute_external_in_child(t_shell *shell, t_cmd *cmd)
{
	char	*executable;
	char	**envp;

	executable = ft_find_executable(cmd->expanded_argv[0],
			shell->env_list, shell);
	if (!executable)
		ft_exec_error(shell, cmd->expanded_argv[0]);
	ft_update_shlvl(shell);
	envp = ft_env_to_array(shell->env_list, shell);
	execve(executable, cmd->expanded_argv, envp);
	perror("execve");
	ft_cleanup_and_exit(shell, 126);
}

int	ft_execute_child_process(t_shell *shell, t_cmd *cmd, int *pipefd,
						int prev_fd)
{
	ft_default_signals();
	ft_setup_pipe_connections(pipefd, prev_fd, cmd);
	if (ft_handle_redirections(cmd) != 0)
	{
		ft_cleanup_and_exit(shell, 1);
	}
	if (!cmd->expanded_argv || !cmd->expanded_argv[0])
	{
		ft_cleanup_and_exit(shell, 1);
	}
	if (ft_is_builtin(cmd->expanded_argv[0]))
		ft_execute_builtin_in_child(shell, cmd);
	else
		ft_execute_external_in_child(shell, cmd);
	return (0);
}
