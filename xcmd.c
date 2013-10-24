#include "xcmd.h"

void xcmd_free_loop(char **p) {
	int i;

	if(!p) return;

	for(i = 0; p[i] != NULL; i++) {
		free(p[i]);
	}

	return;
}

xcmd_t * xcmd_init(void) {
	xcmd_t * x_p = NULL;

	x_p = (xcmd_t *) calloc(1,sizeof(xcmd_t));
	return x_p;
}


void xcmd_fini(xcmd_t *x_p) {
	if(!x_p) return;

	xcmd_free_loop(x_p->seps);
	xcmd_free_loop(x_p->argv);
	xcmd_free_loop(x_p->envp);
	
	memset(x_p,0,sizeof(xcmd_t));
	return;
}

void xcmd_add_seps(xcmd_t *x_p, char * sep, ...) {
	va_list ap;
	char * s;

	if(!x_p || !sep) return;

	s = sep;
	va_start(ap,sep);

	while(s) {
		printf("hi %s\n", s);
		s = va_arg(ap, char *);
		if(!s) break;
	}

	va_end(ap);

	return;
}


void xcmd_process_argv(xcmd_t * x_p, char * argv) {
	return;
}

void xcmd_process_envp(xcmd_t * x_p, char * envp) {
	return;
}

void xcmd_info(xcmd_t * x_p) {
	if(!x_p) return;

	printf("xcmd_info:\n");
	return;
}


xcmd_t * xcmd_create(char * argv, char * envp, char * argv_and_envp, char * seps) {
	xcmd_t * x_p = NULL;
	char * s_1, *s_2, *divider = "__SEP__";

	if(((!argv && !envp) && !argv_and_envp) || !seps) {
		return NULL;
	}

	x_p = xcmd_init();
	xcmd_add_seps(x_p, "``", NULL);

	if(argv && envp) {
		xcmd_process_argv(x_p, argv);
		xcmd_process_envp(x_p, envp);
	} else if(argv_and_envp) {
		s_1 = strstr(argv_and_envp, divider);
		if(!s_1) {
			xcmd_process_argv(x_p, argv_and_envp);
		} else {
			s_1 = strstr(argv_and_envp, divider);
			*s_1 = '\0';
			s_1 += strlen(divider);
			s_2 = argv_and_envp;

			xcmd_process_argv(x_p, s_1);
			xcmd_process_envp(x_p, s_2);
		}
	}

	return x_p;
}
