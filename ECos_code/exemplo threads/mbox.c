#include <cyg/kernel/kapi.h>
#include <stdio.h>

#define MAX 50
#define NT 1
#define PRI 8
#define STKSIZE 4096

char stack[NT][STKSIZE];

cyg_handle_t threadsH[NT];
cyg_thread threads[NT];

cyg_handle_t mbx1H, mbx2H;
cyg_mbox mbx1, mbx2;


/* thread code */
void thread_func(cyg_addrword_t data)
{
  char *m;

  while (1) {
    m = cyg_mbox_get( mbx1H );    // wait for message
    printf("m=%s\n", m);
    m[0]='R';                     // modify message
    cyg_mbox_put( mbx2H, m );     // send message back
  }
}

int main (void) 
{
  char msg[MAX];

  cyg_mbox_create( &mbx1H, &mbx1);
  cyg_mbox_create( &mbx2H, &mbx2);

  cyg_thread_create(PRI, thread_func, (cyg_addrword_t) 0,
		    "Thread", (void *) stack[0], STKSIZE,
		    &threadsH[0], &threads[0]);

  cyg_thread_resume(threadsH[0]);

  while (1) {
    char *m;

    printf("MSG a enviar: \n");
    fgets(msg, MAX, stdin);

    cyg_mbox_put( mbx1H, msg );

    m = cyg_mbox_get( mbx2H );
    printf("MSG recebida: %s\n", m);
    cyg_thread_delay(100);
  }

}
