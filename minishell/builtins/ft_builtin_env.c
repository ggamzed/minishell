/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:18:39 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:18:39 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

int	ft_builtin_env(char **argv, t_env *env_list)
{
	t_env	*current;

	if (argv[1])
	{
		if (argv[1][0] == '-')
		{
			ft_putstr_fd("env: invalid option\n", 2);
			return (125);
		}
		else
		{
			ft_putstr_fd("env: No such file or directory", 2);
			return (127);
		}
	}
	current = env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
