#include "../minishell.h"

//environment değişkenlerini siler
int	ft_builtin_unset(char **argv, t_env **env_list, t_shell *shell)
{
	int	i = 1;

	if (!argv[1])
		return (0);
	
	// Option kontrolü
	while (argv[i] && argv[i][0] == '-')
	{
		if (ft_strcmp(argv[i], "-f") == 0 || ft_strcmp(argv[i], "-v") == 0 || 
		    ft_strcmp(argv[i], "-n") == 0)
		{
			// Valid options (şimdilik skip)
			i++;
		}
		else
		{
			ft_putstr_fd("bash: unset: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": invalid option\n", 2);
			return (2); // Option error için 2
		}
	}
	
	// Normal unset işlemi
	while (argv[i])
	{
		ft_unset_env_value(argv[i], env_list);
		// Export list'ten de sil (eğer varsa)
		if (shell && shell->export_list)
			ft_unset_env_value(argv[i], &shell->export_list);
		i++;
	}
	return (0);
}