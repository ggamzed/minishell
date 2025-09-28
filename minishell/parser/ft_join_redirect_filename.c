/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_join_redirect_filename.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:51 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:52 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_join_filename_tokens(t_token **current, t_shell *shell)
{
	char	*filename;
	char	*temp;
	t_token	*token_ptr;

	filename = ft_strdup((*current)->value, shell);
	token_ptr = *current;
	while (token_ptr->next && token_ptr->next->space_flag == 0
		&& token_ptr->next->type != PIPE
		&& token_ptr->next->type != REDIRECT_IN
		&& token_ptr->next->type != REDIRECT_OUT
		&& token_ptr->next->type != REDIRECT_APPEND
		&& token_ptr->next->type != HEREDOC)
	{
		token_ptr = token_ptr->next;
		temp = ft_strjoin(filename, token_ptr->value, shell);
		filename = temp;
	}
	*current = token_ptr;
	return (filename);
}

static char	*ft_expand_home_directory(char *filename, t_shell *shell)
{
	char	*home;
	char	*path_part;
	char	*expanded_filename;

	home = ft_get_env_value("HOME", shell->env_list);
	if (!home)
		return (filename);
	if (ft_strcmp(filename, "~") == 0)
		expanded_filename = ft_strdup(home, shell);
	else
	{
		path_part = filename + 1;
		expanded_filename = ft_strjoin(home, path_part, shell);
	}
	return (expanded_filename);
}

static char	*ft_expand_filename_by_type(char *filename, t_token_type type,
									t_shell *shell)
{
	char	*expanded_filename;
	int		i;

	if (type == VARIABLE)
	{
		i = 0;
		expanded_filename = ft_extract_and_expand_var(filename, &i, shell);
	}
	else if (type == WORD && filename[0] == '~')
		expanded_filename = ft_expand_home_directory(filename, shell);
	else if (type == DOUBLE_QUOTED_STRING)
		expanded_filename = ft_expand_double_quoted(filename, shell);
	else
		expanded_filename = filename;
	return (expanded_filename);
}

char	*ft_join_redirect_filename(t_token **current, t_shell *shell)
{
	char			*filename;
	char			*expanded_filename;
	t_token_type	token_type;

	if (!*current)
		return (NULL);
	token_type = (*current)->type;
	filename = ft_join_filename_tokens(current, shell);
	if (!filename)
		return (NULL);
	expanded_filename = ft_expand_filename_by_type(filename, token_type, shell);
	return (expanded_filename);
}
