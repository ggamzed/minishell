/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_and_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:10 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:11 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_extract_variable_name(char *str, int *i, t_shell *shell)
{
	int		start;

	if (str[*i] == '$')
		(*i)++;
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start, shell));
}

char	*ft_extract_and_expand_var(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*env_value;

	if (str[*i] != '$')
		return (NULL);
	if (str[*i] == '$' && str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_handle_exit_status(shell));
	}
	if (!ft_isalpha(str[*i + 1]) && str[*i + 1] != '_')
		return (NULL);
	var_name = ft_extract_variable_name(str, i, shell);
	if (!var_name || !*var_name)
	{
		return (ft_strdup("", shell));
	}
	env_value = ft_get_env_value(var_name, shell->env_list);
	if (env_value)
		return (ft_strdup(env_value, shell));
	return (ft_strdup("", shell));
}
