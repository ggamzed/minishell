#include "../minishell.h"

int	ft_builtin_echo(char **argv)
{
	int	i;
	int	newline;

	newline = 1; // varsayılan olarak newline yazdır
	i = 1;
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0) // -n parametresi kontrolü ->-n parametresi ile newline karakteri bastırılmaz
	{
		newline = 0; // newline yazdırma
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1]) // son argüman değilse boşluk ekle
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
