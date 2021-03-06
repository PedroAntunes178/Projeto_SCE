/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida Rubio (IST98319)
| Data:  Dezembro 2020
***************************************************************************/
#include "project.h"


/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/
void cmd_sair (int argc, char **argv)
{
  exit(0);
}


/*-------------------------------------------------------------------------+
| Function: cmd_ini - inicializar dispositivo
+--------------------------------------------------------------------------*/
void cmd_ini(int argc, char **argv)
{
  printf("Initialising communication\n");
  printf("io_lookup\n");
  if ((argc > 1) && (argv[1][0] = '1'))
    err = cyg_io_lookup("/dev/ser1", &serH);
  else err = cyg_io_lookup("/dev/ser0", &serH);
  printf("lookup err=%x\n", err);
}

/*-------------------------------------------------------------------------+
| Function: cmd_rc  - read clock
+--------------------------------------------------------------------------*/
void cmd_rc(int argc, char** argv){
  unsigned char x[] = {0, SOM, RCLK, CMD_OK, EOM};
  x[0] = sizeof(x);
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_sc  - set clock
+-----------------------------------------------------------------------------------------------------*/
void cmd_sc(int argc, char** argv){

  if(argc == 4){
    unsigned char x[] = {0, SOM, SCLK, 0, 0, 0, EOM};
    x[0] = sizeof(x);

    x[3] = (unsigned char)atoi(argv[1]);
    x[4] = (unsigned char)atoi(argv[2]);
    x[5] = (unsigned char)atoi(argv[3]);
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_rtl  - read temperature and luminosity
+-----------------------------------------------------------------------------------------------------*/
void cmd_rtl(int argc, char** argv){
  unsigned char x[] = {0, SOM, RTL, CMD_OK, EOM};
  x[0] = sizeof(x);
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_rp  - read parameters (PMON, TALA)
+-----------------------------------------------------------------------------------------------------*/
void cmd_rp(int argc, char** argv){
  unsigned char x[] = {0, SOM, RPAR, CMD_OK, EOM};
  x[0] = sizeof(x);
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mmp  - modify monitoring period (seconds - 0 deactivate)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mmp(int argc, char** argv){
  unsigned char x[] = {0, SOM, MMP, 0, EOM};
  x[0] = sizeof(x);
  if(argc == 2){
    x[3] = (unsigned char)atoi(argv[1]);
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mta  - modify time alarm (seconds)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mta(int argc, char** argv){
  unsigned char x[] = {0, SOM, MTA, 0, EOM};
  x[0] = sizeof(x);
  if(argc == 2){
    x[3] = (unsigned char)atoi(argv[1]);
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_ra  - read alarms (clock, temperature, luminosity, active/inactive-1/0)
+-----------------------------------------------------------------------------------------------------*/
void cmd_ra(int argc, char** argv){
  unsigned char x[] = {0, SOM, RALA, CMD_OK, EOM};
  x[0] = sizeof(x);
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dac  - define alarm clock
+-----------------------------------------------------------------------------------------------------*/
void cmd_dac(int argc, char** argv){
  if(argc == 4){
    unsigned char x[] = {0, SOM, DAC, 0, 0, 0, EOM};
    x[0] = sizeof(x);

    x[3] = (unsigned char)atoi(argv[1]);
    x[4] = (unsigned char)atoi(argv[2]);
    x[5] = (unsigned char)atoi(argv[3]);
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}
/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dtl  - define alarm temperature and luminosity
+-----------------------------------------------------------------------------------------------------*/
void cmd_dtl(int argc, char** argv){
  if(argc == 3){
    unsigned char x[] = {0, SOM, DATL, 0, 0, EOM};
    x[0] = sizeof(x);
    x[3] = (unsigned char)atoi(argv[1]);
    x[4] = (unsigned char)atoi(argv[2]);
    cyg_mbox_put( mbx1H, x);
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_aa  - activate/deactivate alarms (1/0)
+-----------------------------------------------------------------------------------------------------*/
void cmd_aa(int argc, char** argv){
  unsigned char x[] = {0, SOM, AALA, 0, EOM};
  x[0] = sizeof(x);
  if(argc == 2){
    x[3] = (unsigned char)atoi(argv[1]);
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_ir  - information about registers (NREG, nr, iread, iwrite)
+-----------------------------------------------------------------------------------------------------*/
void cmd_ir(int argc, char** argv){
  unsigned char x[] = {0, SOM, IREG, CMD_OK, EOM};
  x[0] = sizeof(x);
  cyg_mbox_put( mbx1H, x );

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_trc  - transfer n registers from current iread position
+-----------------------------------------------------------------------------------------------------*/
void cmd_trc(int argc, char** argv){
  unsigned char x[] = {0, SOM, TRGC, 0, EOM};
  x[0] = sizeof(x);
  if(argc == 2){
    x[3] = (unsigned char)atoi(argv[1]);
    n_reg = (unsigned int)x[3];
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
  /*
  cyg_mutex_lock(&cliblock);
  printf("Debug %d.\n", n_reg);
  cyg_mutex_unlock(&cliblock);*/

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_tri  - transfer n registers from index i (0 - oldest)
+-----------------------------------------------------------------------------------------------------*/
void cmd_tri(int argc, char** argv){
  unsigned char x[] = {0, SOM, TRGI, 0, 0, EOM};
  x[0] = sizeof(x);
  if(argc == 3){
    x[3] = (unsigned char)atoi(argv[1]);
    n_reg = (unsigned int)x[3];
    x[4] = (unsigned char)atoi(argv[2]);
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
  /*
  cyg_mutex_lock(&cliblock);
  printf("Debug %d.\n", n_reg);
  cyg_mutex_unlock(&cliblock);*/
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_irl  - information about local registers (NRBUF, nr, iread, iwrite)
+-----------------------------------------------------------------------------------------------------*/
void cmd_irl(int argc, char** argv){

  cyg_mutex_lock(&cliblock);
  printf("Number of registers in ring buffer (NRBUF): %d\n", NRBUF);
  printf("Number of valid registers in PC: %d\n", ng);
  if(iread==-1) printf("No register read by the PC.\n");
  else printf("Last register read index: %d\n", iread);
  if(iwrite==-1) printf("No register wrote from PIC to PC.\n");
  else printf("Last register wrote index: %d\n", iwrite);
  cyg_mutex_unlock(&cliblock);
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_lr  - list n registers (local memory) from index i (0 - oldest)
+-----------------------------------------------------------------------------------------------------*/
void cmd_lr(int argc, char** argv){
  int k = 0;
  int k_true = 0;
  int n = 0;
  int i = 0;

  if((argc == 3)||(argc == 2)){
    n = atoi(argv[1]);
    if(argc == 3) i = atoi(argv[2]);
    else i = iread;
    if(i==-1) i=0;
    if((ng==NRBUF)&&(i==0)){
      i = iwrite + 1;
    }
    if(i==NRBUF) i = 0;
    if(i>ng){
      i=0;
      cyg_mutex_lock(&cliblock);
      printf("Index asked doen't have a resgister saved.\n Printing registers from index 0.\n");
      cyg_mutex_unlock(&cliblock);
    }
    if((i+n>ng) && (ng!=NRBUF)){
      cyg_mutex_lock(&cliblock);
      printf("There aren't %d registers from index %d to oldest.\n Printing %d registers.\n", n, i, ng-i);
      cyg_mutex_unlock(&cliblock);
      n=ng-i;
    }
    if((iwrite<i) && (i+n>=NRBUF)) iwrite = iwrite+NRBUF;
    for(k=i; k<=iwrite && k<i+n; k++){
      if (k>=NRBUF){
        iwrite = iwrite-NRBUF;
        i = i-NRBUF;
        k = k-NRBUF;
      }
      else k_true = k;
      cyg_mutex_lock(&cliblock);
      printf("Register index: %d\nTime: %d:%d:%d, Temperature: %d, Luminosity: %d\n", k_true, registers[k_true][0], registers[k_true][1], registers[k_true][2], registers[k_true][3], registers[k_true][4]);
      cyg_mutex_unlock(&cliblock);
    }
    if(i+n>iread) iread = i+n-1;
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dr  - delete registers (local memory)
+-----------------------------------------------------------------------------------------------------*/
void cmd_dr(int argc, char** argv){
  iread = -1;
  iwrite = -1;
  ng = 0;
  cyg_mutex_lock(&cliblock);
  printf("Deleted Registers.\n");
  cyg_mutex_unlock(&cliblock);
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_cpt  - check period of transference
+-----------------------------------------------------------------------------------------------------*/
void cmd_cpt(int argc, char** argv){
  unsigned char x = 123;
  cyg_mbox_put( mbx2H, &x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mpt  - modify period of transference (minutes - 0 deactivate)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mpt(int argc, char** argv){
  unsigned char x = 124;
  if(argc == 2){
    cyg_mbox_put( mbx2H, &x );
    cyg_mbox_put( mbx2H, argv[1]);
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_cttl  - check threshold temperature and luminosity for processing
+-----------------------------------------------------------------------------------------------------*/
void cmd_cttl(int argc, char** argv){
  unsigned char x = 125;
  cyg_mbox_put( mbx2H, &x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dttl  - define threshold temperature and luminosity for processing
+-----------------------------------------------------------------------------------------------------*/
void cmd_dttl(int argc, char** argv){
  unsigned char x = 126;
  if(argc == 3){
    cyg_mbox_put( mbx2H, &x );
    cyg_mbox_put( mbx2H, argv[1]);
    cyg_mbox_put( mbx2H, argv[2]);
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_pr  - process registers (max, min, mean) between instants t1 and t2 (h,m,s)
+-----------------------------------------------------------------------------------------------------*/
void cmd_pr(int argc, char** argv){
  unsigned char x = 127;
  int time_min = 0;
  int time_max = 0;

  if(argc == 7){
    time_min = atoi(argv[1])*60*60+atoi(argv[2])*60+atoi(argv[3])-1;
    time_max = atoi(argv[4])*60*60+atoi(argv[5])*60+atoi(argv[6])+1;
    cyg_mbox_put( mbx2H, &x );
    cyg_mbox_put( mbx2H, &time_min);
    cyg_mbox_put( mbx2H, &time_max);
  }
  else if(argc == 4){
    time_min = atoi(argv[1])*60*60+atoi(argv[2])*60+atoi(argv[3])-1;
    time_max = 24*60*60;
    cyg_mbox_put( mbx2H, &x );
    cyg_mbox_put( mbx2H, &time_min);
    cyg_mbox_put( mbx2H, &time_max);
  }
  else if(argc == 1){
    time_min = 0;
    time_max = 24*60*60;
    cyg_mbox_put( mbx2H, &x );
    cyg_mbox_put( mbx2H, &time_min);
    cyg_mbox_put( mbx2H, &time_max);
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
}
