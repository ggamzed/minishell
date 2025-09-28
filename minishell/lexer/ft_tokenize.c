/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:24 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:24 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_skip_spaces(char *line, int *i, int *had_space)
{
	while (line[*i] && ft_is_white_space(line[*i]))
	{
		*had_space = 1;
		(*i)++;
	}
}

static int	ft_check_space_after(char *line, int i)
{
	if (line[i] && ft_is_white_space(line[i]))
		return (1);
	return (0);
}

static t_token	*ft_process_operator(char *line, int *i, int had_space,
									t_shell *shell)
{
	t_token_type	type;
	t_token			*token;

	type = ft_get_operator_type(line, i);
	token = ft_create_token(type, NULL, shell);
	if (!token)
		return (NULL);
	token->space_flag = had_space;
	return (token);
}

static t_token	*ft_process_word(char *line, int *i, int had_space,
								t_shell *shell)
{
	t_token_type	type;
	t_token			*token;
	char			*word;

	type = ft_get_word_type(line, *i);
	word = ft_get_word(line, i, shell);
	if (!word)
		return (NULL);
	token = ft_create_token(type, word, shell);
	if (!token)
		return (NULL);
	token->space_flag = had_space;
	return (token);
}

t_token	*ft_tokenize(char *line, t_shell *shell)
{
	t_token	*head_of_token;
	t_token	*token;
	int		i;
	int		had_space;

	head_of_token = NULL;
	i = 0;
	had_space = 1;
	while (line[i])
	{
		ft_skip_spaces(line, &i, &had_space);
		if (!line[i])
			break ;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			token = ft_process_operator(line, &i, had_space, shell);
		else
			token = ft_process_word(line, &i, had_space, shell);
		if (!token)
			return (NULL);
		ft_add_token(&head_of_token, token);
		had_space = ft_check_space_after(line, i);
	}
	return (head_of_token);
}
