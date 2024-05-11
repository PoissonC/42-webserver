#include <stdio.h>
#include <unistd.h>
int main(){
	int wc = write(1, "hello world\n", 100);
	printf("%d\n", wc);
}