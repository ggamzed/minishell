#include "../minishell.h"

int	ft_is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_is_digit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_handle_exit_argument(char **argv, int argc, t_shell *shell)
{
	if (!ft_is_valid_number(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_flag = 0;
		return (-1);
	}
	return (ft_atoi(argv[1]));
}

int	ft_builtin_exit(char **argv, t_shell *shell, int in_pipe)
{
	int	exit_code;
	int	argc;
	int	arg_result;

	argc = 0;
	while (argv[argc])
		argc++;
	shell->exit_flag = 1;
	if (!argv[1])
		exit_code = shell->exit_status;
	else
	{
		arg_result = ft_handle_exit_argument(argv, argc, shell);
		if (arg_result == -1)
			return (1);
		exit_code = arg_result;
	}
	if (isatty(STDIN_FILENO) && !in_pipe)
		printf("exit\n");
	shell->exit_status = exit_code;
	return (exit_code);
}
