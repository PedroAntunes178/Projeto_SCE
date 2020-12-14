/***************************************************************************
| File: monitor.c
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include "project.h"


/*-------------------------------------------------------------------------+
| Variable and constants definition
+--------------------------------------------------------------------------*/
const char TitleMsg[] = "\nApplication Control Monitor\n";
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
  {cmd_sc,   "sc","<h> <m> <s>                   set clock"},
  {cmd_rtl,  "rtl","                             read temperature and luminosity"},
  {cmd_rp,   "rp","                              read parameters"},
  {cmd_mmp,  "mmp","<p>                          modify monitoring period"},
  {cmd_mta,  "mta","<t>                          modify time alarm"},
  {cmd_ra,   "ra","                              read alarms (clock, temp., luminosity, active/inactive)"},
  {cmd_dac,  "dac","<h> <m> <s>                  define alarm clock"},
  {cmd_dtl,  "dtl","<T> <L>                      define alarm temperature and luminosity"},
  {cmd_aa,   "aa","<a>                           activate/deactivate alarms (1/0)"},
  {cmd_ir,   "ir","                              information about registers (NREG, nr, iread, iwrite)"},
  {cmd_trc,  "trc","<n>                          transfer n registers from current iread position"},
  {cmd_tri,  "tri","<n> <i>                      transfer n registers from index i (0 - oldest)"},
  {cmd_irl,  "irl","                             information about local registers (NRBUF, nr, iread, iwrite)"},
  {cmd_lr,   "lr","<n> <i>                       list n registers (local memory) from index i (0 - oldest)"},
  {cmd_dr,   "dr","                              delete registers (local memory)"},
  {cmd_cpt,  "cpt","                             check period of transference"},
  {cmd_mpt,  "mpt","<p>                          modify period of transference (minutes - 0 deactivate)"},
  {cmd_cttl, "cttl","                            check threshold temperature and luminosity for processing"},
  {cmd_dttl, "dttl","                            define threshold temperature and luminosity for processing"},
  {cmd_pr,   "pr","<[h1 m1 s1 [h2 m2 s2]]>       process registers (max, min, mean) between instants t1 and t2 (h,m,s)"}
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
void monitor (void)
{
  static char *argv[ARGVECSIZE+1], *p;
  int argc, i;

  cyg_mutex_lock(&cliblock);
  printf("%sType sos for help\n", TitleMsg);
  cyg_mutex_unlock(&cliblock);
  for (;;) {
    cyg_mutex_lock(&cliblock);
    printf("\nMyCmd> ");
    cyg_mutex_unlock(&cliblock);
    /* Reading and parsing command line  ----------------------------------*/
    if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
      for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
      for (i = 0; i < NCOMMANDS; i++)
        if (strcmp(argv[0], commands[i].cmd_name) == 0)
          break;
      /* Executing commands -----------------------------------------------*/
      if (i < NCOMMANDS)
        commands[i].cmd_fnct (argc, argv);
      else{
        cyg_mutex_lock(&cliblock);
        printf("%s", InvalMsg);
        cyg_mutex_unlock(&cliblock);
      }
    } /* if my_getline */
  } /* forever */
}
