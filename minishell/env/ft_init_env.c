#include "../minishell.h"

t_env	*ft_init_env(char **envp, t_shell *shell)
{
	t_env	*env_list; //-> sonuç olarak linked list dönecek
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i]) // her environment string'i için
	{
		if (!ft_parsing_env_entry(envp[i], &env_list, shell))
			return (NULL);
		i++;
	}
	return (env_list);
}
