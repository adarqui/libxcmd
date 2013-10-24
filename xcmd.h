#ifndef XCMD_H
#define XCMD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct Xcmd {
	char **seps;
	int argc;
	char ** argv;
	char ** envp;
} xcmd_t;

xcmd_t * xcmd_init(void);
void xcmd_fini(xcmd_t *);
void xcmd_add_seps(xcmd_t *, char *, ...);
void xcmd_process_argv(xcmd_t *, char *);
void xcmd_process_envp(xcmd_t *, char *);
void xcmd_info(xcmd_t *);

void xcmd_free_loop(char **);

/* wrapper */
xcmd_t * xcmd_create(char * argv, char * envp, char * argv_and_envp, char *);

#endif
