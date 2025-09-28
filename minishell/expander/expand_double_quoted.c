/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double_quoted.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:05 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:06 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_append_char(char *str, char c, t_shell *shell)
{
	char	temp[2];
	char	*temp_str;

	temp[0] = c;
	temp[1] = '\0';
	temp_str = ft_strdup(temp, shell);
	return (ft_strjoin(str, temp_str, shell));
}

char	*ft_expand_double_quoted(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("", shell);
	i = 0;
	while (str[i])
	{
		temp = ft_extract_and_expand_var(str, &i, shell);
		if (temp)
			result = ft_strjoin(result, temp, shell);
		else
		{
			result = ft_append_char(result, str[i], shell);
			i++;
		}
	}
	return (result);
}
