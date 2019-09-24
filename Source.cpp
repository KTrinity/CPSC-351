#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstdlib>


#define MAX 1000

using namespace std;

int main() {
	char str[MAX];
	char seperators[] = " \n";
	const char* commandList[] = { "dir", "help", "vol", "path", "tasklist", "notepad", "echo", "color", "ping", "exit", "quit" };
	int commandSize = sizeof(commandList) / sizeof(commandList[0]);
	char* command;

	cout << "Welcome to myShell\n";
	
	while (1) {
		char* token;
		char* total_token[4];
		size_t i = 0;
		size_t j = 0;
		bool match = false;
		fgets(str, MAX, stdin);
		token = strtok(str, seperators);
		total_token[j++] = token;

		while ((match != true) && (i < commandSize)) {
			if (strcmp(token, commandList[i]) == 0) {
				match = true;
			}
			++i;
		}
		if (match == false) {
			cout << "/'" << token << "/' is not recognized as an internal or external command,\noperable program or batch file.\n";
		}
		else if (strcmp(token, commandList[9]) == 0 || strcmp(token, commandList[10]) == 0) {
			cout << "Thanks for using myShell!\n";
			system("PAUSE");
			return 0;
		}
		else {
			while ((token = strtok(NULL, seperators)) != NULL) {
				total_token[j++] = token;
			}
			if (j <= 4) {
				command = total_token[0];
				for (int i = 1; i < j; i++) {
					strcat(command, total_token[i]);
				}
				cout << command << endl;
			}
		}

	}
	system("PAUSE");
	return 0;
}