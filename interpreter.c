#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 6;

int help();
int quit();
int badcommand();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int badcommandFileDoesNotExist();

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} 
	else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size != 1) return badcommand();
		system("ls -1");
		return 0;

	} 
	else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size<3) return badcommand();
		else if (args_size==3) return set(command_args[1], command_args[2]);
		else if (3<args_size) {
			char * s = malloc (100 * args_size *sizeof(char));
		
			for(int i=2; i < args_size; i++) {
				strcat(s, command_args[i]);
				strcat(s, " ");
			}
			return set(command_args[1], s);
		}
	} else if(strcmp(command_args[0], "echo")==0) {
		if (args_size != 2) return badcommand();
		else{
			if(command_args[1][0]=='$') {
				char * s = malloc(100*sizeof(char));
				strcat(s, &command_args[1][1]);

				if (strcmp(mem_get_value(s), "Variable does not exist")==0) {
					printf("\n");
					free(s);
					return 0;
				} else{
					printf("%s\n", mem_get_value(s));
					free(s); 
					return 0;
				}
			} else{
				char * b = malloc(100*sizeof(char));
				strcat(b, command_args[1]);
				printf("%s\n", b);
				free(b);
				return 0;
			}
		}
		
	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size != 1) return badcommand();
		system("ls -1");
		return 0;
	}
	else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} 
	
	else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int set(char* var, char* value){

	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);

	mem_set_value(var, value);

	return 0;
}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);

	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}
