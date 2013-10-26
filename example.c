#include "xcmd.h"
#include <errno.h>
#
extern int errno;

int fake_main(int argc, char *argv[], char *envp[]) {
	if(argc <= 0 || !argv || !envp) return -1;

	printf("fake_main! %i %p %p\n", argc, argv, envp);

	return 0;
}

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

		xcmd_run_parse(x_p);
		xcmd_info(x_p);
		errno = 0;
		if(xcmd_run(x_p, fake_main) < 0) {
			if(errno != 0) {
				perror("main:xcmd_run:");
			}
		}
		xcmd_fini(x_p);
	}
	return 0;
}
