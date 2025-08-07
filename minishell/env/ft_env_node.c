#include "../minishell.h"

t_env	*ft_create_env_node(char *key, char *value)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env));
	node->key = ft_strdup(key);
	if (value)
    	node->value = ft_strdup(value);
	else
    	node->value = NULL;
	node->next = NULL;
	return (node);
}

void	ft_add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
