#include <stdio.h>

int main(void) {
	char* ch = new char(2);
	char* chh = new char[2];
	ch[0] = 'a';
	chh[0] = 'b';
	printf("%c", ch[0]);
}