/***************************************************************************
| File: cmd.c  -  Codigo principal da parte do pc (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include <stdio.h>

extern void cmd_ini (int, char** );
extern void monitor(void);

int main(void)
{
  cmd_ini(0, NULL);
  monitor();

  return 0;
}
