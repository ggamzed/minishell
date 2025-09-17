#include "../minishell.h"

int	ft_builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code; // bu fonksiyonda hesaplanan çıkış kodu, amaç: shell'den çıkış kodu belirlemek

	shell->exit_flag = 1; // shell'in main loop'unun durmasını sağlayan bayrak
	if (!argv[1])
		exit_code = shell->exit_status; // argüman yoksa mevcut exit status'u kullan
	else
	{
		exit_code = ft_atoi(argv[1]);
		if (!ft_is_digit(argv[1][0]) && argv[1][0] != '-' && argv[1][0] != '+')
		{
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			exit_code = 255;
		} // to do: exit 123 123 bash'de dene
	}
	printf("exit\n");
	shell->exit_status = exit_code; // shell'in son komutun exit durumunu tutar = $? -> bu fonksiyon bittikten sonra maine dönülür, exit_flag bir olur, main "return (shell.exit_status);" return eder. bu yüzden exit_code saklıyoruz
	return (exit_code);
}
