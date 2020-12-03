/***************************************************************************
| File: cmd.c  -  Codigo principal da parte do pc (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

extern void cmd_ini (int, char** );
extern void monitor(void);

#define THREAD_READ_PRI 9
#define THREAD_CMD_PRI 10
#define NUMBER_OF_THREADS 2 //two thread objects
#define STACKSIZE 4096 //4K stacks

/* now declare (and allocate space for) some kernel objects,
  like the two threads we will use */
cyg_thread thread_s[NUMBER_OF_THREADS];	/* space for thread objects */

char stack[NUMBER_OF_THREADS][STACKSIZE];	/* space for two stacks */

/* now the handles for the threads */
cyg_handle_t cmd_thread, read_thread;

/* and now variables for the procedure which is the thread */
cyg_thread_entry_t cmd_program, read_program;

/* and now a mutex to protect calls to the C library */
cyg_mutex_t cliblock;

/* we install our own startup routine which sets up threads */
void cyg_user_start(void)
{
  printf("Entering twothreads' cyg_user_start() function\n");

  cyg_mutex_init(&cliblock);
  cmd_ini(0, NULL);

  cyg_thread_create(THREAD_CMD_PRI, cmd_program, (cyg_addrword_t) 0,
  "Thread CMD", (void *) stack[0], STACKSIZE,
  &cmd_thread, &thread_s[0]);
  cyg_thread_create(THREAD_READ_PRI, read_program, (cyg_addrword_t) 1,
  "Thread READ", (void *) stack[1], STACKSIZE,
  &read_thread, &thread_s[1]);

  cyg_thread_resume(cmd_thread);
  cyg_thread_resume(read_thread);
  while(1){

  }
}

/* this is a simple program which runs in a thread */
void cmd_program(cyg_addrword_t data){
  cyg_mutex_lock(&cliblock);
  monitor();
  cyg_mutex_unlock(&cliblock);
}
/* this is a simple program which runs in a thread */
void read_program(cyg_addrword_t data)
{
  int message = (int) data;
  int delay;

  printf("Beginning execution; thread data is %d\n", message);

  cyg_thread_delay(200);

  for (;;) {
    delay = 200 + (rand() % 50);

    /* note: printf() must be protected by a call to cyg_mutex_lock() */
    cyg_mutex_lock(&cliblock);
    printf("Thread %d: and now a delay of %d clock ticks\n", message, delay);
    cyg_mutex_unlock(&cliblock);
    cyg_thread_delay(delay);
  }
}
