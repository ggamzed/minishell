#include "../minishell.h"

int ft_is_valid_number(char *str)
{
	int i = 0;
	
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i]) // Sadece + veya - varsa
		return (0);
	while (str[i])
	{
		if (!ft_is_digit(str[i]))  // ft_isdigit yerine ft_is_digit
			return (0);
		i++;
	}
	return (1);
}
int	ft_builtin_exit(char **argv, t_shell *shell, int in_pipe)
{
	int	exit_code;
	int	argc = 0;
	
	// Argüman sayısını say
	while (argv[argc])
		argc++;
	
	shell->exit_flag = 1;
	
	if (!argv[1])
		exit_code = shell->exit_status;
	else
	{
		// ÖNCE numeric kontrolü yap
		if (!ft_is_valid_number(argv[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_code = 2;
			// Shell kapanır (exit_flag = 1 zaten)
		}
		else if (argc > 2) // SONRA argüman sayısını kontrol et
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			shell->exit_flag = 0; // Exit'i iptal et
			return (1);
		}
		else
		{
			exit_code = ft_atoi(argv[1]);
		}
	}
	
	if (isatty(STDIN_FILENO) && !in_pipe)  // Interactive mode kontrolü
		printf("exit\n");
	
	shell->exit_status = exit_code;
	return (exit_code);
}