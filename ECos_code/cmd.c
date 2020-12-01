/***************************************************************************
| File: cmd.c  -  Codigo principal da parte do pc (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <stdio.h>

#include <cyg/kernel/kapi.h>

// These numbers depend entirely on our application
#define MAX 50
#define NT 2
#define READ_PRI 9
#define CMD_PRI 10
#define READ_STKSIZE CYGNUM_HAL_STACK_SIZE_TYPICAL
#define CMD_STACKSIZE    (CYGNUM_HAL_STACK_SIZE_MINIMUM + 1024)

extern void cmd_ini (int, char** );
extern void monitor(void);


static unsigned char read_stack[READ_STKSIZE];
static unsigned char cmd_stacks[NT][CMD_STACKSIZE];
static cyg_handle_t read_handle, cmd_handles[NT];
static cyg_thread_t read_thread, cmd_threads[NT];

cyg_mutex_t  cliblock;

static unsigned char msg[MAX];

void read_func(cyg_addrword_t);
void cmd_func(cyg_addrword_t);

int main(void)
{
  cmd_ini(0, NULL);
  monitor();

  /*
  int i;
  cyg_flag_value_t efv;

  cyg_flag_init(&ef);
  cyg_mutex_init(&cliblock);

  for(i=0; i<NT; i++) {
    cyg_thread_create(PRI+i, thread_func, (cyg_addrword_t) i,
          "Thread", (void *) stack[i], STKSIZE,
          &threadsH[i], &threads[i]);
    cyg_thread_resume(threadsH[i]);
  }

  while (1) {
    cyg_mutex_lock(&cliblock);
    printf("Main: evflag wait\n");
    cyg_mutex_unlock(&cliblock);

    // efv = cyg_flag_wait(&ef, 0x03, CYG_FLAG_WAITMODE_AND);
    efv = cyg_flag_wait(&ef, 0x03, CYG_FLAG_WAITMODE_AND | CYG_FLAG_WAITMODE_CLR);
    // efv = cyg_flag_wait(&ef, 0x03, CYG_FLAG_WAITMODE_OR | CYG_FLAG_WAITMODE_CLR);

    cyg_mutex_lock(&cliblock);
    printf("Main: evflag=%x\n", efv);
    cyg_mutex_unlock(&cliblock);

    //  cyg_thread_delay(100);
  }*/

  return 0;
}


/* thread code */
void read_func(cyg_addrword_t data)
{
  int i = (int)data;
  cyg_flag_value_t efv=0x01;

  while (1) {
    cyg_mutex_lock(&cliblock);
    printf("Thread: %d\n", i);
    cyg_mutex_unlock(&cliblock);
    cyg_flag_setbits(&ef, (efv << i));
    cyg_thread_delay(100 - (50*i));
  }
}

void cmd_func(cyg_addrword_t data)
{
  int i = (int)data;
  cyg_flag_value_t efv=0x01;

  while (1) {
    cyg_mutex_lock(&cliblock);
    printf("Thread: %d\n", i);
    cyg_mutex_unlock(&cliblock);
    cyg_flag_setbits(&ef, (efv << i));
    cyg_thread_delay(100 - (50*i));
  }
}
