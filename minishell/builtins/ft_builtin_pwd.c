#include "../minishell.h"

int	ft_builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	printf("minishell: pwd"); // -> hata durumunda
	return (1);
}
