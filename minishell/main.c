#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

static t_shell	*ft_init_shell(char **envp, t_mem **mem_tracker)
{
	t_shell	*shell;
	//t_mem *tmp;

	shell = malloc(sizeof(t_shell)); // burada ft_malloc kullan kendi yazdığın ft_mallocun adını değiştir diğerlerinde onu kullan
	if (!shell)
		return (NULL);
	shell->mem_tracker = mem_tracker;
	shell->env_list = ft_init_env(envp, shell);
	if (!shell->env_list)
	{
		free(shell);
		return (NULL);
	}
	shell->cmd_list = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->exit_flag = 0;
	return (shell);
}

static int	ft_process_line(t_shell *shell, char *line)
{
	t_token	*tokens;

	shell->line = line;
	
	// Input validation
	if (!ft_validate_syntax(line))
	{
		shell->exit_status = 2;
		printf("minishell: syntax error\n");
		return (1);
	}
	
	// Tokenization
	tokens = ft_tokenize(line, shell);
	if (!tokens)
		return (0);
		
	// Parsing - token'ları command'lara çevir
	shell->cmd_list = ft_parse_tokens(tokens, shell);
	if (!shell->cmd_list)
		return (0);

	// Heredoc işleme
	if (ft_handle_heredoc(shell) == 0)
	{
		ft_free_commands(shell->cmd_list);
		shell->cmd_list = NULL;
		return (0);
	}
	
	// Expansion - tüm komutların argv'lerini hazırla (SADECE BURADA YAP)
	t_cmd *current = shell->cmd_list;
	while (current)
	{
		if (current->args) // Null check ekle
		{
			current->expanded_argv = ft_expand_tokens(current->args, shell);
			if (!current->expanded_argv || !current->expanded_argv[0])
			{
				shell->exit_status = 1;
				ft_free_commands(shell->cmd_list);
				shell->cmd_list = NULL;
				return (1);
			}
		}
		current = current->next;
	}
	
	// Execution - komutları çalıştır
	shell->exit_status = ft_execute_commands(shell);
	if (shell->exit_status == -42) // -42 idi burası
	{
		ft_free_commands(shell->cmd_list);
		shell->cmd_list = NULL;
		return (0);
	}
	
	// Cleanup - commands'ı free et
	ft_free_commands(shell->cmd_list);
	shell->cmd_list = NULL;
	
	// tokens'ı da free et (memory leak'i önlemek için)
	// Ama dikkat: tokens zaten cmd'lerin içinde referans ediliyorsa
	// cmd_list free edildiğinde tokens da free edilmiş olur
	
	return (1);
}

static void	ft_shell_loop(t_shell *shell)
{
	char	*line;
	
	while (!shell->exit_flag)
	{
		ft_setup_signals();
		line = readline(PROMPT);
		if (!line)  // EOF (Ctrl+D) CTRL+D = NULL döner
		{
			printf("exit\n");
			shell->exit_flag = 1;
			break;
		}
		if (*line)  // Non-empty line
		{
			add_history(line);
			if (ft_process_line(shell, line) == 0)
			{
				free(line);
				break ;
			}	
		}
		free(line);
		if (g_signal == SIGINT)
		{
			shell->exit_status = 130;
			g_signal = 0;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_mem	*mem_tracker;
	int		exit_code;

	(void)argc;
	(void)argv;
	
	// Shell initialization
	mem_tracker = NULL;
	shell = ft_init_shell(envp, &mem_tracker);
	//printf("mem_tracker: %p\n", (void*)&mem_tracker);
	//printf("shell->mem_tracker: %p\n", (void*)shell->mem_tracker);
	if (!shell)
	{
		fprintf(stderr, "minishell: failed to initialize shell\n");
		ft_free_mem_tracker(&mem_tracker);
		free(shell);
		return (1);
	}
	
	// Main shell loop
	ft_shell_loop(shell);
	
	// Cleanup and exit
	exit_code = shell->exit_status;
	ft_free_mem_tracker(&mem_tracker);
	free(shell);
	//ft_free_shell(shell);
	//rl_clear_history();
	
	return (exit_code);
}
