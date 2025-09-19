#include "../minishell.h"

int	ft_is_valid_n(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (1);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}


int	ft_builtin_echo(char **argv)
{
	int	i;
	int	newline;

	newline = 1; // varsayılan olarak newline yazdır
	i = 1;
	while (argv[i] && argv[i][0] == '\0')
		i++;
	while (argv[i] && ft_is_valid_n(argv[i]) == 0) // -n parametresi kontrolü ->-n parametresi ile newline karakteri bastırılmaz
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
