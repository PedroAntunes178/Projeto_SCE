#include <cyg/kernel/kapi.h>
#include <stdio.h>

#define MAX 50
#define NT 2
#define PRI 9
#define STKSIZE 4096

char stack[NT][STKSIZE];

cyg_handle_t threadsH[NT];
cyg_thread   threads[NT];
cyg_mutex_t  cliblock;

char msg[MAX];

/* thread code */
void thread_func(cyg_addrword_t data)
{
  int i = (int)data;
  
  while (1) {
    cyg_mutex_lock(&cliblock);
    printf("Thread: %d\n", i);
    cyg_mutex_unlock(&cliblock);
    cyg_thread_delay(100-i);
  }
}

int main (void) 
{
  int i;
  cyg_priority_t p;

  p = cyg_thread_get_priority( cyg_thread_self() );
  printf("Main: pri=%d\n", p);

  cyg_mutex_init(&cliblock);

  for(i=0; i<NT; i++) {
    cyg_thread_create(PRI+i, thread_func, (cyg_addrword_t) i,
		      "Thread", (void *) stack[i], STKSIZE,
		      &threadsH[i], &threads[i]);
    cyg_thread_resume(threadsH[i]);
  }

  while (1) {
    cyg_mutex_lock(&cliblock);
    printf("MSG: \n");
    cyg_mutex_unlock(&cliblock);

    fgets(msg, MAX, stdin);

    cyg_thread_delay(100);
  }

}
