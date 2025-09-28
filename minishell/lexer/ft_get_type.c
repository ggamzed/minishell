/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:16 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:17 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token_type	ft_get_word_type(char *line, int i)
{
	if (line[i] == '$' && line[i + 1] == '?')
		return (EXIT_STATUS);
	if (line[i] == '$' && (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
		return (VARIABLE);
	if (line[i] == '\'')
		return (SINGLE_QUOTED_STRING);
	if (line[i] == '"')
		return (DOUBLE_QUOTED_STRING);
	return (WORD);
}

t_token_type	ft_get_operator_type(char *line, int *i)
{
	if (line[*i] == '|' && ++(*i))
		return (PIPE);
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			(*i += 2);
			return (HEREDOC);
		}
		++(*i);
		return (REDIRECT_IN);
	}
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			(*i += 2);
			return (REDIRECT_APPEND);
		}
		++(*i);
		return (REDIRECT_OUT);
	}
	return (ft_get_word_type(line, *i));
}
