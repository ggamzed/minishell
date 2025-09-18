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
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_code = 2;
		} // to do: exit 123 123 bash'de dene
	}
	printf("exit\n");
	shell->exit_status = exit_code; // shell'in son komutun exit durumunu tutar = $? -> bu fonksiyon bittikten sonra maine dönülür, exit_flag bir olur, main "return (shell.exit_status);" return eder. bu yüzden exit_code saklıyoruz
	return (exit_code);
}
