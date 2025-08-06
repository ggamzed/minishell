
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

void print_cmd_args(t_cmd_arg *args)
{
    t_cmd_arg *current = args;
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

int main(void) //int main(int argc, char **argv, char **envp) -> token / parser test
{
	t_token *tokens;
	t_cmd   *cmd;
	
	//printf("Input: \"echo hello << a | echo hello | \\\"ece\\\"\"\n\n");
	printf("input: cat << ece > a.txt | echo ece > b.txt | cat << ece > c.txt\n\n");
	
	// Tokenize
	//tokens = ft_tokenize("echo hello << a | echo hello | \"ece\"");
	tokens = ft_tokenize("cat << ece >> a.txt | echo ece > b.txt | cat << ece > c.txt");
	if (!tokens)
	{
		printf("Tokenization failed!\n");
		return (1);
	}
	
	// Print tokens
	print_tokens(tokens);
	
	// Parse
	cmd = ft_parse_tokens(tokens);
	if (!cmd)
	{
	    printf("Parsing failed!\n");
	    return (1);
	}
	
	// Print parsed commands
	print_commands(cmd);
	
	return (0);
}