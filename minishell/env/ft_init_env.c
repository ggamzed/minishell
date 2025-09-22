#include "../minishell.h"

// static void	ft_set_shlvl(t_env **env_list, t_shell *shell)
// {
// 	char	*current_shlvl;
// 	int		shlvl_value;
// 	char	*new_shlvl;

// 	current_shlvl = ft_get_env_value("SHLVL", *env_list);
// 	if (current_shlvl)
// 	{
// 		shlvl_value = ft_atoi(current_shlvl);
// 		shlvl_value++;
// 		new_shlvl = ft_itoa(shlvl_value, shell);
// 		ft_set_env_value("SHLVL", new_shlvl, env_list, shell);
// 		free(new_shlvl);
// 	}
// 	else
// 		ft_set_env_value("SHLVL", "1", env_list, shell);
// }

static void	ft_set_shlvl(t_env **env_list, t_shell *shell)
{
	char	*current_shlvl;

	current_shlvl = ft_get_env_value("SHLVL", *env_list);
	if (current_shlvl)
	{
		// SHLVL mevcut - değiştirme, olduğu gibi bırak
		// Bash zaten doğru değeri vermiş
		return;
	}
	else
	{
		// SHLVL yok - 1 yap (env -i durumu)
		ft_set_env_value("SHLVL", "1", env_list, shell);
	}
}


t_env	*ft_init_env(char **envp, t_shell *shell)
{
	t_env	*env_list;
	char	*pwd;
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
	
	// PWD ayarla
	if (!ft_get_env_value("PWD", env_list))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			ft_set_env_value("PWD", pwd, &env_list, shell);
			free(pwd);
		}
	}
	
	// SHLVL'ı doğru şekilde ayarla
	ft_set_shlvl(&env_list, shell);
	
	return (env_list);
}

