/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_single_command.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:41 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:41 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static int	ft_execute_builtin_in_parent(t_shell *shell, t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;
	int	result;

	if (ft_handle_builtin_redirections(cmd, &original_stdin,
			&original_stdout) != 0)
		return (1);
	result = ft_execute_builtin(shell, cmd, 0);
	ft_restore_redirections(original_stdin, original_stdout);
	return (result);
}

static int	ft_execute_external_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		ft_execute_child_process(shell, cmd, NULL, -1);
		ft_cleanup_and_exit(shell, 1);
	}
	if (cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	ft_ignore_signals();
	waitpid(pid, &status, 0);
	return (ft_check_only_child_status(status));
}

int	ft_execute_single_command(t_shell *shell, t_cmd *cmd)
{
	if (ft_is_builtin(cmd->expanded_argv[0]))
		return (ft_execute_builtin_in_parent(shell, cmd));
	else
		return (ft_execute_external_command(shell, cmd));
}
