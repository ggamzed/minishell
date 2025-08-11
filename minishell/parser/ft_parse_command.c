#include "../minishell.h"

t_cmd	*ft_create_command(void)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd));
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

static void	ft_process_redirection(t_cmd *cmd, t_token **current)
{
	// Multiple redirects: son redirect geçerli olur
	if ((*current)->type == REDIRECT_IN)
	{
		// Eğer heredoc varsa, input redirection'ı ignore et
		if (cmd->heredoc_delimiter)
		{
			*current = (*current)->next; // Skip the filename
			if (*current && (*current)->type == WORD)
				*current = (*current)->next; // Skip the filename
			return;
		}
		
		// Önceki input file'ı temizle
		if (cmd->input_file)
		{
			free(cmd->input_file);
			cmd->input_file = NULL;
		}
		ft_handle_redirect_in(cmd, current);
	}
	else if ((*current)->type == REDIRECT_OUT)
	{
		// Önceki output file'ı temizle (hem > hem >> için)
		if (cmd->output_file)
		{
			free(cmd->output_file);
			cmd->output_file = NULL;
			cmd->append_mode = 0; // Reset append mode
		}
		ft_handle_redirect_out(cmd, current);
	}
	else if ((*current)->type == REDIRECT_APPEND)
	{
		// Önceki output file'ı temizle (hem > hem >> için)
		if (cmd->output_file)
		{
			free(cmd->output_file);
			cmd->output_file = NULL;
			cmd->append_mode = 0; // Reset append mode
		}
		ft_handle_redirect_append(cmd, current);
	}
	else if ((*current)->type == HEREDOC)
	{
		// Heredoc varsa input file'ı temizle (heredoc öncelikli)
		if (cmd->input_file)
		{
			free(cmd->input_file);
			cmd->input_file = NULL;
		}
		
		// Önceki heredoc'u temizle
		if (cmd->heredoc_delimiter)
		{
			free(cmd->heredoc_delimiter);
			cmd->heredoc_delimiter = NULL;
		}
		if (cmd->heredoc_fd != -1)
		{
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1;
		}
		ft_handle_heredoc(cmd, current);
	}
}

// token listesinden tek bir komut parse eder, redirection token'larını ayırır, argüman token'larını cmd->args'a bağlar
t_cmd	*ft_parse_command(t_token **current)
{
	t_cmd	*cmd;
	t_token	*arg_token;

	cmd = ft_create_command();
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
			ft_process_redirection(cmd, current);
	}
	
	// Command validation: en az bir argüman veya redirect olmalı
	if (!cmd->args && !cmd->input_file && !cmd->output_file && !cmd->heredoc_delimiter)
	{
		// Empty command - this is valid in some cases
		// For example: < input.txt > output.txt (command with only redirects)
	}
	
	return (cmd);
}
