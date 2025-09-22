#include "../minishell.h"

static int	ft_create_pipe(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1) //pipe hazır bir fonksiyon. pipefd[0]=okuma ucu | pipefd[1]=yazma ucu
		{
			perror("minishell: pipe");
			return (1);
		}
	}
	return (0);
}

static pid_t	ft_create_child_and_execute(t_shell *shell, t_cmd *cmd,
									int pipefd[2], int prev_fd)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
		ft_execute_child_process(shell, cmd, pipefd, prev_fd);
	return (pid);
}

static int	ft_wait_all_children(void)
{
	int status;
	int last_status;
	int	sig;
	int	quit_printed;
	
	last_status = 0;
	quit_printed = 0;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			
			// SIGPIPE'ı ignore et (pipe'da normal durum)
			if (sig == SIGPIPE)
			{
				// SIGPIPE durumunda last_status'u değiştirme
				// Son child'ın exit status'unu koru
				continue;
			}
			
			if (sig == SIGQUIT && !quit_printed)
			{
				ft_putstr_fd("Quit (core dumped)\n", 2);
				quit_printed = 1;
			}
			last_status = 128 + WTERMSIG(status);
		}
	}
	return (last_status);
}

int	ft_execute_multiple_command(t_shell *shell)
{
	t_cmd	*current;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;

	current = shell->cmd_list;
	prev_fd = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	while (current)
	{
		if (ft_create_pipe(current, pipefd))
			return (1);
		pid = ft_create_child_and_execute(shell, current, pipefd, prev_fd);
		if (pid == -1)
			return (1);
		if (prev_fd != -1)
			close(prev_fd);
		if (current->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		current = current->next;
	}
	return (ft_wait_all_children());
}


/*
ÖRNEK SENARYO: ls | grep txt | wc -l

İterasyon 1: ls
prev_fd = -1 (yok)
pipe() → pipefd[0]=4, pipefd[1]=5
fork() → child: ls çalışır, STDOUT→5
parent: close(5), prev_fd=4

İterasyon 2: grep
prev_fd = 4 (ls'ten gelen)
pipe() → pipefd[0]=6, pipefd[1]=7
fork() → child: grep çalışır, STDIN→4, STDOUT→7
parent: close(4), close(7), prev_fd=6

İterasyon 3: wc
prev_fd = 6 (grep'ten gelen)
pipe() → OLUŞTURULMAZ (son komut)
fork() → child: wc çalışır, STDIN→6, STDOUT→ekran
parent: close(6)


PIPE1        PIPE2
    [4][5]       [6][7]
ls ───5→ grep ←4───7→ wc ←6─── ekran
    ↑              ↑
 child1         child2      child3

*/