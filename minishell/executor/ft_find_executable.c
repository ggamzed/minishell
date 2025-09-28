/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_executable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:55 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:19:55 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <sys/stat.h>

static char	*ft_check_direct_path(char *cmd, t_shell *shell)
{
	struct stat	st;

	if (!ft_strchr(cmd, '/'))
		return (cmd);
	if (access(cmd, F_OK) != 0)
		return (NULL);
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		return (NULL);
	if (access(cmd, X_OK) != 0)
		return (NULL);
	return (ft_strdup(cmd, shell));
}

static char	*ft_search_in_paths(char *cmd, char **paths, t_shell *shell)
{
	char		*temp;
	char		*full_path;
	struct stat	st;
	int			i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/", shell);
		full_path = ft_strjoin(temp, cmd, shell);
		if (access(full_path, F_OK) == 0)
		{
			if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
			{
				i++;
				continue ;
			}
			if (access(full_path, X_OK) == 0)
				return (full_path);
		}
		i++;
	}
	return (NULL);
}

char	*ft_find_executable(char *cmd, t_env *env_list, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (ft_strcmp(cmd, "..") == 0)
		return (NULL);
	result = ft_check_direct_path(cmd, shell);
	if (result != cmd)
		return (result);
	path_env = ft_get_env_value("PATH", env_list);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':', shell);
	if (!paths)
		return (NULL);
	result = ft_search_in_paths(cmd, paths, shell);
	return (result);
}
