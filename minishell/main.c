#include "minishell.h"

static t_shell	*ft_init_shell(char **envp)
{
	t_shell	*shell;

	shell = ft_malloc(sizeof(t_shell));
	shell->env_list = ft_init_env(envp);
	shell->cmd_list = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->exit_flag = 0;
	return (shell);
}

static void	ft_process_line(t_shell *shell, char *line)
{
	t_token	*tokens;

	shell->line = line;
	
	// Input validation
	if (!ft_validate_syntax(line))
	{
		shell->exit_status = 2;  // Syntax error exit code
		printf("minishell: syntax error\n");
		return;
	}
	
	// Tokenization
	tokens = ft_tokenize(line);
	if (!tokens)
		return;
	
	// Parsing - token'ları command'lara çevir
	shell->cmd_list = ft_parse_tokens(tokens);
	//ft_free_tokens(tokens);
	
	if (!shell->cmd_list)
		return;
	
	 ft_handle_heredoc(shell);
	
	// Expansion - tüm komutların argv'lerini hazırla
	// t_cmd *current = shell->cmd_list;
	// while (current)
	// {
	// 	current->expanded_argv = ft_expand_tokens(current->args, shell);
	// 	if (!current->expanded_argv || !current->expanded_argv[0])
	// 	{
	// 		shell->exit_status = 1;
	// 		ft_free_commands(shell->cmd_list);
	// 		shell->cmd_list = NULL;
	// 		return;
	// 	}
	// 	current = current->next;
	// }
	
	// Execution - komutları çalıştır
	shell->exit_status = ft_execute_commands(shell);
	
	// Cleanup
	//ft_free_commands(shell->cmd_list);
	shell->cmd_list = NULL;
}

static void	ft_shell_loop(t_shell *shell)
{
	char	*line;
	
	while (!shell->exit_flag)
	{
		// setup_signals();  // TODO: Implement signals
		
		line = readline(PROMPT);
		if (!line)  // EOF (Ctrl+D)
		{
			printf("exit\n");
			shell->exit_flag = 1;
			break;
		}
		
		if (*line)  // Non-empty line
		{
			add_history(line);
			ft_process_line(shell, line);
		}
		
		free(line);
		
		// TODO: Signal handling
		// if (g_signal == SIGINT)
		// {
		//     shell->exit_status = 130;
		//     g_signal = 0;
		// }
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_code;

	(void)argc;
	(void)argv;
	
	// Shell initialization
	shell = ft_init_shell(envp);
	if (!shell)
	{
		fprintf(stderr, "minishell: failed to initialize shell\n");
		return (1);
	}
	
	// Main shell loop
	ft_shell_loop(shell);
	
	// Cleanup and exit
	exit_code = shell->exit_status;
	ft_free_shell(shell);
	rl_clear_history();
	
	return (exit_code);
}