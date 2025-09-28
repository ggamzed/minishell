/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:13 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:14 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_handle_tilde_base(const char *value, char **suffix,
								t_shell *shell)
{
	if (value[1] == '+' || value[1] == '-')
	{
		*suffix = (char *)(value + 2);
		if (value[1] == '+')
			return (ft_get_env_value("PWD", shell->env_list));
		else
			return (ft_get_env_value("OLDPWD", shell->env_list));
	}
	else
	{
		*suffix = (char *)(value + 1);
		return (ft_get_env_value("HOME", shell->env_list));
	}
}

char	*ft_expand_tilde(const char *value, t_shell *shell)
{
	char	*base;
	char	*suffix;
	char	*expanded;
	char	*temp;

	if (!value || value[0] != '~')
		return (NULL);
	if (value[1] != '\0' && value[1] != '/'
		&& value[1] != '+' && value[1] != '-')
		return (NULL);
	base = ft_handle_tilde_base(value, &suffix, shell);
	if (!base)
		return (ft_strdup(value, shell));
	if (suffix[0] == '\0')
		return (ft_strdup(base, shell));
	if (suffix[0] == '/')
		return (ft_strjoin(base, suffix, shell));
	temp = ft_strjoin(base, "/", shell);
	expanded = ft_strjoin(temp, suffix, shell);
	return (expanded);
}
