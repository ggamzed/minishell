/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:56 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:56 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_add_command_list(t_cmd **command_list, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*command_list)
	{
		*command_list = new_cmd;
		return ;
	}
	current = *command_list;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

t_cmd	*ft_parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*command_list;
	t_cmd	*cmd;
	t_token	*current;

	command_list = NULL;
	current = tokens;
	while (current)
	{
		cmd = ft_parse_command(&current, shell);
		if (!cmd)
			return (NULL);
		ft_add_command_list(&command_list, cmd);
		if (current && current->type == PIPE)
			current = current->next;
	}
	return (command_list);
}
