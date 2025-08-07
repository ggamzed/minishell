#include "../minishell.h"

t_cmd	*ft_create_command(void)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd));
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->input_type = WORD;
	cmd->output_file = NULL;
	cmd->output_type = WORD;
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->heredoc_type = WORD;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

t_token	*ft_create_cmd_arg(char *value, t_token_type type)
{
	t_token	*arg;

	arg = ft_malloc(sizeof(t_token));
	arg->value = ft_strdup(value);
	arg->type = type;
	arg->next = NULL;
	return (arg);
}

void	ft_add_cmd_arg(t_token **args, t_token *new_arg)
{
	t_token	*current;

	if (!*args)
	{
		*args = new_arg;
		return;
	}
	current = *args;
	while (current->next)
		current = current->next;
	current->next = new_arg;
}

t_cmd	*ft_parse_command(t_token **current)
{
	t_cmd		*cmd;
	t_token	*arg;

	cmd = ft_create_command();
	while (*current && (*current)->type != PIPE) // Tüm token'ları tek döngüde işle
	{
		if (ft_is_argument_token((*current)->type)) // Normal argüman
		{
			arg = ft_create_cmd_arg((*current)->value, (*current)->type);
			ft_add_cmd_arg(&cmd->args, arg);
			*current = (*current)->next;
		}
		else if ((*current)->type == REDIRECT_IN)
			ft_handle_redirect_in(cmd, current);
		else if ((*current)->type == REDIRECT_OUT)
			ft_handle_redirect_out(cmd, current);
		else if ((*current)->type == REDIRECT_APPEND)
			ft_handle_redirect_append(cmd, current);
		else if ((*current)->type == HEREDOC)
			ft_handle_heredoc(cmd, current);
		else //gerek var mı?
			*current = (*current)->next;
	}
	
	return (cmd);
}
