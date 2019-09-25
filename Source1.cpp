#include <iostream>

using namespace std;

#define MAX 25

int main() {
	char buf[MAX];
	while (1) {
		fgets(buf, MAX, stdin);
		printf("String is %s\n", buf);
	}
	return 0;
}