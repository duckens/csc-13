#include <stdio.h>

int sgn(int x);
int sysprint(char* ptr, int size);

int main(){

	printf("Hello world, sign of 34 = %d", sgn(34));
	char str[15] = "Hello sysprint\n";
	sysprint(str, 15);

	return 0;
}
