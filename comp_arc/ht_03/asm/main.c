#include <stdio.h>

int asm_func();

int get_esp();
int get_ebp();
int get_cs();
int get_ds();
int get_ss();
// int get_eflags();

int get_esp(){
	asm("mov %esp,%eax");
}
int get_ebp(){
	asm("mov %ebp,%eax");
}
int get_cs(){
	asm("mov %cs,%eax");
}
int get_ds(){
	asm("mov %ds,%eax");
}
int get_ss(){
	asm("mov %ss,%eax");
}
// int get_eflags(){
// 	asm("mov %eflags,%eax");
// }

int main(){

	int n = asm_func();

	printf("n = %d\n", n);
	
	// printf("cs = %d\n", get_esp());
	// printf("cs = %d\n", get_ebp());
	// printf("cs = %d\n", get_cs());
	// printf("ds = %d\n", get_ds());
	// printf("ss = %d\n", get_ss());
	
	return 0;
}