
#include "minishell.h"

char *get_token_name(t_token_type type)
{
    if (type == WORD) return "WORD";
    if (type == PIPE) return "PIPE";
    if (type == REDIRECT_APPEND) return "REDIRECT_APPEND";
    if (type == REDIRECT_OUT) return "REDIRECT_OUT";
    if (type == REDIRECT_IN) return "REDIRECT_IN";
    if (type == HEREDOC) return "HEREDOC";
    if (type == VARIABLE) return "VARIABLE";
    if (type == EXIT_STATUS) return "EXIT_STATUS";
    if (type == SINGLE_QUOTED_STRING) return "SINGLE_QUOTED";
    if (type == DOUBLE_QUOTED_STRING) return "DOUBLE_QUOTED";
    return "UNKNOWN";
}

void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    int i = 0;
    
    printf("=== TOKENS ===\n");
    while (current)
    {
        printf("[%d] %s", i++, get_token_name(current->type));
        if (current->value)
            printf(": %s", current->value);
        printf("\n");
        current = current->next;
    }
    printf("Total tokens: %d\n\n", i);
}

void print_cmd_args(t_token *args)
{
    t_token *current = args;
    int i = 0;
    
    printf("    Arguments:\n");
    while (current)
    {
        printf("      [%d] %s: \"%s\"\n", i++, 
               get_token_name(current->type), current->value);
        current = current->next;
    }
    if (i == 0)
        printf("      (no arguments)\n");
}

void print_commands(t_cmd *commands)
{
    t_cmd *current = commands;
    int cmd_num = 0;
    
    printf("=== PARSED COMMANDS ===\n");
    while (current)
    {
        printf("Command %d:\n", cmd_num++);
        
        // Print arguments
        print_cmd_args(current->args);
        
        // Print redirections
        if (current->input_file)
        {
            printf("    Input: \"%s\" (type: %s)\n", 
                   current->input_file, get_token_name(current->input_type));
        }
        
        if (current->output_file)
        {
            printf("    Output: \"%s\" (type: %s) %s\n", 
                   current->output_file, get_token_name(current->output_type),
                   current->append_mode ? "(append)" : "(overwrite)");
        }
        
        if (current->heredoc_delimiter)
        {
            printf("    Heredoc delimiter: \"%s\" (type: %s)\n", 
                   current->heredoc_delimiter, get_token_name(current->heredoc_type));
        }
        
        printf("\n");
        current = current->next;
    }
}

void print_all_env(t_env *env_list)
{
	t_env *current = env_list;
	
	while (current)
	{
		// Value NULL değilse yazdır (boş string olsa bile)
		if (current->value != NULL)
		{
			printf("%s=%s\n", current->key, current->value);
		}
		current = current->next;
	}
}

int main(int argc, char **argv, char **envp) //int main(int argc, char **argv, char **envp) -> token / parser / environment tester
{
	t_token *tokens;
	t_cmd   *cmd;
	t_env *env_list;;
	
	(void)argc;
    (void)argv;

	//printf("Input: \"echo hello << a | echo hello | \\\"ece\\\"\"\n\n");
	printf("input: cat << ece > a.txt | echo ece > b.txt | cat << ece > c.txt\n\n");

	//tokens = ft_tokenize("echo hello << a | echo hello | \"ece\"");
	tokens = ft_tokenize("cat << ece >> a.txt | echo ece > b.txt | cat << ece > c.txt");
	if (!tokens)
	{
		printf("Tokenization failed!\n");
		return (1);
	}
	print_tokens(tokens);
	


	cmd = ft_parse_tokens(tokens);
	if (!cmd)
	{
	    printf("Parsing failed!\n");
	    return (1);
	}
	print_commands(cmd);

	
    env_list = init_env(envp);
	if (!env_list)
	{
		printf("Environment failed!\n");
		return (1);
	}
	print_all_env(env_list);
	
	return (0);
}