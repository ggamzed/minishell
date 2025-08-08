#include "../minishell.h"

//fd olayı -> executor.txt

// tek komut çalıştırır. builtin -> parent / external -> fork
/* Builtin komut için redirection setup ve cleanup yapar */
static int	ft_handle_builtin_redirections(t_cmd *cmd, int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO); // klavyenin kopyasını al //Var olan bir file descriptor'ın kopyasını oluşturur.
	*stdout_backup = dup(STDOUT_FILENO); // ekranın(terminal) kopyasını al
	/*
	int backup = dup(STDOUT_FILENO);  // STDOUT_FILENO = 1
	backup = 3 oldu diyelim
	Şimdi hem 1 hem 3 AYNI YERE (ekrana) işaret ediyor!
	*/
	if (setup_redirections(cmd) != 0) // bu fonksiyon yok
	{
		close(*stdin_backup);
		close(*stdout_backup);
		return (1);
	}
	return (0);
}

/* Builtin komut sonrası original fd'leri restore eder */
/*
neden restore yapıyoruz?
fork açılmadığı için:
1. echo "hello" > file.txt çalışıyor
2. Shell'in STDOUT'u file.txt'ye yönlendiriliyor
3. echo çalışıyor, "hello" file.txt'ye yazılıyor
4. AMA STDOUT hala file.txt'yi gösteriyor
5. pwd komutunu yazdığınızda → EKRANA DEĞİL, file.txt'ye yazılır
6. Bundan sonraki TÜM komutlar file.txt'ye gider
*/
static void	ft_restore_redirections(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO); //dup2(int oldfd, int newfd); newfd'yi oldfd'nin gösterdiği yere yönlendirir.
	dup2(stdout_backup, STDOUT_FILENO);
	/*
	int file_fd = open("output.txt", O_WRONLY);  // file_fd = 4 diyelim
	dup2(file_fd, STDOUT_FILENO);  // STDOUT'u (1) file'a yönlendir
	Artık printf() ekrana değil, output.txt'ye yazacak!
	*/
	close(stdin_backup);
	close(stdout_backup);
}

/* Builtin komutu parent process'te çalıştırır */
static int	ft_execute_builtin_in_parent(t_shell *shell, t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;
	int	result;

	if (ft_handle_builtin_redirections(cmd, &original_stdin, &original_stdout) != 0)
		return (1);
	
	result = ft_execute_builtin(shell, cmd, 0);
	ft_restore_redirections(original_stdin, original_stdout);
	return (result);
}

/* External komut için fork yapar ve child process'i başlatır */
static int	ft_execute_external_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		ft_execute_child_process(shell, cmd, NULL, -1);
	// ignore_signals(); // Sinyal fonksiyonu henüz yok
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));			//-->> exit status makroları, executor.txt
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

/* Ana single command executor - builtin veya external komut çalıştırır */
int	ft_execute_single_command(t_shell *shell, t_cmd *cmd)
{
	if (ft_is_builtin(cmd->expanded_argv[0]))
		return (ft_execute_builtin_in_parent(shell, cmd));
	else
		return (ft_execute_external_command(shell, cmd));
}
