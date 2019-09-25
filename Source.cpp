#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define MAX 1000 // Used to initialize char[] to recieve input from user

using namespace std;

void CreateThread(char* token, char** total_token, size_t token_sum); // Prototype for CreateThread below, parameters are initial token, token array, and number of tokens.

char seperators[] = " \n"; // Used for strtok, sets delimiters to " " and "\n"
const char* commandList[] = { "dir", "help", "vol", "path", "tasklist", "notepad", "echo", "color", "ping", "exit", "quit" }; // List of shell commands to be checked with first token


int main() {
	int commandSize = sizeof(commandList) / sizeof(commandList[0]); // Number of shell commands (11) to determine if token has been been compared with all elements in commandList[]

	cout << "Welcome to myShell\n"; // Introduction

	while (1) { // Continues to loop until "exit" or "quit" command is called
		char* total_token[4]; // Array containing up to 4 tokens/arguments
		char str[MAX]; // Initializes char[] with 1000 elements
		char* token; // char* to receive tokens from fgets()
		size_t command_token_counter = 0; // token_counter used to check each command in commandList. Resets to 0 after every user input.
		size_t token_sum = 0; // token_counter used to check number of tokens
		bool match = false; // bool used to check if user input is a command from commandList

		fgets(str, MAX, stdin); // Stores user input into str
		token = strtok(str, seperators); // Sets token to first token in str
		total_token[token_sum++] = token; // Adds token to the array of tokens

		while ((match != true) && (command_token_counter < commandSize)) { // Loop while token does not match command in commmandList or until command_token_counter reaches the end of the commandList
			if (strcmp(token, commandList[command_token_counter]) == 0) { // Checks to see if token matches command
				match = true; // Set match to true if token matches (used to exit while loop)
			}
			++command_token_counter; // Increase command token_counter to check next command
		}
		if (match == false) { // If match not found, send error and return to top of loop
			cout << "/'" << token << "/' is not recognized as an internal or external command,\noperable program or batch file.\n"; // Error message, invalid command
		}
		else if (strcmp(token, commandList[9]) == 0 || strcmp(token, commandList[10]) == 0) { // Compares token to "exit" or "quit"
			cout << "Thanks for using myShell!\n"; // Exit message
			system(commandList[9]); // calls system("exit");
			return 0; // Exit Status: Success
		}
		else {
			CreateThread(token, total_token, token_sum); // Calls function CreateThread with initial token, array of tokens, and token token_counter
		}

	}
	return 0; // Exit Status: Success
}

void CreateThread(char* token, char** total_token, size_t token_sum) { // Function to create command to use with system()
	char space[] = " "; // Used to add space to char commands[] in CreateThread function
	char* command[7]; // Array containing at most 4 tokens with at most 3 spaces to be used in system()
	size_t token_counter = 0; // token_counter to keep track of iterations through 

	while ((token = strtok(NULL, seperators)) != NULL) { // Loop while tokens still exist in user input
		total_token[token_sum++] = token; // Add token to token array
	}
	if (token_sum <= 4) { // Total amount of tokens/arguments must be less than 4
		for (int i = 0; i < (token_sum + (token_sum - 1)); i++) { // Iterates through all tokens to create complete command with spaces
			if ((i % 2) == 1) { // If the element position is odd, set element to a " "
				command[i] = space;
			}
			else {
				command[i] = total_token[token_counter++]; // Sets element in command to the token
			}

		}
	}
	else {
		cout << "Invalid Number of Arguments" << endl; // Error message
		return; // Exit CreateThread()
	}

	char* output; // Output to be made with command[]
	size_t len = 0; // Number of char bytes to be allocated for output
	for (int i = 0; i < (token_sum + (token_sum - 1)); i++) { // Iterates through all tokens
		len += strlen(command[i]); // Sum length of all tokens to len
	}
	len *= sizeof(char); // Length converted into number of char bytes
	len += 1; // Additional byte for null terminator \0

	output = (char*)malloc(len); // Allocates memory for the correct amount of bytes 

	for (int i = 0; i < (token_sum + (token_sum - 1)); i++) { // Iterates through all tokens
		if (i == 0) { // If it is the first token, use strcpy to intialize output to the first token
			strcpy(output, command[i]);
		}
		else {
			strcat(output, command[i]); // Adds following tokens to output
		}
	}
	system(output); // Call system with full output (i.e. ping 192.168.1.4)
}
