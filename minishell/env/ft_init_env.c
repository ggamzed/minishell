#include "../minishell.h"

t_env	*ft_init_env(char **envp)
{
	t_env	*env_list; //-> sonuç olarak linked list dönecek
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i]) // her environment string'i için
	{
		ft_parsing_env_entry(envp[i], &env_list);
		i++;
	}
	return (env_list);
}
