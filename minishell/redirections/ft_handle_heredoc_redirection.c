/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc_redirection.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:59 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:59 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

int	ft_handle_heredoc_redirection(t_cmd *cmd)
{
	if (cmd->heredoc_fd == -1)
		return (0);
	dup2(cmd->heredoc_fd, STDIN_FILENO);
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
	return (0);
}
