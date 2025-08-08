#include "../minishell.h"
#include <fcntl.h>

int	setup_redirections(t_cmd *cmd)
{
	int	fd;

	// Input redirection: < file
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			printf("minishell: %s: No such file or directory\n", cmd->input_file);
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	
	// Heredoc: << delimiter (main'de önceden işlenmiş)
	if (cmd->heredoc_fd != -1)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	
	// Output redirection: > file veya >> file
	if (cmd->output_file)
	{
		if (cmd->append_mode)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		
		if (fd == -1)
		{
			printf("minishell: %s: Permission denied\n", cmd->output_file);
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (0);
}