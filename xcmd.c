#include "xcmd.h"

void xcmd_strip(char *s) {
	if(!s) return;
	while(*s) {
		if(*s == '\r' || *s == '\n') *s = '\0';
		s++;
	}
	return;
}

char * xcmd_eat(char *s, char c) {

	if(!s) return s;

	while(*s == c) s++;

	return s;
}

void xcmd_free_loop(char **p) {
	int i;

	if(!p) return;

	for(i = 0; p[i] != NULL; i++) {
		free(p[i]);
	}

	free(p);

	return;
}

void xcmd_free_js_obj(json_object *js) {

	if(!js) return;

	/* Is this dangerous? Will loop if ref count > 1. But, it should never be >  1 */
	while(json_object_put(js) != 1);
	return;
}

xcmd_t * xcmd_init(void) {
	return (xcmd_t *) calloc(1,sizeof(xcmd_t));
}


void xcmd_fini(xcmd_t *x_p) {
	if(!x_p) return;

	xcmd_free_loop(x_p->argv);
	xcmd_free_loop(x_p->envp);
	xcmd_free_js_obj(x_p->js_argv);
	xcmd_free_js_obj(x_p->js_envp);
	xcmd_free_js_obj(x_p->js);
	
	memset(x_p,0,sizeof(xcmd_t));
	return;
}


xcmd_t * xcmd_fini_NULL(xcmd_t *x_p) {
	xcmd_fini(x_p);
	return NULL;
}

int xcmd_process_array(xcmd_t * x_p, json_object * array, xcmd_array_t array_type) {
	json_object * js_ptr;
	struct array_list * al;
	char * s, *** array_ptr_ptr_ptr, **array_ptr_ptr;
	int alc = 0, i, j = 0, *argc_ptr;

	if(!x_p || !array) return -1;

	switch(array_type) {
		case XCMD_ARRAY_ARGV: {
			array_ptr_ptr_ptr = &x_p->argv;
			argc_ptr = &x_p->argc;
			break;
		}
		case XCMD_ARRAY_ENVP: {
			array_ptr_ptr_ptr = &x_p->envp;
			argc_ptr = &j;
			break;
		}
		default : {
			return -1;
		}
	}

	al = json_object_get_array(array);
	if(!al) return -1;

	*array_ptr_ptr_ptr = (char **) calloc(al->length+1,sizeof(char *));
	if(!*array_ptr_ptr_ptr) return -1;

	array_ptr_ptr = (char **) *array_ptr_ptr_ptr;
	
	for(i = 0; i < al->length; i++) {
		js_ptr = (json_object *)al->array[i];
		if(!js_ptr) continue;

		if(json_object_get_type(js_ptr) != json_type_string) continue;

		s = (char *) json_object_get_string(js_ptr);
		if(!s) continue;

		*argc_ptr = (*argc_ptr) + 1;
		array_ptr_ptr[i] = (char *) strdup(s);
	}


	return alc;
}

int xcmd_process_argv(xcmd_t * x_p, json_object * argv) {
	return xcmd_process_array(x_p,argv,XCMD_ARRAY_ARGV);
}

int xcmd_process_envp(xcmd_t * x_p, json_object * envp) {
	return xcmd_process_array(x_p,envp,XCMD_ARRAY_ENVP);
}


void xcmd_info_print_array(char **array, char * prefix) {
	int i;
	if(!array || !prefix) return;

	for(i=0;array[i]!=NULL;i++) {
		printf("%s%s\n", prefix, array[i]);
	}

	return;
}

void xcmd_info(xcmd_t * x_p) {
	if(!x_p) return;

	puts("xcmd_info:");

	printf("\targc:\n\t\t%i\n",x_p->argc);
	printf("\trun:\n\t\t%i\n",x_p->run);

	puts("\targv:");
	xcmd_info_print_array(x_p->argv, "\t\t");

	puts("\tenvp;");
	xcmd_info_print_array(x_p->envp, "\t\t");

	return;
}


xcmd_t * xcmd_create(char *js_str) {
	xcmd_t * x_p = NULL;

	if(!js_str) {
		return NULL;
	}

	x_p = xcmd_init();
	if(!x_p) return NULL;

	x_p->js = json_tokener_parse(js_str);
	if(!x_p->js) return xcmd_fini_NULL(x_p);

	x_p->js_argv = json_object_object_get(x_p->js, "argv");
	if(!x_p->js_argv) return xcmd_fini_NULL(x_p);

	if(json_object_get_type(x_p->js_argv) != json_type_array) return xcmd_fini_NULL(x_p);

	x_p->js_envp = json_object_object_get(x_p->js, "envp");
	if(!x_p->js_envp) return xcmd_fini_NULL(x_p);

	if(json_object_get_type(x_p->js_envp) != json_type_array) return xcmd_fini_NULL(x_p);

	xcmd_process_argv(x_p, x_p->js_argv);
	xcmd_process_envp(x_p, x_p->js_envp);
	xcmd_flags_parse(x_p);

	return x_p;
}

void xcmd_flags_parse(xcmd_t *x_p) {
	json_object *js_flags;
	char * s;

	if(!x_p && !x_p->js) return;

	js_flags = json_object_object_get(x_p->js,"flags");
	if(!js_flags) return;

	if(json_object_get_type(js_flags) != json_type_string) goto cleanup;

	s = (char *)json_object_get_string(js_flags);
	if(!s) goto cleanup;

	if(!strcasecmp(s, "wait")) {
		x_p->flags |= XCMD_FLAGS_WAIT;
	}

	cleanup:
	if(js_flags) json_object_put(js_flags);

	return;
}

void xcmd_run_parse(xcmd_t *x_p) {
	json_object *js_run;
	char * s;

	if(!x_p && !x_p->js) return;

	js_run = json_object_object_get(x_p->js,"run");
	if(!js_run) return;

	if(json_object_get_type(js_run) != json_type_string) goto cleanup;

	s = (char *)json_object_get_string(js_run);
	if(!s) goto cleanup;

	if(!strcasecmp(s, "none")) {
		x_p->run = XCMD_RUN_NONE;
	} else if(!strcasecmp(s, "cb")) {
		x_p->run = XCMD_RUN_CB;
	} else if(!strcasecmp(s, "exec")) {
		x_p->run = XCMD_RUN_EXEC;
	} else if(!strcasecmp(s, "fork")) {
		x_p->run = XCMD_RUN_FORK;
	} else if(!strcasecmp(s, "pthread")) {
		x_p->run = XCMD_RUN_PTHREAD;
	}
	
	cleanup:
	if(js_run) json_object_put(js_run);

	return;
}

int xcmd_run(xcmd_t *x_p, xcmd_run_fn fn) {
	int n = -1;

	if(!x_p) return n;
	
	if(!x_p->argc || !x_p->argv || !x_p->envp) return n;

	if(!x_p->run) { xcmd_run_parse(x_p); }
	if(!x_p->run) return n;

	switch(x_p->run) {
		case XCMD_RUN_EXEC: {
			n = execve(x_p->argv[0], x_p->argv, x_p->envp);
			break;
		}
		case XCMD_RUN_CB: {
			if(!fn) break;
			n = fn(x_p->argc, x_p->argv, x_p->envp);
			break;
		}
		case XCMD_RUN_FORK: {
			if(!fn) break;
			if(!fork()) {
				n = fn(x_p->argc, x_p->argv, x_p->envp);
				exit(n);
			} else {
				if(x_p->flags & XCMD_FLAGS_WAIT) {
					wait(&n);
				} else {
					waitpid(-1, &n, WNOHANG);
				}
			}
		}
		default: {
			return n;
		}
	}

	return n;
}
