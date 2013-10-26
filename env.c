#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
	int i;
	if(envp) {
		for(i=0;envp[i]!=NULL;i++) {
			printf("env[%i] = %s\n",i,envp[i]);
		}
	}	
}
