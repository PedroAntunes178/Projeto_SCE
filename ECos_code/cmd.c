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

/* and now a mutex to protect calls to the C library */
cyg_mutex_t cliblock;

/*Variaveis globais para escrever e ler do PIC16*/
Cyg_ErrNo err;
cyg_io_handle_t serH;

/*Variaveis globais correspondentes a mail box. mbx1 é a mail box  */
cyg_handle_t mbx1H, mbx2H;
cyg_mbox mbx1, mbx2;

/*Register related variables*/
unsigned int n_reg = 0;
unsigned char registers[NRBUF][5];
int iread = 0;
int iwrite = 0;

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
  unsigned char buff[128];
  unsigned char c;
  unsigned int flag = 0;
  int buff_index = 0;
  unsigned int n = 1;

  while(1){
    err = cyg_io_read(serH, &c, &n);
    if (c == SOM ){
      flag = 1;
      buff_index = 0;
    }
    else if (c == EOM){
      flag = 0;
      read_buffer(buff);
    }
    else if (flag == 1){
      buff[buff_index]=c;
      buff_index++;
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("Received not a valid char.\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
}


void write_program(cyg_addrword_t data){
  unsigned char *bufw;
  unsigned int n;
  while (1) {
    bufw = cyg_mbox_get( mbx1H );    // wait for message
    n = (unsigned int)bufw[0];

    err = cyg_io_write(serH, bufw, &n);
    cyg_mutex_lock(&cliblock);
    printf("io_write err=%x, n=%d\n", err, n);
    cyg_mutex_unlock(&cliblock);
  }
}

void process_program(cyg_addrword_t data){
  unsigned char *buffer_process;
  unsigned int n;
  int i =0;

  while (1) {
    buffer_process = cyg_mbox_get( mbx2H );    // wait for message
    n = (unsigned int)sizeof(buffer_process);
    /*
    int i=0;
    cyg_mutex_lock(&cliblock);
    printf("debug, %d!\n", n);
    cyg_mutex_unlock(&cliblock);
    for(i=0;i<n;i++){
      cyg_mutex_lock(&cliblock);
      printf("debug, %x!\n", buffer_process[i]);
      cyg_mutex_unlock(&cliblock);
    }*/

  }
}

void read_buffer(unsigned char *buffer) {
  int i =0;

  if (buffer[0] == RCLK ){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTime: %d:%d:%d\n", buffer[1], buffer[2], buffer[3]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == SCLK){
    if (buffer[1] == CMD_OK){
      cyg_mutex_lock(&cliblock);
      printf("\nTime setup completo.\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == RTL){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTemprature: %d\nLuminosity:%d\n", buffer[1], buffer[2]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == RPAR){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nPMON: %d\nTALA:%d\n", buffer[1], buffer[2]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == MMP){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nMonitoring period modification complete.\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == MTA){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTime alarm modification complete.\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == RALA){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nAlarm Clock time: %d:%d:%d\n", buffer[1], buffer[2], buffer[3]);
      printf("Alarm Temperature: %d\n", buffer[4]);
      printf("Alarm luminosity: %d\n", buffer[5]);
      if(buffer[6]==1) printf("Alarms are active\n");
      else printf("Alarms are deactivated\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == DAC){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nAlarm clock defined.\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == DATL){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTemprature and luminosity alarm defined.\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == AALA){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nActivated alarm.\n");
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == IREG){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nRing Buffer size on PIC side (NREG): %d\n", buffer[1]);
      printf("Number of valid registers on PIC (nr):%d\n", buffer[2]);
      printf("Last register index read (iread):%d\n", buffer[3]);
      printf("Last register index still not transfered (iwrite):%d\n", buffer[4]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
      iwrite = (int)(buffer[4]-buffer[3]);
      if (iwrite<0) iwrite = iwrite + (int)(buffer[1]);
    }
  }
  else if (buffer[0] == TRGC){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      n_reg = buffer[1];
      for(i=0;i<n_reg;i++){
        if(iread+i>NRBUF) iread = iread - NRBUF;

        cyg_mutex_lock(&cliblock);
        printf("\n%dº registo escrito\n", iread+i);
        printf("Time: %d:%d:%d\n", buffer[i*5+1+1], buffer[i*5+1+2], buffer[i*5+1+3]);
        printf("Temprature: %d\nLuminosity:%d\n", buffer[i*5+1+4], buffer[i*5+1+5]);
        printf("MyCmd>\n");
        cyg_mutex_unlock(&cliblock);

      }
      iread = iread+i;
      iwrite = iwrite-i;
      if (iwrite < 0) iwrite = 0;
    }
  }
  else if (buffer[0] == TRGI){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>\n");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      n_reg = buffer[1];
      for(i=0;i<n_reg;i++){
        cyg_mutex_lock(&cliblock);
        printf("\n%dº registo obtido\n", i+1);
        printf("Time: %d:%d:%d\n", buffer[i*5+1], buffer[i*5+2], buffer[i*5+3]);
        printf("Temprature: %d\nLuminosity:%d\n", buffer[i*5+4], buffer[i*5+5]);
        printf("MyCmd>\n");
        cyg_mutex_unlock(&cliblock);
      }
    }
  }
  else if (buffer[0] == NMFL ){
    cyg_mutex_lock(&cliblock);
    printf("Memory half full!\n");
    cyg_mutex_unlock(&cliblock);
  }
}
