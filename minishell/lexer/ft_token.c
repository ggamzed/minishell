/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:21 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:21 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_create_token(t_token_type type, char *value, t_shell *shell)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), shell);
	if (!token)
		ft_cleanup_and_exit(shell, 1);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	ft_add_token(t_token **token_list, t_token *new_token)
{
	t_token	*current;

	if (!*token_list)
	{
		*token_list = new_token;
		return ;
	}
	current = *token_list;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
