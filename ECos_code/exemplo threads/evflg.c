#include <cyg/kernel/kapi.h>
#include <stdio.h>

#define MAX 50
#define NT 2
#define PRI 10
#define STKSIZE 4096

char stack[NT][STKSIZE];

cyg_handle_t threadsH[NT];
cyg_thread   threads[NT];
cyg_mutex_t  cliblock;
cyg_flag_t   ef;

char msg[MAX];


/* thread code */
void thread_func(cyg_addrword_t data)
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

int main (void) 
{
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
    //    efv = cyg_flag_wait(&ef, 0x03, CYG_FLAG_WAITMODE_OR | CYG_FLAG_WAITMODE_CLR);

    cyg_mutex_lock(&cliblock);
    printf("Main: evflag=%x\n", efv);
    cyg_mutex_unlock(&cliblock);

    //  cyg_thread_delay(100);
  }

}
