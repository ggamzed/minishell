#include "../minishell.h"

/* Heredoc input'unu kullanıcıdan alır ve FD döndürür */
int ft_process_heredoc(char *delimiter)
{
    char *line;
    int pipefd[2];
    
    // Pipe oluştur (veya geçici dosya)
    if (pipe(pipefd) == -1)
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
        // if (g_signal == SIGINT)
        // {
        //     close(pipefd[0]);
        //     close(pipefd[1]);
        //     return (-1);
        // }
		// if (!line)  // Ctrl+D
        // {
        //     write(1, "\n", 1);
        //     break;
        // }
		
        // Delimiter'a ulaştık mı?
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        
        // Pipe'a yaz -> Pipe, kernel'da 64KB'lık bir buffer'dır. Dosya değil, memory'de geçici alan!
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    
    close(pipefd[1]);  // Yazma ucunu kapat
    return (pipefd[0]); // Okuma ucunu döndür
}

/* Main'de heredoc'ları işle */
void ft_handle_heredoc(t_shell *shell)
{
    t_cmd *current = shell->cmd_list;
    
    while (current)
    {
        if (current->heredoc_delimiter)
        {
            current->heredoc_fd = ft_process_heredoc(current->heredoc_delimiter);
            if (current->heredoc_fd == -1)
            {
                // Hata durumu
                shell->exit_status = 1;
                return;
            }
        }
        current = current->next;
    }
}
