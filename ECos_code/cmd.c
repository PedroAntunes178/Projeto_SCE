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
  monitor();
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
      printf("Received not a valid char. Ignored.\n");
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
  cyg_handle_t counterH, system_clockH, alarmH;
  cyg_alarm alarm;
  unsigned char variable = 0;
  unsigned char *bufw;
  int transfer_period = 6000;
  int threshold_temperature = 0;
  int threshold_luminosity = 0;
  int max = 0;
  int min = 0;

  system_clockH = cyg_real_time_clock();
  cyg_clock_to_counter(system_clockH, &counterH);
  cyg_alarm_create(counterH, alarm_func, (cyg_addrword_t) &variable, &alarmH, &alarm);
  cyg_alarm_initialize(alarmH, cyg_current_time()+transfer_period, transfer_period);

  for (;;) {
    bufw = cyg_mbox_get( mbx2H );    // wait for message

    if(bufw[0] == '1'){
      cyg_mutex_lock(&cliblock);
      printf("\nPeriod of transference: %d\n", transfer_period);
      cyg_mutex_unlock(&cliblock);
    }
    else if (bufw[0] == '2'){
      cyg_alarm_delete(alarmH);
      bufw = cyg_mbox_get( mbx2H );
      transfer_period = atoi((char *)bufw);
      if (transfer_period !=0) cyg_alarm_initialize(alarmH, cyg_current_time()+transfer_period, transfer_period);
      cyg_mutex_lock(&cliblock);
      printf("\nModified period of transference: %d\n", transfer_period);
      cyg_mutex_unlock(&cliblock);
    }
    else if (bufw[0] == '3'){
      cyg_mutex_lock(&cliblock);
      printf("\nThreshold temperature: %d\n", threshold_temperature);
      printf("Threshold luminosity: %d\n", threshold_luminosity);
      cyg_mutex_unlock(&cliblock);
    }
    else if (bufw[0] == '4'){
      bufw = cyg_mbox_get( mbx2H );
      threshold_temperature = atoi((char *)bufw);
      bufw = cyg_mbox_get( mbx2H );
      threshold_luminosity = atoi((char *)bufw);
    }
    else if (bufw[0] == '5'){
      bufw = cyg_mbox_get( mbx2H );
      min = atoi((char *)bufw);
      bufw = cyg_mbox_get( mbx2H );
      max = atoi((char *)bufw);
      process_registers(max, min);
    }
  }
}
/* test_alarm_func() is invoked as an alarm handler, so
   it should be quick and simple. in this case it increments
   the data that is passed to it. */
void alarm_func(cyg_handle_t alarmH, cyg_addrword_t data){
  ++*((unsigned *) data);
  cyg_mutex_lock(&cliblock);
  printf("\nAsked for Registers\n");
  cyg_mutex_unlock(&cliblock);
  unsigned char x[] = {5, SOM, TRGC, 25, EOM};
  cyg_mbox_put( mbx1H, x );
}

void process_registers(int max, int min) {
  int i=0, k=0;
  int time_s = 0;
  int max_t=0, max_l=0, min_t=100, min_l=100, som_t=0, som_l=0;

  for(i=0; (i<iread) && (i<NRBUF);i++){
    time_s = registers[0][0]*60*60+registers[0][1]*60+registers[0][2];
    if((time_s>min) && (time_s<max) && (time_s!=0)){
      k++;
      som_t = som_t + registers[i][3];
      som_l = som_l + registers[i][4];
      if(max_t<registers[i][3]) max_t=registers[i][3];
      if(max_l<registers[i][4]) max_l=registers[i][4];
      if(min_t>registers[i][3]) min_t=registers[i][3];
      if(min_l>registers[i][4]) min_l=registers[i][4];
    }
  }
  cyg_mutex_lock(&cliblock);
  printf("\nTemperature: max = %d; min = %d; mean = %d.\n", max_t, min_t, som_t/k);
  printf("\nLuminosity: max = %d; min = %d; mean = %d.\n", max_l, min_l, som_l/k);
  printf("\nMyCmd>");
  cyg_mutex_unlock(&cliblock);
}

void read_buffer(unsigned char *buffer) {
  int i =0;

  if (buffer[0] == RCLK ){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTime: %d:%d:%d\n", buffer[1], buffer[2], buffer[3]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == SCLK){
    if (buffer[1] == CMD_OK){
      cyg_mutex_lock(&cliblock);
      printf("\nTime setup completo.\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == RTL){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTemprature: %d\nLuminosity:%d\n", buffer[1], buffer[2]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == RPAR){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nPMON: %d\nTALA:%d\n", buffer[1], buffer[2]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == MMP){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nMonitoring period modification complete.\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == MTA){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTime alarm modification complete.\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == RALA){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nAlarm Clock time: %d:%d:%d\n", buffer[1], buffer[2], buffer[3]);
      printf("Alarm Temperature: %d\n", buffer[4]);
      printf("Alarm luminosity: %d\n", buffer[5]);
      if(buffer[6]==1) printf("Alarms are active\n");
      else printf("Alarms are deactivated\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == DAC){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nAlarm clock defined.\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
  }
  else if (buffer[0] == DATL){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nTemprature and luminosity alarm defined.\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == AALA){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nActivated alarm.\n");
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
  }
  else if (buffer[0] == IREG){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("\nRing Buffer size on PIC side (NREG): %d\n", buffer[1]);
      printf("Number of valid registers on PIC (nr):%d\n", buffer[2]);
      printf("Last register index read (iread):%d\n", buffer[3]);
      printf("Last register index still not transfered (iwrite):%d\n", buffer[4]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
      iwrite = (int)(buffer[4]-buffer[3]);
      if (iwrite<0) iwrite = iwrite + (int)(buffer[1]);
    }
  }
  else if (buffer[0] == TRGC){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);

    }
    else{
      n_reg = buffer[1];
      for(i=0;i<n_reg;i++){
        if(iread+i>NRBUF) iread = iread - NRBUF;
        registers[iread+i][0]=buffer[i*5+1+1];
        registers[iread+i][1]=buffer[i*5+1+2];
        registers[iread+i][2]=buffer[i*5+1+3];
        registers[iread+i][3]=buffer[i*5+1+4];
        registers[iread+i][4]=buffer[i*5+1+5];
        cyg_mutex_lock(&cliblock);
        printf("\nIndex %d register:\n", iread+i);
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
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      n_reg = buffer[1];
      for(i=0;i<n_reg;i++){
        if(iread+i>NRBUF) iread = iread - NRBUF;
        registers[iread+i][0]=buffer[i*5+2+1];
        registers[iread+i][1]=buffer[i*5+2+2];
        registers[iread+i][2]=buffer[i*5+2+3];
        registers[iread+i][3]=buffer[i*5+2+4];
        registers[iread+i][4]=buffer[i*5+2+5];

        cyg_mutex_lock(&cliblock);
        printf("\nIndex %d register:\n", iread+i);
        printf("Time: %d:%d:%d\n", buffer[i*5+2+1], buffer[i*5+2+2], buffer[i*5+2+3]);
        printf("Temprature: %d\nLuminosity:%d\n", buffer[i*5+2+4], buffer[i*5+2+5]);
        printf("MyCmd>\n");
        cyg_mutex_unlock(&cliblock);

      }
    }
  }
  else if (buffer[0] == NMFL ){
    if (buffer[1] == CMD_ERROR){
      cyg_mutex_lock(&cliblock);
      printf("\nError from PIC side in command %x\n", buffer[0]);
      printf("\nMyCmd>");
      cyg_mutex_unlock(&cliblock);
    }
    else{
      cyg_mutex_lock(&cliblock);
      printf("Memory half full!\n");
      cyg_mutex_unlock(&cliblock);
      unsigned char x[] = {0, SOM, TRGC, 25, EOM};
      cyg_mbox_put( mbx1H, x );
    }
  }
}
