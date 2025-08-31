#include "../minishell.h"
#include <fcntl.h>

/* Input redirection işlemlerini yapar (< file) */
static int	ft_handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file)
		return (0);
	fd = open(cmd->input_file, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: %s: No such file or directory\n", 
			cmd->input_file);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

/* Heredoc redirection işlemlerini yapar (<< delimiter) */
static int	ft_handle_heredoc_redirection(t_cmd *cmd)
{
	if (cmd->heredoc_fd == -1)
		return (0);
	dup2(cmd->heredoc_fd, STDIN_FILENO);
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;  // Tekrar kullanılmasını önle
	return (0);
}

/* Output redirection işlemlerini yapar (> file veya >> file) */
static int	ft_handle_output_redirection(t_cmd *cmd)
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
		printf("minishell: %s: Permission denied\n", cmd->output_file);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int		ft_handle_redirections(t_cmd *cmd)
{
	if (ft_handle_input_redirection(cmd) != 0)
		return (1);
	if (ft_handle_heredoc_redirection(cmd) != 0) // Heredoc (input'u override edebilir)
		return (1);
	if (ft_handle_output_redirection(cmd) != 0)
		return (1);
	return (0);
}
