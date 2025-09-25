#include "../minishell.h"

static int	ft_is_valid_export_var(char *str)
{
	int	i;

	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_handle_export_option(char *option)
{
	if (ft_strcmp(option, "-p") == 0)
		return (0);
	ft_putstr_fd("bash: export: ", 2);
	ft_putstr_fd(option, 2);
	ft_putstr_fd(": invalid option\n", 2);
	if (option[0] == '-' && option[1] == '-')
		ft_putstr_fd("export: usage: export [-fn] [name[=value] ...] or export -p\n", 2);
	return (2);
}

static void	ft_print_export_list(t_env *export_list)
{
	t_env	*curr;

	curr = export_list;
	while (curr)
	{
		if (curr->value)
			printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		else
			printf("declare -x %s\n", curr->key);
		curr = curr->next;
	}
}

int	ft_builtin_export(char **argv, t_env **env_list, t_shell *shell)
{
	int	i;
	int	exit_code;

	if (!argv[1])
	{
		ft_print_export_list(shell->export_list);
		return (0);
	}
	i = 1;
	exit_code = 0;
	while (argv[i])
	{
		if (argv[i][0] == '-' && ft_handle_export_option(argv[i]) != 0)
			return (2);
		else if (ft_is_valid_export_var(argv[i]) == 0)
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			exit_code = 1;
		}
		else
			ft_set_export_variable(argv[i], env_list, shell);
		i++;
	}
	return (exit_code);
}
