/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:09 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:09 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static void	ft_set_shlvl(t_env **env_list, t_shell *shell)
{
	char	*current_shlvl;

	current_shlvl = ft_get_env_value("SHLVL", *env_list);
	if (current_shlvl)
		return ;
	else
		ft_set_env_value("SHLVL", "1", env_list, shell);
}

static int	ft_ensure_pwd_exists(t_env **env_list, t_shell *shell)
{
	char	*pwd;

	if (ft_get_env_value("PWD", *env_list))
		return (1);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (0);
	ft_set_env_value("PWD", pwd, env_list, shell);
	free(pwd);
	return (1);
}

t_env	*ft_init_env(char **envp, t_shell *shell)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	if (envp)
	{
		while (envp[i])
		{
			if (!ft_parsing_env_entry(envp[i], &env_list, shell))
				return (NULL);
			i++;
		}
	}
	if (!ft_ensure_pwd_exists(&env_list, shell))
		return (env_list);
	ft_set_shlvl(&env_list, shell);
	return (env_list);
}
