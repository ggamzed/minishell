#include "../minishell.h"

t_env	*ft_init_env(char **envp, t_shell *shell)
{
	t_env	*env_list; //-> sonuç olarak linked list dönecek
	char	*pwd;
	int		i;

	env_list = NULL;
	i = 0;
	if (envp)
	{
		while (envp[i]) // her environment string'i için
		{
			if (!ft_parsing_env_entry(envp[i], &env_list, shell))
				return (NULL);
			i++;
		}
	}
	// env yoksa min gerekli olanları ekle
	if (!ft_get_env_value("PWD", env_list))
    {
        pwd = getcwd(NULL, 0);
        if (pwd)
        {
            ft_set_env_value("PWD", pwd, &env_list, shell);
            free(pwd);
        }
    }
    
    if (!ft_get_env_value("SHLVL", env_list))
    {
        ft_set_env_value("SHLVL", "1", &env_list, shell);
    }
	return (env_list);
}
