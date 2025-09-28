/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_input_redirection.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:01 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 12:46:39 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

static int	ft_check_directory_input(char *input_file)
{
	struct stat	st;

	if (stat(input_file, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_print_error_msg(input_file, ": Is a directory\n");
			return (1);
		}
	}
	return (0);
}

static int	ft_open_input_file(char *input_file)
{
	int	fd;

	fd = open(input_file, O_RDONLY);
	if (fd == -1)
	{
		ft_print_error_msg(input_file, ": ");
		perror("");
		return (1);
	}
	return (fd);
}

int	ft_handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	if (ft_check_directory_input(cmd->input_file))
		return (1);
	fd = ft_open_input_file(cmd->input_file);
	if (fd == -1)
		return (1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
