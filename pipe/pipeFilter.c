#include <sys/wait.h>
#include <stdio.h>
#include "tlpi_hdr.h"
#include "textColors.h"

#define BUF_SIZE 20

int main(int argc, char *argv[])
{
	int pfd[2];
	char buf[BUF_SIZE];
	ssize_t numRead;
	printf("Pipe Parent-Child Process!!!\n");

	if (argv[1] == NULL){
		printf("Warning(No input Data)\n");
		printf("The input Data will be set as Default\n");
		argv[1] = "Default";
	}

	if (pipe(pfd) == -1) /*check create pipe file error*/
		errExit("pipe");

	switch (fork()) {	/*create a child process*/
	case -1:
		errExit("fork"); 
	case 0: /* Child Process*/
		if (close(pfd[1]) == -1) /* Close unused write end, */
			errExit("close - Child process");
		
		/* Child now reads from pipe */
		while(1) {
			/* Read data from pipe, echo on stdout */
			numRead = read(pfd[0], buf, BUF_SIZE);
			if (numRead == -1) errExit("read");
			if (numRead == 0) break; /* End-of-file */
			printf("Child read: \033[1;31m%s\033[0m \n", buf);
			// if (write(STDOUT_FILENO, buf, numRead) != numRead)
			// 	fatal("child - partial/failed write");
		}
		// write(STDOUT_FILENO, "\n", 1);
		if (close(pfd[0]) == -1) errExit("close");
		_exit(EXIT_SUCCESS);
		break;

	default: /* Parent Process*/
		if (close(pfd[0]) == -1) 	/* Close unused read end */
			errExit("close - Parent process");

		if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1])){
			//do nothing here
			printf("Parent failed write");
			// fatal("parent - partial/failed write");
		}
		else {
			printf("Parent wrote: \033[1;31m%s\033[0m\n", argv[1]);
		}

		/* Parent now writes to pipe */
		if (close(pfd[1]) == -1) errExit("close");
		wait(NULL);
		exit(EXIT_SUCCESS);

		break;
	}
}