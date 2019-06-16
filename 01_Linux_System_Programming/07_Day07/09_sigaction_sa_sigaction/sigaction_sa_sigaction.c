#include <headers.h>

#if 0
siginfo_t {
	int      si_signo;     /* Signal number */
	int      si_errno;     /* An errno value */
	int      si_code;      /* Signal code */
	int      si_trapno;    /* Trap number that caused
							  hardware-generated signal
							  (unused on most architectures) */
	pid_t    si_pid;       /* Sending process ID */
	uid_t    si_uid;       /* Real user ID of sending process */
	int      si_status;    /* Exit value or signal */
	clock_t  si_utime;     /* User time consumed */
	clock_t  si_stime;     /* System time consumed */
	sigval_t si_value;     /* Signal value */
	int      si_int;       /* POSIX.1b signal */
	void    *si_ptr;       /* POSIX.1b signal */
	int      si_overrun;   /* Timer overrun count;
							  POSIX.1b timers */
	int      si_timerid;   /* Timer ID; POSIX.1b timers */
	void    *si_addr;      /* Memory location which caused fault */
	long     si_band;      /* Band event (was int in
							  glibc 2.3.2 and earlier) */
	int      si_fd;        /* File descriptor */
	short    si_addr_lsb;  /* Least significant bit of address
							  (since Linux 2.6.32) */
	void    *si_lower;     /* Lower bound when address violation
							  occurred (since Linux 3.19) */
	void    *si_upper;     /* Upper bound when address violation
							  occurred (since Linux 3.19) */
	int      si_pkey;      /* Protection key on PTE that caused
							  fault (since Linux 4.6) */
	void    *si_call_addr; /* Address of system call instruction
							  (since Linux 3.5) */
	int      si_syscall;   /* Number of attempted system call
							  (since Linux 3.5) */
	unsigned int si_arch;  /* Architecture of attempted system call
							  (since Linux 3.5) */
}
#endif
//void myfun(int sig)
//{
//	printf("sig = %d\n", sig);
//	sleep(5);
//	printf("wake up...\n");
//}
void fun(int signo, siginfo_t * info, void* context)
{
	printf("捕捉到的信号   ：%d\n", signo);
	printf("信号编号       ：%d\n", info->si_signo);
	printf("传递的值       ：%d\n", info->si_value.sival_int);
	printf("发送信号的进程 ：%d\n", info->si_pid);
}

int main()
{
	int ret = -1;
	pid_t pid = -1;
	int status = -1;

	union sigval tmp;
	tmp.sival_int = 100;

	struct sigaction act= {
		.sa_flags = SA_SIGINFO,
		.sa_sigaction = fun,
		//.sa_handler = myfun
	}, old_act;

	ret = sigaction(SIGINT, &act, &old_act);

	pid = fork();
	if(-1 == pid)
	{
		perror("fork");
		return 1;
	}
	else if( 0 == pid )
	{
		for(int i = 5; i>0; i--)
		{
			printf("即将发送数据：倒计时%d\n", i);
			sleep(1);

		}
		ret = sigqueue(getppid(), SIGINT, tmp);
	}

	

	printf("press any key\n");
	waitpid(pid, &status, 0);


    return 0;
}

