/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:18:31 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:18:31 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

static int	ft_count_args(char **argv)
{
	int	arg_count;

	arg_count = 0;
	while (argv[arg_count])
		arg_count++;
	return (arg_count);
}

static char	*ft_get_cd_path(char **argv, t_env *env_list)
{
	char	*home;
	int		arg_count;

	arg_count = ft_count_args(argv);
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (NULL);
	}
	if (!argv[1])
	{
		home = ft_get_env_value("HOME", env_list);
		if (!home || !home[0])
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (home);
	}
	return (argv[1]);
}

static int	ft_validate_path(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode) == 0)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": Not a directory\n", 2);
			return (0);
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	return (1);
}

static int	ft_perform_cd_operation(char *path, t_env **env_list,
									t_shell *shell)
{
	char	old_cwd[PATH_MAX];
	char	new_cwd[PATH_MAX];

	if (getcwd(old_cwd, sizeof(old_cwd)))
		ft_set_env_value("OLDPWD", old_cwd, env_list, shell);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	if (getcwd(new_cwd, sizeof(new_cwd)))
		ft_set_env_value("PWD", new_cwd, env_list, shell);
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: \
cannot access parent directories: \
No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int	ft_builtin_cd(char **argv, t_env *env_list, t_shell *shell)
{
	char	*path;

	path = ft_get_cd_path(argv, env_list);
	if (!path)
		return (1);
	if (!ft_validate_path(path))
		return (1);
	return (ft_perform_cd_operation(path, &env_list, shell));
}
