#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "string_parser.h"
#include "command.h"

void error_handler(char* command) {
    write(1, "Error! Unsupported parameters for command: ", strlen("Error! Unsupported parameters for command: "));
    write(1, command, strlen(command));
    write(1, "\n", strlen("\n"));
}

void execute_command(char** command_list, int num_token) {
    if (strcmp(command_list[0], "ls") == 0) {
        if (num_token == 1) {
            listDir();
        } else {
            error_handler(command_list[0]);
        }
    } else if (strcmp(command_list[0], "pwd") == 0) {
        if (num_token == 1) {
            showCurrentDir();
        } else {
            error_handler(command_list[0]);
        }
    } else if (strcmp(command_list[0], "mkdir") == 0) {
        if (num_token == 2) {
            makeDir(command_list[1]);
        } else {
            error_handler(command_list[0]);
        }
    } else if (strcmp(command_list[0], "cd") == 0) {
        if (num_token == 2) {
            changeDir(command_list[1]);
        } else {
            error_handler(command_list[0]);
        }
    } else if (strcmp(command_list[0], "cp") == 0) {
        if (num_token == 3) {
            copyFile(command_list[1], command_list[2]);
        } else {
            error_handler(command_list[0]);
        }
    } else if (strcmp(command_list[0], "mv") == 0) {
        if (num_token == 3) {
            moveFile(command_list[1], command_list[2]);
        } else {
            error_handler(command_list[0]);
        }
    } else if (!(strcmp(command_list[0], "rm"))) {
        if (num_token == 2) {
            deleteFile(command_list[1]);
        } else {
            error_handler(command_list[0]);
        }
    } else if (!(strcmp(command_list[0], "cat"))) {
        if (num_token == 2) {
            displayFile(command_list[1]);
        } else {
            error_handler(command_list[0]);
        }
    } else {
        write(1, "Error! Unrecognized command: ", strlen("Error! Unrecognized command: "));
        write(1, command_list[0], strlen(command_list[0]));
        write(1, "\n", strlen("\n"));
    }
}

void interactive_mode() {
    int quit = 1;
    
    size_t len = 128;
	char* line_buf = malloc (len);

	command_line large_token_buffer;
	command_line small_token_buffer;

    while(quit) {
        write(1, ">>> ", strlen(">>> "));
        getline(&line_buf, &len, stdin);

        large_token_buffer = str_filler (line_buf, ";");

		//iterate through each large token
		for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
		{
			//tokenize large buffer
			//smaller token is seperated by " "(space bar)
			small_token_buffer = str_filler (large_token_buffer.command_list[i], " ");

            if (small_token_buffer.command_list[0] != NULL) {
                if (strcmp(small_token_buffer.command_list[0], "exit") == 0) {
                    quit = 0;
                } else {
                    execute_command(small_token_buffer.command_list, small_token_buffer.num_token);
                }
            }

			//free smaller tokens and reset variable
			free_command_line(&small_token_buffer);
			memset (&small_token_buffer, 0, 0);
		}

		//free smaller tokens and reset variable
		free_command_line (&large_token_buffer);
		memset (&large_token_buffer, 0, 0);
    }

    free(line_buf);
}

void file_mode(char** argv) {
    //opening file to read
	FILE *inFPtr;
	inFPtr = fopen (argv[2], "r");

    FILE *outFPtr;
    outFPtr = fopen("output.txt", "w");
    freopen("output.txt", "w", stdout);

	//declear line_buffer
	size_t len = 128;
	char* line_buf = malloc (len);

	command_line large_token_buffer;
	command_line small_token_buffer;

	//loop until the file is over
	while (getline (&line_buf, &len, inFPtr) != -1)
	{
		//tokenize line buffer
		//large token is seperated by ";"
		large_token_buffer = str_filler (line_buf, ";");
        //printf("[%s]\n", line_buf);
		//iterate through each large token
		for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
		{
			//tokenize large buffer
			//smaller token is seperated by " "(space bar)
			small_token_buffer = str_filler (large_token_buffer.command_list[i], " ");

            execute_command(small_token_buffer.command_list, small_token_buffer.num_token);

			//free smaller tokens and reset variable
			free_command_line(&small_token_buffer);
			memset (&small_token_buffer, 0, 0);
		}

		//free smaller tokens and reset variable
		free_command_line (&large_token_buffer);
		memset (&large_token_buffer, 0, 0);
	}
	fclose(inFPtr);
    fclose(outFPtr);
	//free line buffer
	free (line_buf);
}

int main(int argc, char** argv) {

    if (argc == 1) {
        interactive_mode();
    } else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        file_mode(argv);
    } else {
        printf("Error: Usage ./pseudo-shell or ./pseudo-shell -f <filename>\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
