/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate_redirections.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:35 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:35 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_skip_redirection_operator(char *line, int *i)
{
	if (line[*i] == '<')
	{
		(*i)++;
		if (line[*i] == '<')
			(*i)++;
	}
	else if (line[*i] == '>')
	{
		(*i)++;
		if (line[*i] == '>')
			(*i)++;
	}
}

static int	ft_check_filename_after_redirection(char *line, int i, int is_quote)
{
	while (line[i] && ft_is_white_space(line[i]))
		i++;
	if (!line[i] || line[i] == '|' || line[i] == '<' || line[i] == '>')
	{
		if (is_quote)
			return (0);
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (1);
	}
	return (0);
}

int	ft_validate_redirections(char *line)
{
	int	i;
	int	is_quote;

	i = 0;
	is_quote = 0;
	while (line[i])
	{
		if ((line[i] == '"') || (line[i] == '\''))
			is_quote++;
		if (line[i] == '<' || line[i] == '>')
		{
			ft_skip_redirection_operator(line, &i);
			if (ft_check_filename_after_redirection(line, i, is_quote) == 1)
				return (1);
		}
		else
			i++;
	}
	return (0);
}
