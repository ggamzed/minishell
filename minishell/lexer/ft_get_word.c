/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:19 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 13:49:38 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_handle_quoted_word(char *line, int *i, int *start)
{
	char	quote;

	quote = line[*i];
	(*i)++;
	*start = *i;
	while (line[*i])
	{
		if (line[*i] == quote)
			break ;
		(*i)++;
	}
	(*i)++;
	return (*i - *start - 1);
}

static int	ft_handle_variable_word(char *line, int *i, int start)
{
	(*i)++;
	if (line[*i] == '?')
	{
		(*i)++;
		return (*i - start);
	}
	if (ft_isalpha(line[*i]) || line[*i] == '_')
	{
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		return (*i - start);
	}
	return (1);
}

static int	ft_is_word_delimiter(char c)
{
	if (ft_is_white_space(c) || c == '|' || c == '<' || c == '>')
		return (1);
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

static int	ft_handle_normal_word(char *line, int *i, int start)
{
	while (line[*i] && !ft_is_word_delimiter(line[*i]))
	{
		if (line[*i] == '\\' && line[*i + 1] != '\0')
			*i += 2;
		else if (line[*i] == '$')
			break ;
		else
			(*i)++;
	}
	return (*i - start);
}

char	*ft_get_word(char *line, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	if (line[*i] == '\'' || line[*i] == '"')
		len = ft_handle_quoted_word(line, i, &start);
	else if (line[*i] == '$')
		len = ft_handle_variable_word(line, i, start);
	else
		len = ft_handle_normal_word(line, i, start);
	word = ft_substr(line, start, len, shell);
	return (word);
}
