/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:54 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:54 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_cmd	*ft_create_command(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd), shell);
	if (!cmd)
		ft_cleanup_and_exit(shell, 1);
	cmd->args = NULL;
	cmd->expanded_argv = NULL;
	cmd->input_file = NULL;
	cmd->input_type = WORD;
	cmd->output_file = NULL;
	cmd->output_type = WORD;
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->heredoc_type = WORD;
	cmd->heredoc_fd = -1;
	cmd->heredoc_should_expand = 1;
	cmd->all_output_files = NULL;
	cmd->all_append_modes = NULL;
	cmd->output_count = 0;
	cmd->next = NULL;
	return (cmd);
}

static void	ft_link_arg_token(t_cmd *cmd, t_token *token)
{
	t_token	*current;

	if (!cmd->args)
	{
		cmd->args = token;
		return ;
	}
	current = cmd->args;
	while (current->next)
		current = current->next;
	current->next = token;
}

static void	ft_process_redirection(t_cmd *cmd, t_token **current,
								t_shell *shell)
{
	if ((*current)->type == REDIRECT_IN)
		ft_in_parser_handle_redirect_in(cmd, current, shell);
	else if ((*current)->type == REDIRECT_OUT)
		ft_in_parser_handle_redirect_output(cmd, current, shell, 0);
	else if ((*current)->type == REDIRECT_APPEND)
		ft_in_parser_handle_redirect_output(cmd, current, shell, 1);
	else if ((*current)->type == HEREDOC)
		ft_in_parser_handle_heredoc(cmd, current, shell);
}

t_cmd	*ft_parse_command(t_token **current, t_shell *shell)
{
	t_cmd	*cmd;
	t_token	*arg_token;

	cmd = ft_create_command(shell);
	if (!cmd)
		return (NULL);
	while (*current && (*current)->type != PIPE)
	{
		if (ft_is_argument_token((*current)->type))
		{
			arg_token = *current;
			*current = (*current)->next;
			arg_token->next = NULL;
			ft_link_arg_token(cmd, arg_token);
		}
		else if (ft_is_redirection((*current)->type))
			ft_process_redirection(cmd, current, shell);
		else
			*current = (*current)->next;
	}
	return (cmd);
}
