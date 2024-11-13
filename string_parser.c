/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2020
 *      Author: gguan, Monil
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GUN_SOURCE

int count_token (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/

	// Check for NULL string
    if (buf == NULL || *buf == '\0') {
        return 0;
    }

    int count = 0;
	char* saveptr;

    char* token = strtok_r(buf, delim, &saveptr);

    // Iterate through the string counting tokens
    while (token != NULL) {
        count++;
        token = strtok_r(NULL, delim, &saveptr);
    }

    return count;
}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled and returned
	*	#2.	count the number of tokens with count_token function, set num_token. 
    *           one can use strtok_r to remove the \n at the end of the line.
	*	#3. malloc memory for token array inside command_line variable
	*			based on the number of tokens.
	*	#4.	use function strtok_r to find out the tokens 
    *   #5. malloc each index of the array with the length of tokens,
	*			fill command_list array with tokens, and fill last spot with NULL.
	*	#6. return the variable.
	*/

    command_line result;

    int i = 0;
    while (buf[i] != '\0') {
        if (buf[i] == '\n') {
            // Shift all characters to the left by one
            int j = i;
            while (buf[j] != '\0') {
                buf[j] = buf[j + 1];
                j++;
            }
        } else {
            i++;
        }
    }

    //strtok is a destructive operation- create a duplicate buffer to count the tokens before tokenizing the original string.
    char *copy = strdup(buf);
    result.num_token = count_token(copy, delim);
    free(copy);

    //initialize command list to num_tokens + 1 space for the NULL last space.
    result.command_list = (char **)malloc(sizeof(char *) * (result.num_token + 1));

    char *parser_saveptr = NULL;

    char *token = strtok_r(buf, delim, &parser_saveptr);
    
    //tokenize the original string, putting it into command list
    for(int i = 0; token != NULL; i++) {
        result.command_list[i] = strdup(token);
        token = strtok_r(NULL, delim, &parser_saveptr);
    }

    result.command_list[result.num_token] = NULL;

    return result;
}


void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/

	// Free the array of strings
    for (int i = 0; i < command->num_token; i++) {
        free(command->command_list[i]);
    }
    // Free the array itself
    free(command->command_list);

}
