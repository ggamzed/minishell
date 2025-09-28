/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_multiple_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:38 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:38 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

static pid_t	ft_create_child_and_execute(t_shell *shell, t_cmd *cmd,
										int pipefd[2], int prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
		ft_execute_child_process(shell, cmd, pipefd, prev_fd);
	return (pid);
}

int	ft_execute_multiple_command(t_shell *shell)
{
	t_cmd	*current;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	pid_t	last_pid;

	current = shell->cmd_list;
	prev_fd = -1;
	last_pid = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	while (current)
	{
		if (ft_create_pipe(current, pipefd))
			return (1);
		pid = ft_create_child_and_execute(shell, current, pipefd, prev_fd);
		if (pid == -1)
			return (1);
		if (!current->next)
			last_pid = pid;
		ft_handle_pipe_fds(current, pipefd, &prev_fd);
		current = current->next;
	}
	return (ft_wait_all_children(last_pid));
}
