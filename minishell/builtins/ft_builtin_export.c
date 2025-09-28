/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:18:52 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 20:45:31 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

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
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(option, 2);
	ft_putstr_fd(": invalid option\n", 2);
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
		if (argv[1] && argv[1][0] == '-')
			return (ft_handle_export_option(argv[i]));
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
