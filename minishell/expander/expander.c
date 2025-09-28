/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:07 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:07 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_handle_exit_status(t_shell *shell)
{
	return (ft_itoa(shell->exit_status, shell));
}

static int	ft_count_args(t_token *args)
{
	t_token	*current;
	int		count;

	count = 0;
	current = args;
	while (current)
	{
		current = current->next;
		count++;
	}
	return (count);
}

static char	*ft_expand_token_value(char *value, t_token_type type,
							t_shell *shell, int is_heredoc_delimiter)
{
	int	i;

	i = 0;
	if (!value)
		return (NULL);
	if (is_heredoc_delimiter)
		return (ft_strdup(value, shell));
	if (type == VARIABLE)
		return (ft_extract_and_expand_var(value, &i, shell));
	else if (type == EXIT_STATUS)
		return (ft_handle_exit_status(shell));
	else if (type == DOUBLE_QUOTED_STRING)
		return (ft_expand_double_quoted(value, shell));
	else if (type == SINGLE_QUOTED_STRING)
		return (ft_strdup(value, shell));
	else if (type == WORD)
		return (ft_handle_word(value, shell));
	else
		return (ft_strdup(value, shell));
}

char	**ft_expand_tokens(t_token *args, t_shell *shell)
{
	char	**expanded_argv;
	char	**joined_argv;
	char	**argv;
	t_token	*current;
	int		i;

	expanded_argv = ft_malloc(sizeof(char *) * (ft_count_args(args) + 1),
			shell);
	if (!expanded_argv)
		ft_cleanup_and_exit(shell, 1);
	current = args;
	i = 0;
	while (current)
	{
		expanded_argv[i++] = ft_expand_token_value(current->value,
				current->type, shell, 0);
		current = current->next;
	}
	expanded_argv[i] = NULL;
	joined_argv = ft_join_tokens_back(expanded_argv, args, shell);
	argv = ft_handle_word_split(joined_argv, args, shell);
	return (argv);
}
