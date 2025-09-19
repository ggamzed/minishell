#include "../minishell.h"

/* Heredoc input'unu kullanıcıdan alır ve FD döndürür */
int ft_process_heredoc(char *delimiter, t_shell *shell, int should_expand)
{
	char *line;
	char *expanded_line;
	int pipefd[2];
	
	// Pipe oluştur (veya geçici dosya)
	// pipefd[0] → okuma ucu
	// pipefd[1] → yazma ucu
	if (pipe(pipefd) == -1) // Pipe, kernel’de memory’de geçici bir buffer oluşturur.
	{
		perror("minishell: pipe");
		return (-1);
	}

	// Kullanıcıdan satır satır input al
	while (1)
	{
		
		line = readline("> ");  // Heredoc prompt
		if (!line)  // EOF (Ctrl+D)
			break;
		
		// // Ctrl+C handle
		if (g_signal == SIGINT)
		{
		    close(pipefd[0]);
		    close(pipefd[1]);
		    return (-1);
		}
		if (!line)  // Ctrl+D
		{
		    write(1, "\n", 1);
		    break;
		}
		
		// Delimiter'a ulaştık mı? örn: EOF
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		// Variable expansion kontrolü
		if (should_expand)
		{
			expanded_line = ft_expand_double_quoted(line, shell);
			if (!expanded_line)
			{
				free(line);
				close(pipefd[1]);
				close(pipefd[0]);
				return (-1);
			}	
		}  
		else
		{
			expanded_line = ft_strdup(line, shell);
			if (!expanded_line)
			{
				free(line);
				close(pipefd[1]);
				close(pipefd[0]);
				return (-1);
			}
		}
		// Pipe'a yaz -> Pipe, kernel'da 64KB'lık bir buffer'dır. Dosya değil, memory'de geçici alan!
		write(pipefd[1], expanded_line, ft_strlen(expanded_line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	
	close(pipefd[1]);  // Yazma ucunu kapat
	return (pipefd[0]); // Okuma ucunu döndür
}

/* Main'de heredoc'ları işle */
// int	ft_handle_heredoc(t_shell *shell)
// {
//     t_cmd *current;
//     t_cmd *cleanup_cmd;
	
// 	current = shell->cmd_list;
//     while (current)
//     {
//         if (current->heredoc_delimiter)
//         {
//             current->heredoc_fd = ft_process_heredoc(current->heredoc_delimiter, shell, current->heredoc_should_expand);
// 			if (current->heredoc_fd == -1)
//             {
//                 // Hata durumunda önceki tüm heredoc fd'lerini kapat
//                 cleanup_cmd = shell->cmd_list;
//                 while (cleanup_cmd != current)
//                 {
//                     if (cleanup_cmd->heredoc_fd != -1)
//                     {
//                         close(cleanup_cmd->heredoc_fd);
//                         cleanup_cmd->heredoc_fd = -1;
//                     }
//                     cleanup_cmd = cleanup_cmd->next;
//                 }
//                 shell->exit_status = 1;
//                 return (0);
//             }
//         }
//         current = current->next;
//     }
// 	return (1);
// }

int	ft_handle_heredoc(t_shell *shell)
{
    t_cmd *current;
    
    current = shell->cmd_list;
    while (current)
    {
        // SADECE parse sırasında işlenmemiş heredoc'ları işle
        if (current->heredoc_delimiter && current->heredoc_fd == -1)
        {
            current->heredoc_fd = ft_process_heredoc(current->heredoc_delimiter, shell, current->heredoc_should_expand);
            if (current->heredoc_fd == -1)
            {
                shell->exit_status = 1;
                return (0);
            }
        }
        current = current->next;
    }
    return (1);
}

