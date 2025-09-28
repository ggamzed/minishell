/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:11 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:11 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_parse_env_value(char *env_str, t_shell *shell)
{
	char	*equals_sign;
	char	*value;

	equals_sign = ft_strchr(env_str, '=');
	if (!equals_sign)
		return (NULL);
	value = ft_strdup(equals_sign + 1, shell);
	return (value);
}

static char	*ft_parse_env_key(char *env_str, t_shell *shell)
{
	char	*equals_sign;
	char	*key;

	equals_sign = ft_strchr(env_str, '=');
	if (!equals_sign)
		return (NULL);
	key = ft_substr(env_str, 0, equals_sign - env_str, shell);
	return (key);
}

int	ft_parsing_env_entry(char *env_str, t_env **env_list, t_shell *shell)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	key = ft_parse_env_key(env_str, shell);
	if (!key)
		return (0);
	value = ft_parse_env_value(env_str, shell);
	if (!value)
		return (0);
	new_node = ft_create_env_node(key, value, shell);
	if (!new_node)
		return (0);
	ft_add_env_node(env_list, new_node);
	return (1);
}
