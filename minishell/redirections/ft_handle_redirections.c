#include "../minishell.h"
#include <fcntl.h>
#include <sys/stat.h>

/* Input redirection işlemlerini yapar (< file) */
// int	ft_handle_input_redirection(t_cmd *cmd)
// {
// 	int	fd;

// 	if (!cmd->input_file)
// 		return (0);
// 	printf("TEST POINT 3\n");
// 	fd = open(cmd->input_file, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(cmd->input_file, 2);
// 		ft_putstr_fd(": ", 2);
// 		perror("");
// 		return (1);
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	return (0);
// }

int	ft_handle_input_redirection(t_cmd *cmd)
{
	int	fd;
	struct stat st;

	if (!cmd->input_file)
		return (0);
	
	// Önce dosyanın directory olup olmadığını kontrol et
	if (stat(cmd->input_file, &st) == 0)
	{
		if ((st.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_print_error_msg(cmd->input_file, ": Is a directory\n");
			return (1);
		}
	}
	
	fd = open(cmd->input_file, O_RDONLY);
	if (fd == -1)
	{
		ft_print_error_msg(cmd->input_file, ": ");
		perror("");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}


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
int ft_handle_output_redirection(t_cmd *cmd)
{
    int fd;
    int final_fd = -1;
    
		//printf("TEST POINT 1\n");
    // Önce tüm dosyaları oluştur (bash davranışı)
    for (int i = 0; i < cmd->output_count; i++)
    {
        if (cmd->all_append_modes[i])
            fd = open(cmd->all_output_files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->all_output_files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->all_output_files[i], 2);
            
            if (errno == EISDIR)
                ft_putstr_fd(": is a directory\n", 2);
            else
            {
                ft_putstr_fd(": ", 2);
                perror("");
            }
            return (1);
        }
        
        // Son dosya hariç diğerlerini kapat
        if (i == cmd->output_count - 1)
            final_fd = fd;
        else
            close(fd);
    }
    
    // Sadece son dosyaya output yönlendir
    if (final_fd != -1)
    {
        dup2(final_fd, STDOUT_FILENO);
        close(final_fd);
    }
    
    // Fallback: eski davranış (tek output file varsa)
    if (cmd->output_count == 0 && cmd->output_file)
    {
		//printf("TEST POINT 5\n");

        if (cmd->append_mode)
            fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->output_file, 2);
            
            if (errno == EISDIR)
                ft_putstr_fd(": is a directory\n", 2);
            else
            {
                ft_putstr_fd(": ", 2);
                perror("");
            }
            return (1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    
    return (0);
}