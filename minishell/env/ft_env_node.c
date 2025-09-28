/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:04 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:04 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_create_env_node(char *key, char *value, t_shell *shell)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env), shell);
	if (!node)
		ft_cleanup_and_exit(shell, 1);
	node->key = ft_strdup(key, shell);
	if (value)
		node->value = ft_strdup(value, shell);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	ft_add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
