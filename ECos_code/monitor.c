/***************************************************************************
| File: monitor.c
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "project.h"

/*-------------------------------------------------------------------------+
| Headers of command functions
+--------------------------------------------------------------------------*/
extern void cmd_sair(int, char**);
extern void cmd_ini(int, char**);

extern void cmd_rc(int, char**);
extern void cmd_sc(int, char**);
extern void cmd_rtl(int, char**);
extern void cmd_rp(int, char**);
extern void cmd_mmp(int, char**);
extern void cmd_mta(int, char**);
extern void cmd_ra(int, char**);
extern void cmd_dac(int, char**);
extern void cmd_dtl(int, char**);
extern void cmd_aa(int, char**);
extern void cmd_ir(int, char**);
extern void cmd_trc(int, char**);
extern void cmd_tri(int, char**);
extern void cmd_irl(int, char**);
extern void cmd_lr(int, char**);
extern void cmd_dr(int, char**);
extern void cmd_cpt(int, char**);
extern void cmd_mpt(int, char**);
extern void cmd_cttl(int, char**);
extern void cmd_dttl(int, char**);
extern void cmd_pr(int, char**);

       void cmd_sos(int, char**);

/*-------------------------------------------------------------------------+
| Variable and constants definition
+--------------------------------------------------------------------------*/
const char TitleMsg[] = "\n Application Control Monitor\n";
const char InvalMsg[] = "\nInvalid command!";

struct 	command_d {
  void  (*cmd_fnct)(int, char**);
  char*	cmd_name;
  char*	cmd_help;
} const commands[] = {
  {cmd_sos,  "sos","                             help"},
  {cmd_sair, "sair","                            sair"},
  {cmd_ini,  "ini","<d>                          inicializar dispositivo (0/1) ser0/ser1"},
  {cmd_rc,   "rc","                              read clock"},
  //...
  {cmd_pr,   "pr","<[h1 m1 s1]> <[h2 m2 s2]>     process registers (max, min, mean) between instants t1 and t2 (h,m,s)"}
};

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

/*-------------------------------------------------------------------------+
| Function: cmd_sos - provides a rudimentary help
+--------------------------------------------------------------------------*/
void cmd_sos (int argc, char **argv)
{
  int i;

  printf("%s\n", TitleMsg);
  for (i=0; i<NCOMMANDS; i++)
    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
}

/*-------------------------------------------------------------------------+
| Function: getline        (called from monitor)
+--------------------------------------------------------------------------*/
int my_getline (char** argv, int argvsize)
{
  static char line[MAX_LINE];
  char *p;
  int argc;

  fgets(line, MAX_LINE, stdin);

  /* Break command line into an o.s. like argument vector,
     i.e. compliant with the (int argc, char **argv) specification --------*/

  for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL) return argc;
  }
  argv[argc] = p;
  return argc;
}

/*-------------------------------------------------------------------------+
| Function: monitor        (called from main)
+--------------------------------------------------------------------------*/
void monitor (cyg_mutex_t *cliblock)
{
  static char *argv[ARGVECSIZE+1], *p;
  int argc, i;

  cyg_mutex_lock(cliblock);
  printf("%s Type sos for help\n", TitleMsg);
  cyg_mutex_unlock(cliblock);
  for (;;) {
    cyg_mutex_lock(cliblock);
    printf("\nMyCmd> ");
    cyg_mutex_unlock(cliblock);
    /* Reading and parsing command line  ----------------------------------*/
    if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
      for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
      for (i = 0; i < NCOMMANDS; i++)
        if (strcmp(argv[0], commands[i].cmd_name) == 0)
          break;
      /* Executing commands -----------------------------------------------*/
      if (i < NCOMMANDS)
        commands[i].cmd_fnct (argc, argv);
      else
        cyg_mutex_lock(cliblock);
        printf("%s", InvalMsg);
        cyg_mutex_unlock(cliblock);
    } /* if my_getline */
  } /* forever */
}
