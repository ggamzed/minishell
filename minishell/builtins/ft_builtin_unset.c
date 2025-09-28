/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:18:57 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:18:57 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_builtin_unset(char **argv, t_env **env_list, t_shell *shell)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		if (argv[i] && argv[i][0] == '-')
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": invalid option\n", 2);
			return (2);
		}
		ft_unset_env_value(argv[i], env_list);
		if (shell && shell->export_list)
			ft_unset_env_value(argv[i], &shell->export_list);
		i++;
	}
	return (0);
}
