#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
 
#include <sys/types.h>
#include <sys/wait.h>

static char cmp[1024];
static char* directories;
static char* args[512];
int command_pipe[2];
pid_t pid;
 
#define READ  0
#define WRITE 1

/*
void parsePath(void){
	char* s = getenv("PATH");
	directories = strtok(s, ":");
	while(directories) { 
		printf("%s\n", directories);
		directories = strtok(NULL, ":");
	}
}
*/

static int command(int input, int first, int last)
{
	int pipettes[2];
 
	/* Invoke pipe */
	pipe(pipettes);	
	pid = fork();
 
	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) {
			// First command
			dup2(pipettes[WRITE], STDOUT_FILENO);
		} else if (first == 0 && last == 0 && input != 0) {
			// Middle command
			dup2(input, STDIN_FILENO);
			dup2(pipettes[WRITE], STDOUT_FILENO);
		} else {
			// Last command
			dup2(input, STDIN_FILENO);
		}
 
		if (execvp( args[0], args) == -1){
			printf("%s", "No command in PATH\n");
			_exit(EXIT_FAILURE); // If child fails
		}
	}
 
	if (input != 0) 
		close(input);
 
	// Nothing more needs to be written
	close(pipettes[WRITE]);
 
	// If it's the last command, nothing more needs to be read
	if (last == 1)
		close(pipettes[READ]);
 
	return pipettes[READ];
}

char* skipwhite(char* s){
	//printf("%s", "hi4");
	while(isspace(*s)){
		++s;
	}
	return s;
}

int run(char *line, int input, int first, int last){
	line = skipwhite(line);
	char *next = strchr(line, ' ');
	int i = 0;

	while(next != NULL){
		next[0] = '\0';
		if(strcmp(line, ">") != 0){		
			args[i] = line;
			++i;
		}
		line = skipwhite(next + 1);
		next = strchr(line, ' ');
	}

	if(line[0] != '\0') {
		args[i] = line;
		next = strchr(line, '\n');
		next[0] = '\0';
		++i;
	}
	
	args[i] = NULL;
	
	if(args[0] != NULL){
		if(strcmp(args[0], "exit") == 0){
			exit(0);
		}
		return command(input, first, last);	
	}	
	return 0;
}

int main(void){
	while(1){
		printf("%s ", "%");
		fgets(cmp, 1024, stdin);		
		int input = 0;
		int first = 1;
		
		char* cmd = cmp;

		input = run(cmd, input, first, 1);
	}
	return 0;
}
