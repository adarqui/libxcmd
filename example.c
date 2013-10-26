#include "xcmd.h"

int main(int argc, char *argv[], char *envp[]) {
	xcmd_t * x_p;
	char buf[1024], *p;

	while(1) {
		memset(buf,0,sizeof(buf));

		p = fgets(buf,sizeof(buf)-1,stdin);
		if(!p) break;

		x_p = xcmd_create(buf);
		if(!x_p) {
			perror("main:xcmd_create");
			exit(-1);
		}

		xcmd_info(x_p);
		xcmd_fini(x_p);
	}
	return 0;
}
