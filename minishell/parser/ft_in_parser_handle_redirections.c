#include "../minishell.h"

int	ft_in_parser_handle_redirect_in(t_cmd *cmd, t_token **current)
{
	*current = (*current)->next;
	if (*current && (*current)->type == WORD)
	{
		cmd->input_file = ft_strdup((*current)->value);
		cmd->input_type = (*current)->type;
		*current = (*current)->next;
	}
	return (1);
}

int	ft_in_parser_handle_redirect_out(t_cmd *cmd, t_token **current)
{
	*current = (*current)->next;
	if (*current && (*current)->type == WORD)
	{
		cmd->output_file = ft_strdup((*current)->value);
		cmd->output_type = (*current)->type;
		cmd->append_mode = 0;
		*current = (*current)->next;
	}
	return (1);
}

int	ft_in_parser_handle_redirect_append(t_cmd *cmd, t_token **current)
{
	*current = (*current)->next;
	if (*current && (*current)->type == WORD)
	{
		cmd->output_file = ft_strdup((*current)->value);
		cmd->output_type = (*current)->type;
		cmd->append_mode = 1;
		*current = (*current)->next;
	}
	return (1);
}

int	ft_in_parser_handle_heredoc(t_cmd *cmd, t_token **current)
{
	*current = (*current)->next;
	if (*current)
	{
		cmd->heredoc_delimiter = ft_strdup((*current)->value);
		cmd->heredoc_type = (*current)->type;
		*current = (*current)->next;
	}
	return (1);
}
