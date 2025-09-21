#include "../minishell.h"

t_env	*ft_init_env(char **envp, t_shell *shell)
{
	t_env	*env_list;
	char	*pwd;
	char	*current_shlvl;
	char	*new_shlvl;
	int		shlvl_value;
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
	
	// SHLVL'ı ayarla - minishell için her zaman 1'den başla, sadece nested minishell'de artır
	current_shlvl = ft_get_env_value("SHLVL", env_list);
	if (current_shlvl && ft_atoi(current_shlvl) > 0)
	{
		// Eğer SHLVL 0'dan büyükse ve minishell nested ise artır
		// Basit kontrol: MINISHELL_LEVEL environment variable'ı var mı?
		char *minishell_level = ft_get_env_value("MINISHELL_LEVEL", env_list);
		if (minishell_level)
		{
			shlvl_value = ft_atoi(current_shlvl) + 1;
			new_shlvl = ft_itoa(shlvl_value, shell);
			ft_set_env_value("SHLVL", new_shlvl, &env_list, shell);
		}
		else
		{
			ft_set_env_value("SHLVL", "1", &env_list, shell);
		}
	}
	else
		ft_set_env_value("SHLVL", "1", &env_list, shell);
	
	// Minishell level marker ekle
	ft_set_env_value("MINISHELL_LEVEL", "1", &env_list, shell);
	
	return (env_list);
}