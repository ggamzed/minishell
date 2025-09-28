/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:28 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:29 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_should_split(t_token *token_start)
{
	t_token	*current;

	current = token_start;
	while (current)
	{
		if (current->type == VARIABLE)
			return (1);
		if (current->next && current->next->space_flag == 0)
			current = current->next;
		else
			break ;
	}
	return (0);
}

static int	ft_calculate_final_count(char **joined_argv, t_token *tokens,
								t_shell *shell)
{
	t_token	*current;
	char	**split_words;
	int		word_count;
	int		i;

	current = tokens;
	word_count = 0;
	i = 0;
	while (current)
	{
		if (ft_should_split(current))
		{
			split_words = ft_split(joined_argv[i], ' ', shell);
			word_count += ft_count_split_words(split_words);
		}
		else
			word_count++;
		ft_skip_token_group(&current);
		i++;
	}
	return (word_count);
}

static int	ft_add_split_words(char **argv, char **split_words,
							int *k, t_shell *shell)
{
	int	j;

	if (!split_words)
		return (0);
	j = 0;
	while (split_words[j])
	{
		argv[*k] = ft_strdup(split_words[j], shell);
		(*k)++;
		j++;
	}
	return (1);
}

static int	ft_process_splitting(char **argv, char **joined_argv,
			t_token *tokens, t_shell *shell)
{
	t_token	*current;
	char	**split_words;
	int		i;
	int		k;

	current = tokens;
	i = 0;
	k = 0;
	while (current)
	{
		if (ft_should_split(current))
		{
			split_words = ft_split(joined_argv[i], ' ', shell);
			if (!ft_add_split_words(argv, split_words, &k, shell))
				argv[k++] = ft_strdup(joined_argv[i], shell);
		}
		else
			argv[k++] = ft_strdup(joined_argv[i], shell);
		ft_skip_token_group(&current);
		i++;
	}
	argv[k] = NULL;
	return (1);
}

char	**ft_handle_word_split(char **joined_argv, t_token *org_tokens,
						t_shell *shell)
{
	char	**argv;
	int		argv_count;
	int		check;

	argv_count = ft_calculate_final_count(joined_argv, org_tokens, shell);
	argv = ft_malloc(sizeof(char *) * (argv_count + 1), shell);
	if (!argv)
		ft_cleanup_and_exit(shell, 1);
	check = ft_process_splitting(argv, joined_argv, org_tokens, shell);
	if (!check)
		return (NULL);
	return (argv);
}
