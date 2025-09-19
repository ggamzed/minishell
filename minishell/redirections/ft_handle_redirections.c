#include "../minishell.h"
#include <fcntl.h>

/* Input redirection işlemlerini yapar (< file) */
int	ft_handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	fd = open(cmd->input_file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->input_file, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

/* Heredoc redirection işlemlerini yapar (<< delimiter) */
// static int	ft_handle_heredoc_redirection(t_cmd *cmd) --version 1
// {
// 	if (cmd->heredoc_fd == -1)
// 		return (0);
// 	dup2(cmd->heredoc_fd, STDIN_FILENO);
// 	close(cmd->heredoc_fd);
// 	cmd->heredoc_fd = -1;  // Tekrar kullanılmasını önle
// 	return (0);
// }

// int	ft_handle_heredoc_redirection(t_cmd *cmd) --version 2
// {
// 	t_heredoc *current_hd;
// 	t_heredoc *active_hd = NULL;
	
// 	// Son heredoc'u bul (bash mantığı: son geçerli olur)
// 	current_hd = cmd->heredocs;
// 	while (current_hd)
// 	{
// 		if (current_hd->fd != -1)
// 			active_hd = current_hd;
// 		current_hd = current_hd->next;
// 	}
	
// 	if (active_hd && active_hd->fd != -1)
// 	{
// 		dup2(active_hd->fd, STDIN_FILENO);
		
// 		// Tüm heredoc fd'lerini kapat
// 		current_hd = cmd->heredocs;
// 		while (current_hd)
// 		{
// 			if (current_hd->fd != -1)
// 			{
// 				close(current_hd->fd);
// 				current_hd->fd = -1;
// 			}
// 			current_hd = current_hd->next;
// 		}
// 	}
// 	return (0);
// }

int	ft_handle_heredoc_redirection(t_cmd *cmd)
{
	if (cmd->heredoc_fd == -1)
		return (0);
	dup2(cmd->heredoc_fd, STDIN_FILENO);
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
	return (0);
}

/* Output redirection işlemlerini yapar (> file veya >> file) */
int	ft_handle_output_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->output_file)
		return (0);


	if (cmd->append_mode)
		fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644); // APPEND: Dosyanın sonuna ekle
	else
		fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // TRUNC: Dosyayı sıfırla, yaz
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->output_file, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

// int	ft_handle_heredoc(t_shell *shell)
// {
//     t_cmd *current;
//     int cmd_index = 0;
    
//     printf("DEBUG: ft_handle_heredoc - başlıyor\n");
    
//     current = shell->cmd_list;
//     while (current)
//     {
//         printf("DEBUG: Komut %d - heredoc_delimiter: '%s', heredoc_fd: %d\n", 
//                cmd_index, 
//                current->heredoc_delimiter ? current->heredoc_delimiter : "NULL",
//                current->heredoc_fd);
               
//         if (current->heredoc_delimiter && current->heredoc_fd == -1)
//         {
//             printf("DEBUG: Komut %d için heredoc işleniyor\n", cmd_index);
//             current->heredoc_fd = ft_process_heredoc(current->heredoc_delimiter, shell, current->heredoc_should_expand);
//             if (current->heredoc_fd == -1)
//             {
//                 shell->exit_status = 1;
//                 return (0);
//             }
//         }
//         current = current->next;
//         cmd_index++;
//     }
    
//     printf("DEBUG: ft_handle_heredoc - bitiyor\n");
//     return (1);
// }
