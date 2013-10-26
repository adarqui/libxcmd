#ifndef XCMD_H
#define XCMD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <json-c/json.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef enum xcmd_ARRAY {
	XCMD_ARRAY_ARGV,
	XCMD_ARRAY_ENVP,
} xcmd_array_t;

typedef enum xcmd_RUN {
	XCMD_RUN_NONE = 0,
	XCMD_RUN_CB,
	XCMD_RUN_EXEC,
	XCMD_RUN_FORK,
	XCMD_RUN_PTHREAD
} xcmd_run_t;

typedef enum xcmd_FLAGS {
	XCMD_FLAGS_WAIT = 0x01,
} xcmd_flags_t;

typedef struct Xcmd {
	int argc;
	char ** argv;
	char ** envp;
	int run;
	int flags;
	json_object *js;
	json_object *js_argv;
	json_object *js_envp;
} xcmd_t;

typedef int (*xcmd_run_fn)(int,char **,char **);

xcmd_t * xcmd_init(void);
void xcmd_fini(xcmd_t *);

int xcmd_process_array(xcmd_t *, json_object *, xcmd_array_t);
int xcmd_process_argv(xcmd_t *, json_object *);
int xcmd_process_envp(xcmd_t *, json_object *);

char * xcmd_reverse(int, char **, char **);

void xcmd_info_print_array(char **, char *);
void xcmd_info(xcmd_t *);

void xcmd_free_loop(char **);
void xcmd_free_js_obj(json_object *);

void xcmd_strip(char *);
char * xcmd_eat(char *, char c);

void xcmd_flags_parse(xcmd_t *);

void xcmd_run_parse(xcmd_t *);
int xcmd_run(xcmd_t *, xcmd_run_fn);

/* wrapper */
xcmd_t * xcmd_create(char *);

#endif
