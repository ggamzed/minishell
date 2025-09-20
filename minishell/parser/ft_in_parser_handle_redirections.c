#include "../minishell.h"

// Heredoc delimiter için bitişik token'ları birleştiren fonksiyon
static char	*ft_join_heredoc_delimiter(t_token **current, t_shell *shell)
{
	char	*delimiter;
	t_token	*temp;

	if (!*current)
		return (NULL);
	
	// İlk token'ı başlangıç olarak al
	delimiter = ft_strdup((*current)->value, shell);
	if (!delimiter)
		return (NULL);
	
	// space_flag = 0 olan token'ları birleştir
	temp = *current;
	while (temp->next && temp->space_flag == 0)
	{
		temp = temp->next;
		delimiter = ft_strjoin_free(delimiter, temp->value, shell);
		if (!delimiter)
			return (NULL);
	}
	
	// current pointer'ını güncelle
	*current = temp;
	return (delimiter);
}

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
	int temp_fd;
	char *joined_delimiter;
	t_token_type first_type;

	*current = (*current)->next;
	if (*current)
	{
		// İlk token'ın tipini kaydet (expansion kontrolü için)
		first_type = (*current)->type;
		
		// Bitişik token'ları birleştir
		joined_delimiter = ft_join_heredoc_delimiter(current, shell);
		if (!joined_delimiter)
			return (0);
		
		// Her heredoc için input al ve sadece sonuncusunu tut
		temp_fd = ft_process_heredoc(joined_delimiter, shell, 
			(first_type != SINGLE_QUOTED_STRING && first_type != DOUBLE_QUOTED_STRING));

		// Önceki heredoc fd varsa kapat
		if (cmd->heredoc_fd != -1)
		{
			close(cmd->heredoc_fd);
		}
		// Yeni fd'yi ata
		cmd->heredoc_fd = temp_fd;
		cmd->heredoc_delimiter = joined_delimiter;
		cmd->heredoc_type = first_type;
		cmd->heredoc_should_expand = (first_type != SINGLE_QUOTED_STRING && first_type != DOUBLE_QUOTED_STRING);
		
		*current = (*current)->next;
	}
	return (1);
}