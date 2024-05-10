#include <arpa/inet.h> 
#include <stdio.h>
int main(){
	printf("%u\n", inet_addr("127.0.0.1"));
}