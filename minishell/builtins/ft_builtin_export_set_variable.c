/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export_set_variable.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:18:46 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:32:09 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_handle_append_variable(char *arg, int append_pos,
									t_env **env_list, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*existing;
	char	*new_value;

	key = ft_substr(arg, 0, append_pos, shell);
	value = ft_strdup(arg + append_pos + 2, shell);
	existing = ft_get_env_value(key, *env_list);
	if (existing)
		new_value = ft_strjoin(existing, value, shell);
	else
		new_value = value;
	ft_set_env_value(key, new_value, env_list, shell);
	ft_set_env_value(key, new_value, &shell->export_list, shell);
}

static void	ft_handle_normal_variable(char *arg, t_env **env_list,
									t_shell *shell)
{
	char	*equals_sign;
	char	*key;
	char	*value;

	equals_sign = ft_strchr(arg, '=');
	if (equals_sign)
	{
		key = ft_substr(arg, 0, equals_sign - arg, shell);
		value = ft_strdup(equals_sign + 1, shell);
		ft_set_env_value(key, value, env_list, shell);
		ft_set_env_value(key, value, &shell->export_list, shell);
	}
	else
	{
		if (ft_get_env_value(arg, shell->export_list))
			return ;
		else
			ft_set_env_value(arg, NULL, &shell->export_list, shell);
	}
}

void	ft_set_export_variable(char *arg, t_env **env_list, t_shell *shell)
{
	int	append_pos;
	int	i;

	i = 0;
	append_pos = 0;
	while (arg[i])
	{
		if (arg[i] == '+' && arg[i + 1] == '=')
		{
			append_pos = i;
			break ;
		}
		i++;
	}
	if (append_pos == 0)
		ft_handle_normal_variable(arg, env_list, shell);
	else
		ft_handle_append_variable(arg, append_pos, env_list, shell);
}
