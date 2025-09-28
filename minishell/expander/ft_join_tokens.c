/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_join_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:17 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:18 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_token_groups(t_token *tokens)
{
	t_token	*current;
	int		count;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		while (current->next && current->next->space_flag == 0)
			current = current->next;
		current = current->next;
	}
	return (count);
}

static char	*ft_join_token_group(char **expanded_argv, int *i,
								t_token **current, t_shell *shell)
{
	char	*merged_word;
	char	*temp;

	merged_word = ft_strdup(expanded_argv[*i], shell);
	(*i)++;
	*current = (*current)->next;
	while (*current && (*current)->space_flag == 0)
	{
		temp = ft_strjoin(merged_word, expanded_argv[*i], shell);
		merged_word = temp;
		(*i)++;
		*current = (*current)->next;
	}
	return (merged_word);
}

static char	**ft_allocate_for_joined(int count, t_shell *shell)
{
	char	**joined_argv;

	joined_argv = ft_malloc(sizeof(char *) * (count + 1), shell);
	if (!joined_argv)
		ft_cleanup_and_exit(shell, 1);
	return (joined_argv);
}

static int	ft_handle_token_groups(char **joined_argv, char **expanded_argv,
							t_token *tokens, t_shell *shell)
{
	t_token	*current;
	char	*joined_word;
	int		i;
	int		j;

	current = tokens;
	i = 0;
	j = 0;
	while (current)
	{
		joined_word = ft_join_token_group(expanded_argv, &i, &current, shell);
		if (!joined_word)
			return (0);
		joined_argv[j] = joined_word;
		j++;
	}
	joined_argv[j] = NULL;
	return (1);
}

char	**ft_join_tokens_back(char **expanded_argv,
				t_token *seperate_tokens, t_shell *shell)
{
	char	**joined_argv;
	int		group_count;
	int		check;

	group_count = ft_count_token_groups(seperate_tokens);
	joined_argv = ft_allocate_for_joined(group_count, shell);
	if (!joined_argv)
		return (NULL);
	check = ft_handle_token_groups(joined_argv, expanded_argv,
			seperate_tokens, shell);
	if (!check)
		return (NULL);
	return (joined_argv);
}
