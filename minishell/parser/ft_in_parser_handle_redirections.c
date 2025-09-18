#include "../minishell.h"

int	ft_in_parser_handle_redirect_in(t_cmd *cmd, t_token **current, t_shell *shell)
{
	*current = (*current)->next;
	if (*current && ((*current)->type == WORD ||
				(*current)->type == SINGLE_QUOTED_STRING ||
				(*current)->type == DOUBLE_QUOTED_STRING))
	{
		cmd->input_file = ft_strdup((*current)->value, shell);
		cmd->input_type = (*current)->type;
		*current = (*current)->next;
	}
	return (1);
}

int	ft_in_parser_handle_redirect_out(t_cmd *cmd, t_token **current, t_shell *shell)
{
	*current = (*current)->next;
	if (*current && ((*current)->type == WORD ||
			(*current)->type == SINGLE_QUOTED_STRING ||
			(*current)->type == DOUBLE_QUOTED_STRING))
	{
		cmd->output_file = ft_strdup((*current)->value, shell);
		cmd->output_type = (*current)->type;
		cmd->append_mode = 0;
		*current = (*current)->next;
	}
	return (1);
}

int	ft_in_parser_handle_redirect_append(t_cmd *cmd, t_token **current, t_shell *shell)
{
	*current = (*current)->next;
	if (*current && ((*current)->type == WORD ||
				(*current)->type == SINGLE_QUOTED_STRING ||
				(*current)->type == DOUBLE_QUOTED_STRING))
	{
		cmd->output_file = ft_strdup((*current)->value, shell);
		cmd->output_type = (*current)->type;
		cmd->append_mode = 1;
		*current = (*current)->next;
	}
	return (1);
}

int	ft_in_parser_handle_heredoc(t_cmd *cmd, t_token **current, t_shell *shell)
{
	*current = (*current)->next;
	if (*current)
	{
		cmd->heredoc_delimiter = ft_strdup((*current)->value, shell);
		cmd->heredoc_type = (*current)->type;
		// Quoted delimiter ise expansion yapma
        if ((*current)->type == SINGLE_QUOTED_STRING || 
            (*current)->type == DOUBLE_QUOTED_STRING)
            cmd->heredoc_should_expand = 0;
        else
            cmd->heredoc_should_expand = 1;
		*current = (*current)->next;
	}
	return (1);
}
