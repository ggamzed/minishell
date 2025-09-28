/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:33 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:33 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_process_quote_content(char *line, int *i, char quote)
{
	while (line[*i])
	{
		if (quote == '"' && line[*i] == '\\' && line[*i + 1] != '\0')
		{
			*i += 2;
			continue ;
		}
		if (line[*i] == quote)
			return (0);
		(*i)++;
	}
	ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
	return (1);
}

int	ft_validate_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"')
			&& (i == 0 || line[i - 1] != '\\'))
		{
			quote = line[i];
			i++;
			if (ft_process_quote_content(line, &i, quote))
				return (1);
		}
		i++;
	}
	return (0);
}
