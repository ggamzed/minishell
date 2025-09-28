/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_in_parser_handle_redirect_input.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:46 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:46 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

static char	*ft_join_consecutive_tokens(t_token **current, t_shell *shell,
									int *has_quotes)
{
	char			*delimiter;
	char			*temp;
	t_token_type	type;
	t_token			*token_ptr;

	delimiter = ft_strdup((*current)->value, shell);
	token_ptr = *current;
	type = token_ptr->type;
	while (token_ptr->next && token_ptr->next->space_flag == 0)
	{
		if ((type != PIPE && type != REDIRECT_IN
				&& type != REDIRECT_OUT && type != REDIRECT_APPEND
				&& type != HEREDOC))
			return (delimiter);
		token_ptr = token_ptr->next;
		type = token_ptr->type;
		if (type == SINGLE_QUOTED_STRING || type == DOUBLE_QUOTED_STRING)
			*has_quotes = 1;
		temp = ft_strjoin(delimiter, token_ptr->value, shell);
		delimiter = temp;
	}
	*current = token_ptr;
	return (delimiter);
}

static char	*ft_join_heredoc_delimiter(t_token **current, t_shell *shell,
								int *has_quotes)
{
	t_token_type	type;

	if (!*current)
		return (NULL);
	*has_quotes = 0;
	type = (*current)->type;
	if (type == SINGLE_QUOTED_STRING || type == DOUBLE_QUOTED_STRING)
		*has_quotes = 1;
	return (ft_join_consecutive_tokens(current, shell, has_quotes));
}

int	ft_in_parser_handle_heredoc(t_cmd *cmd, t_token **current, t_shell *shell)
{
	int				temp_fd;
	char			*joined_delimiter;
	t_token_type	token_type;
	int				has_quotes;

	*current = (*current)->next;
	if (!*current)
		return (1);
	token_type = (*current)->type;
	joined_delimiter = ft_join_heredoc_delimiter(current, shell, &has_quotes);
	if (!joined_delimiter)
		return (0);
	temp_fd = ft_process_heredoc(joined_delimiter, shell, !has_quotes);
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	cmd->heredoc_fd = temp_fd;
	cmd->heredoc_delimiter = joined_delimiter;
	cmd->heredoc_type = token_type;
	cmd->heredoc_should_expand = !has_quotes;
	*current = (*current)->next;
	return (1);
}

int	ft_in_parser_handle_redirect_in(t_cmd *cmd, t_token **current,
						t_shell *shell)
{
	char	*filename;

	*current = (*current)->next;
	if (*current && ((*current)->type == WORD
			|| (*current)->type == SINGLE_QUOTED_STRING
			|| (*current)->type == DOUBLE_QUOTED_STRING
			|| (*current)->type == VARIABLE))
	{
		filename = ft_join_redirect_filename(current, shell);
		if (!filename)
			return (0);
		cmd->input_file = filename;
		cmd->input_type = (*current)->type;
		*current = (*current)->next;
	}
	return (1);
}
