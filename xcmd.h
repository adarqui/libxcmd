#ifndef XCMD_H
#define XCMD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <json-c/json.h>

typedef enum xcmd_array {
	XCMD_ARRAY_ARGV,
	XCMD_ARRAY_ENVP,
} xcmd_array_t;

typedef struct Xcmd {
	int argc;
	char ** argv;
	char ** envp;
	json_object *js;
	json_object *js_argv;
	json_object *js_envp;
} xcmd_t;

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

/* wrapper */
xcmd_t * xcmd_create(char *);

#endif
