/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_output_redirection.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:04 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 12:51:00 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

static int	ft_open_output_file(char *filename, int append_mode)
{
	int	fd;

	if (append_mode == 1)
	{
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

static int	ft_print_file_error(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	if (errno == EISDIR)
		ft_putstr_fd(": is a directory\n", 2);
	else
	{
		ft_putstr_fd(": ", 2);
		perror("");
	}
	return (1);
}

static int	ft_process_multiple_outputs(t_cmd *cmd)
{
	int	fd;
	int	final_fd;
	int	i;

	final_fd = -1;
	i = 0;
	while (i < cmd->output_count)
	{
		fd = ft_open_output_file(cmd->all_output_files[i],
				cmd->all_append_modes[i]);
		if (fd == -1)
			return (ft_print_file_error(cmd->all_output_files[i]));
		if (i == cmd->output_count - 1)
			final_fd = fd;
		else
			close(fd);
		i++;
	}
	if (final_fd != -1)
	{
		dup2(final_fd, STDOUT_FILENO);
		close(final_fd);
	}
	return (0);
}

static int	ft_handle_single_output(t_cmd *cmd)
{
	int	fd;

	if (!cmd->output_file)
		return (0);
	fd = ft_open_output_file(cmd->output_file, cmd->append_mode);
	if (fd == -1)
		return (ft_print_file_error(cmd->output_file));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	ft_handle_output_redirection(t_cmd *cmd)
{
	if (cmd->output_count > 0)
		return (ft_process_multiple_outputs(cmd));
	else
		return (ft_handle_single_output(cmd));
}
