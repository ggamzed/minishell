/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <eecegokcece@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:18:43 by egokce            #+#    #+#             */
/*   Updated: 2025/09/25 00:18:43 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_env_value(char *key, t_env *env_list)
{
	t_env	*current;

	if (!key || !env_list)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	ft_set_env_value(char *key, char *value, t_env **env_list, t_shell *shell)
{
	t_env	*current;
	t_env	*new_node;

	if (!key || !env_list)
		return (0);
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (value)
				current->value = ft_strdup(value, shell);
			else
				current->value = NULL;
			return (0);
		}
		current = current->next;
	}
	new_node = ft_create_env_node(key, value, shell);
	ft_add_env_node(env_list, new_node);
	return (0);
}

static void	ft_remove_env_node(t_env **env_list, t_env *current, t_env *prev)
{
	if (prev)
		prev->next = current->next;
	else
		*env_list = current->next;
}

int	ft_unset_env_value(char *key, t_env **env_list)
{
	t_env	*current;
	t_env	*prev;

	if (!key || !env_list || !*env_list)
		return (0);
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			ft_remove_env_node(env_list, current, prev);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

static int	ft_count_env_vars(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		if (current->value)
			count++;
		current = current->next;
	}
	return (count);
}

static void	ft_fill_env_array(t_env *env_list, char **envp, t_shell *shell)
{
	t_env	*current;
	char	*temp;
	int		i;

	current = env_list;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			temp = ft_strjoin(current->key, "=", shell);
			envp[i] = ft_strjoin(temp, current->value, shell);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
}

char	**ft_env_to_array(t_env *env_list, t_shell *shell)
{
	char	**envp;
	int		count;

	count = ft_count_env_vars(env_list);
	envp = ft_malloc(sizeof(char *) * (count + 1), shell);
	ft_fill_env_array(env_list, envp, shell);
	return (envp);
}
