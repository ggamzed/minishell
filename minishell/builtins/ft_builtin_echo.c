#include "../minishell.h"

// int	ft_is_valid_n(char *str)
// {
// 	int	i;

// 	if (!str || str[0] != '-')
// 		return (1);
// 	i = 1;
// 	while (str[i])
// 	{
// 		if (str[i] != 'n')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }


int	ft_builtin_echo(char **argv)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	//while (argv[i] && ft_is_valid_n(argv[i]) == 0) 
	while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
