#include <sys/wait.h>
#include <stdio.h>
#include "curr_time.h"
#include "tlpi_hdr.h"
#include "textColors.h"

#define BUF_SIZE 20

int main(int argc, char *argv[])
{
	int pfd[2];
	int j, dummy;
	
	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s sleep-time...\n", argv[0]);
	setbuf(stdout, NULL); /* Make stdout unbuffered, since we terminate child with _exit() */
	
	printf("%s Parent started\n", currTime("%T"));
	
	if (pipe(pfd) == -1) errExit("pipe");
	for (j = 1; j < argc; j++) {
		switch (fork()) {
		case -1:
		errExit("fork %d", j);

		case 0: /* Child */
			sleep(2);
			printf("%s Start %d (PID=%ld)\n", currTime("%T"), j, (long) getpid());
			if (close(pfd[0]) == -1) errExit("close");
			/* Read end is unused */
			/* Child does some work, and lets parent know it's done */
			sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));
			/* Simulate processing */
			printf("%s Child %d (PID=%ld) closing pipe\n",
			currTime("%T"), j, (long) getpid());
			if (close(pfd[1]) == -1)
				errExit("close");
			/* Child now carries on to do other things... */
			_exit(EXIT_SUCCESS);
		default: /* Parent loops to create next child */
			break;
		}
	}
	printf("Done for loop\n");

	/* Parent comes here; close write end of pipe so we can see EOF */
	if (close(pfd[1]) == -1) errExit("close"); /* Write end is unused */
	printf("D1\n");
	/* Parent may do other work, then synchronizes with children */
	ssize_t ass = read(pfd[0], &dummy, 1);
	printf("D2\n");
	if (ass  != 0){	/*Wait until read anything from pfd[0] to continue*/
		fatal("parent didn't get EOF");
	}
	else{
		printf("read(pfd[0], &dummy, 1) = %ld\n", ass);
	}
	printf("%s Parent ready to go\n", currTime("%T"));

	/* Parent can now carry on to do other things... */
	exit(EXIT_SUCCESS);
}
