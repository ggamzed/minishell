#include "../minishell.h"

t_cmd	*ft_create_command(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd), shell);
	if (!cmd)
		return (NULL);
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
	cmd->next = NULL;
	return (cmd);
}

void	ft_link_arg_token(t_cmd *cmd, t_token *token) // t_token olarak gelen listeyi t_cmd içindeki t_token args'ın içine aktarıyoruz
{
	t_token	*current;

	if (!cmd->args)
	{
		cmd->args = token;
		return;
	}
	current = cmd->args;
	while (current->next)
		current = current->next;
	current->next = token;
}

static void	ft_process_redirection(t_cmd *cmd, t_token **current, t_shell *shell)
{
	if ((*current)->type == REDIRECT_IN)
		ft_in_parser_handle_redirect_in(cmd, current, shell);
	else if ((*current)->type == REDIRECT_OUT)
		ft_in_parser_handle_redirect_out(cmd, current, shell);
	else if ((*current)->type == REDIRECT_APPEND)
		ft_in_parser_handle_redirect_append(cmd, current, shell);
	else if ((*current)->type == HEREDOC)
		ft_in_parser_handle_heredoc(cmd, current, shell);
}

// token listesinden tek bir komut parse eder, redirection token'larını ayırır, argüman token'larını cmd->args'a bağlar
t_cmd	*ft_parse_command(t_token **current, t_shell *shell)
{
	t_cmd	*cmd;
	t_token	*arg_token;

	cmd = ft_create_command(shell);
	if (!cmd)
		return (NULL);
	while (*current && (*current)->type != PIPE)
	{
		if (ft_is_argument_token((*current)->type)) // Mevcut token'ı argüman listesine taşı
		{	
			arg_token = *current;
			*current = (*current)->next; // bir sonraki token'a geç
			arg_token->next = NULL; // bu token'ı args listesine ekle (link kır)
			ft_link_arg_token(cmd, arg_token);
		}
		else
			ft_process_redirection(cmd, current, shell);
	}
	return (cmd);
}
