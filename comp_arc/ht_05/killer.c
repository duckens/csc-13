#include <glob.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

pid_t find_pid(const char *process_name){

	pid_t pid = -1;
	glob_t pglob;
	char *procname, *readbuf;
	int buflen = strlen(process_name) + 2;
	unsigned i;

	/* Get a list of all comm files. man 5 proc */
	if (glob("/proc/*/comm", 0, NULL, &pglob) != 0)
		return pid;

	/* The comm files include trailing newlines, so... */
	procname = malloc(buflen);
	strcpy(procname, process_name);
	procname[buflen - 2] = '\n';
	procname[buflen - 1] = 0;

	/* readbuff will hold the contents of the comm files. */
	readbuf = malloc(buflen);

	for (i = 0; i < pglob.gl_pathc; ++i) {
		FILE *comm;
		char *ret;

		/* Read the contents of the file. */
		if ((comm = fopen(pglob.gl_pathv[i], "r")) == NULL)
			continue;
		ret = fgets(readbuf, buflen, comm);
		fclose(comm);
		if (ret == NULL)
			continue;

		/*
		If comm matches our process name, extract the process ID from the
		path, convert it to a pid_t, and return it.
		*/
		if (strcmp(readbuf, procname) == 0) {
			pid = (pid_t)atoi(pglob.gl_pathv[i] + strlen("/proc/"));
			break;
		}
	}

	/* Clean up. */
	free(procname);
	free(readbuf);
	globfree(&pglob);

	return pid;
}


int main(int argc, char** argv){

	if(argc < 2 || argc > 3){
		printf("Usage: %s SIG PID or %s SIG (the later will try to send signal to \"handler\")\n", argv[0], argv[0]);
		exit(0);
	}


	int sig = 0;
	pid_t pid = 0;
	sscanf(argv[1], "%d", &sig);

	if(argc == 2){
		pid = find_pid("handler");
	} else {
		sscanf(argv[2], "%d", &pid);
	}
	
	if((sig == SIGTERM || sig == SIGUSR1 ) && (pid > 0)){
		printf("Sending signal %d to process %d..", sig, pid);
		
		int err = kill(pid, sig);
		if(!err){
			printf("success\n", sig, pid);
		} else {
			printf("fail\n", sig, pid);
		}
	} else {
		printf("Cannot send signal %d to process %d. Only %d and %d signal allowed. Pid > 0.\n", sig, pid, SIGTERM, SIGUSR1);
	}

	return 0;
}
