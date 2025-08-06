#include "../minishell.h"

//environment değişkenlerini siler
int	ft_builtin_unset(char **argv, t_env **env_list)
{
	int	i;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		ft_unset_env_value(argv[i], env_list);
		i++;
	}
	return (0);
}
