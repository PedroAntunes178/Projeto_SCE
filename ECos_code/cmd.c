/***************************************************************************
| File: cmd.c  -  Codigo principal da parte do pc (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include "project.h"

#define THREAD_READ_PRI 9
#define THREAD_CMD_PRI 10
#define THREAD_WRITE_PRI 8
#define THREAD_PROCESS_PRI 7
#define NUMBER_OF_THREADS 4 //two thread objects
#define STACKSIZE 4096 //4K stacks


/* now declare (and allocate space for) some kernel objects, like the two threads we will use */
cyg_thread thread_s[NUMBER_OF_THREADS];	/* space for thread objects */

char stack[NUMBER_OF_THREADS][STACKSIZE];	/* space for two stacks */

/* now the handles for the threads */
cyg_handle_t cmd_thread, read_thread, write_thread, process_thread;

/* and now variables for the procedure which is the thread */
cyg_thread_entry_t cmd_program, read_program, write_program, process_program;

/* we install our own startup routine which sets up threads */
void cyg_user_start(void){
  printf("Entrou no programa do ECos ->\n");

  cyg_mutex_init(&cliblock);
  cmd_ini(0, NULL);

  cyg_mbox_create( &mbx1H, &mbx1);
  cyg_mbox_create( &mbx2H, &mbx2);

  cyg_thread_create(THREAD_CMD_PRI, cmd_program, (cyg_addrword_t) 0,
  "Thread CMD", (void *) stack[0], STACKSIZE,
  &cmd_thread, &thread_s[0]);
  cyg_thread_create(THREAD_READ_PRI, read_program, (cyg_addrword_t) 1,
  "Thread READ", (void *) stack[1], STACKSIZE,
  &read_thread, &thread_s[1]);
  cyg_thread_create(THREAD_WRITE_PRI, write_program, (cyg_addrword_t) 2,
  "Thread WRITE", (void *) stack[2], STACKSIZE,
  &write_thread, &thread_s[2]);
  cyg_thread_create(THREAD_PROCESS_PRI, process_program, (cyg_addrword_t) 3,
  "Thread PROCESS", (void *) stack[3], STACKSIZE,
  &process_thread, &thread_s[3]);

  cyg_thread_resume(cmd_thread);
  cyg_thread_resume(read_thread);
  cyg_thread_resume(write_thread);
  cyg_thread_resume(process_thread);

}

/* this is a simple program which runs in a thread */
void cmd_program(cyg_addrword_t data){
  while(1){
    monitor();
  }
}

/* this is a simple program which runs in a thread */
void read_program(cyg_addrword_t data){
  //int message = (int) data;
  unsigned char buff[8];
  unsigned char c;
  int flag = 0;
  int buff_index = 0;
  unsigned int n = 1;

  while(1){
    err = cyg_io_read(serH, &c, &n);
    cyg_mutex_lock(&cliblock);
    printf("io_read err=%x, n=%d\n", err, n);
    cyg_mutex_unlock(&cliblock);
    if (c == SOM ){
      flag = 1;
      buff_index = 0;
    }
    else if (flag == 1 && c == EOM){
      flag = 1;
      cyg_mbox_put( mbx2H, buff );
    }
    else if (flag == 1){
      buff[buff_index]=c;
      buff_index++;
    }
  }
}


void write_program(cyg_addrword_t data){
  char *bufw;
  unsigned int n;
  printf("debug\n");
  while (1) {
    printf("debug\n");
    bufw = cyg_mbox_get( mbx1H );    // wait for message
    n = (unsigned char)sizeof(bufw);
    err = cyg_io_write(serH, bufw, &n);
    cyg_mutex_lock(&cliblock);
    printf("io_write err=%x, n=%d\n", err, n);
    cyg_mutex_unlock(&cliblock);
  }
}

void process_program(cyg_addrword_t data){
  char *buffer_process;
  unsigned int n;

  while (1) {
    buffer_process = cyg_mbox_get( mbx2H );    // wait for message
    n = (unsigned char)sizeof(buffer_process);
    if (buffer_process[0] == RCLK ){
      cyg_mutex_lock(&cliblock);
      printf("Received RC!\n");
      cyg_mutex_unlock(&cliblock);
    }
    else if (buffer_process[0] == NMFL ){
      cyg_mutex_lock(&cliblock);
      printf("Memory half full!\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
}
