#include "../minishell.h"

int	ft_builtin_echo(char **argv, t_shell *shell)
{
	int	i;
	int	newline;
	t_token	*current_token;

	newline = 1;
	i = 1;
	
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	
	current_token = shell->cmd_list->args;
	
	// Echo komutu token'ını atla (argv[0] = "echo")
	if (current_token)
		current_token = current_token->next;
	
	// -n varsa onu da atla
	if (!newline && current_token)
		current_token = current_token->next;
	
	//printf("DEBUG: Starting echo with i=%d\n", i);
	
	while (argv[i])
	{
		printf("%s", argv[i]);
		
		if (current_token && current_token->space_flag == 1)
			printf(" ");
		
		if (current_token)
			current_token = current_token->next;
		i++;
	}
	
	if (newline)
		printf("\n");
	return (0);
}
