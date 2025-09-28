/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:15 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:19 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	ft_free_fds(t_cmd *commands)
{
	t_cmd	*current;
	t_cmd	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		if (current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = next;
	}
}

void	ft_cleanup_and_exit(t_shell *shell, int exit_code)
{
	ft_free_fds(shell->cmd_list);
	ft_free_mem_tracker(shell->mem_tracker);
	free(shell);
	exit(exit_code);
}

void	ft_print_error_msg(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
}

void	*ft_malloc(size_t size, t_shell *shell)
{
	void	*ptr;
	t_mem	*mem_node;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("malloc failed", 2);
		return (NULL);
	}
	if (!shell || !shell->mem_tracker)
		return (ptr);
	mem_node = malloc(sizeof(t_mem));
	if (!mem_node)
	{
		ft_putstr_fd("malloc for memory tracker failed", 2);
		free(ptr);
		return (NULL);
	}
	mem_node->ptr = ptr;
	mem_node->next = *shell->mem_tracker;
	*shell->mem_tracker = mem_node;
	return (ptr);
}

void	ft_free_mem_tracker(t_mem **mem_tracker)
{
	t_mem	*curr;
	t_mem	*tmp;

	if (!mem_tracker)
		return ;
	curr = *mem_tracker;
	while (curr)
	{
		tmp = curr->next;
		if (curr->ptr)
		{
			free(curr->ptr);
			curr->ptr = NULL;
		}
		free(curr);
		curr = tmp;
	}
	*mem_tracker = NULL;
}
