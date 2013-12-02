#include <stdio.h>


int sysprint(char* ptr, int size);

int main(){

	printf("Hello world, sign of 34 = %d\n", sgn(34));
	
	char str[15] = "Hello sysprint\n";
	sysprint(str, 15);

	return 0;
}
