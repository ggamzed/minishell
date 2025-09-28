/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multiple_command_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:59 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 20:24:34 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int	ft_create_pipe(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}
	}
	return (0);
}

static int	ft_check_child_status(int status, pid_t last_pid,
								pid_t waited_pid, int *quit_printed)
{
	int	sig;

	if (WIFEXITED(status))
	{
		if (last_pid == -1 || waited_pid == last_pid)
			return (WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT && !(*quit_printed))
		{
			ft_putstr_fd("Quit (core dumped)\n", 2);
			*quit_printed = 1;
		}
		if (last_pid == -1 || waited_pid == last_pid)
			return (128 + sig);
	}
	return (-1);
}

int	ft_wait_all_children(pid_t last_pid)
{
	int		status;
	int		last_status;
	int		quit_printed;
	int		temp_status;
	pid_t	waited_pid;

	last_status = 0;
	quit_printed = 0;
	waited_pid = wait(&status);
	while (waited_pid > 0)
	{
		temp_status = ft_check_child_status(status, last_pid, waited_pid,
				&quit_printed);
		if (temp_status != -1)
			last_status = temp_status;
		waited_pid = wait(&status);
	}
	return (last_status);
}

void	ft_handle_pipe_fds(t_cmd *current, int *pipefd, int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	if (current->heredoc_fd != -1)
	{
		close(current->heredoc_fd);
		current->heredoc_fd = -1;
	}
}
