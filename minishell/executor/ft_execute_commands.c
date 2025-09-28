/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:35 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:36 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static int	ft_count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static int	ft_execute_redirection_only(t_cmd *cmd, t_shell *shell)
{
	int	fd;

	if (cmd->output_file)
	{
		cmd->output_file = ft_expand_double_quoted(cmd->output_file, shell);
		if (cmd->append_mode)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_print_error_msg(cmd->output_file, ": ");
			perror("");
			return (1);
		}
		close(fd);
	}
	return (0);
}

int	ft_execute_commands(t_shell *shell)
{
	if (!shell->cmd_list)
		return (0);
	if (!shell->cmd_list->args)
	{
		if (shell->cmd_list->output_file || shell->cmd_list->input_file
			|| shell->cmd_list->heredoc_fd != -1)
		{
			return (ft_execute_redirection_only(shell->cmd_list, shell));
		}
		return (0);
	}
	if (ft_count_commands(shell->cmd_list) == 1)
		return (ft_execute_single_command(shell, shell->cmd_list));
	else
		return (ft_execute_multiple_command(shell));
}
