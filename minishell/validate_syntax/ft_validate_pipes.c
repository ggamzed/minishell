/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:30 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:30 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_check_pipe_boundaries(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(line) - 1;
	while (line[i] && ft_is_white_space(line[i]))
		i++;
	while (j >= 0 && ft_is_white_space(line[j]))
		j--;
	if (line[i] == '|' || (j >= 0 && line[j] == '|'))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

static int	ft_handle_pipe_found(char *line, int *i, int is_quote)
{
	(*i)++;
	while (line[*i] && ft_is_white_space(line[*i]))
		(*i)++;
	if (line[*i] == '|')
	{
		if (is_quote)
			return (0);
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (-1);
}

static int	ft_check_double_pipes(char *line)
{
	int	i;
	int	is_quote;
	int	pipe_result;

	i = 0;
	is_quote = 0;
	while (line[i])
	{
		if ((line[i] == '"') || (line[i] == '\''))
			is_quote++;
		if (line[i] == '|')
		{
			pipe_result = ft_handle_pipe_found(line, &i, is_quote);
			if (pipe_result != -1)
				return (pipe_result);
		}
		else
			i++;
	}
	return (0);
}

int	ft_validate_pipes(char *line)
{
	if (ft_check_pipe_boundaries(line) == 1)
		return (1);
	if (ft_check_double_pipes(line) == 1)
		return (1);
	return (0);
}
