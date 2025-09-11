#include "../minishell.h"

static int	ft_count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// exit status için
// 0 -> başarı, 1 -> hata, 2 -> signal ile sonlanma
// ana execute fonksiyonu, pipe sayısına göre çalıştırır
int	ft_execute_commands(t_shell *shell)
{
	if (!shell->cmd_list || !shell->cmd_list->args)
		return (0);

	// EXPANDED_ARGV'Yİ BURADA EXPAND ETME!
	// Zaten ft_process_line içinde expand edilmiş
	
	if (ft_count_commands(shell->cmd_list) == 1)
		return (ft_execute_single_command(shell, shell->cmd_list));
	else
		return (ft_execute_multiple_command(shell));
}

// int	ft_execute_commands(t_shell *shell)
// {
// 	t_cmd *current;
	
// 	if (!shell->cmd_list || !shell->cmd_list->args)
// 		return (0); // Boş bir giriş bir hata değildir, yani program normal şekilde çalışmayı tamamlamış sayılır.
// 	current = shell->cmd_list;
// 	while (current)
// 	{
// 		current->expanded_argv = ft_expand_tokens(current->args, shell); //tüm komutların argv'lerini expand et //expander bitmediği için bu fonksiyon hazır değil
// 		if (!current->expanded_argv)
// 			return (-42);
// 		if (!current->expanded_argv || !current->expanded_argv[0])
// 			return (1);
// 		current = current->next;
// 	}
// 	if (ft_count_commands(shell->cmd_list) == 1)
// 		return (ft_execute_single_command(shell, shell->cmd_list));
// 	else
// 		return (ft_execute_multiple_command(shell));
// }
