/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
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
  unsigned char x[] = {SOM, RCLK, CMD_OK, EOM};
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_sc  - set clock
+-----------------------------------------------------------------------------------------------------*/
void cmd_sc(int argc, char** argv){
  unsigned char x[] = {SOM, SCLK, 0, 0, 0, EOM};
  if(argc == 4){
    x[2] = *argv[1];
    x[3] = *argv[2];
    x[4] = *argv[3];
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
  unsigned char x[] = {SOM, RTL, CMD_OK, EOM};
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_rp  - read parameters (PMON, TALA)
+-----------------------------------------------------------------------------------------------------*/
void cmd_rp(int argc, char** argv){
  unsigned char x[] = {SOM, RPAR, CMD_OK, EOM};
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mmp  - modify monitoring period (seconds - 0 deactivate)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mmp(int argc, char** argv){
  unsigned char x[] = {SOM, SCLK, 0, EOM};
  if(argc == 2){
    x[2] = *argv[1];
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
  unsigned char x[] = {SOM, SCLK, 0, EOM};
  if(argc == 2){
    x[2] = *argv[1];
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
  unsigned char x[] = {SOM, RALA, CMD_OK, EOM};
  cyg_mbox_put( mbx1H, x );
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dac  - define alarm clock
+-----------------------------------------------------------------------------------------------------*/
void cmd_dac(int argc, char** argv){
  unsigned char x[] = {SOM, DAC, 0, 0, 0, EOM};
  if(argc == 4){
    x[2] = *argv[1];
    x[3] = *argv[2];
    x[4] = *argv[3];
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
  unsigned char x[] = {SOM, DATL, 0, 0, EOM};
  if(argc == 3){
    x[2] = *argv[1];
    x[3] = *argv[2];
    cyg_mbox_put( mbx1H, x );
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
  unsigned char x[] = {SOM, AALA, 0, EOM};
  if(argc == 2){
    x[2] = *argv[1];
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
  unsigned char x[] = {SOM, IREG, CMD_OK, EOM};
  cyg_mbox_put( mbx1H, x );

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_trc  - transfer n registers from current iread position
+-----------------------------------------------------------------------------------------------------*/
void cmd_trc(int argc, char** argv){
  unsigned char x[] = {SOM, TRGC, 0, EOM};
  if(argc == 2){
    x[2] = *argv[1];
    n_reg = (int)x[2];
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
  cyg_mutex_lock(&cliblock);
  printf("Debug %d.\n", n_reg);
  cyg_mutex_unlock(&cliblock);

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_tri  - transfer n registers from index i (0 - oldest)
+-----------------------------------------------------------------------------------------------------*/
void cmd_tri(int argc, char** argv){
  unsigned char x[] = {SOM, TRGI, 0, 0, EOM};
  if(argc == 3){
    x[2] = *argv[1];
    n_reg = (int)x[2];
    x[3] = *argv[2];
    cyg_mbox_put( mbx1H, x );
  }
  else{
    cyg_mutex_lock(&cliblock);
    printf("Not the right number of parameters were given.\n");
    cyg_mutex_unlock(&cliblock);
  }
  cyg_mutex_lock(&cliblock);
  printf("Debug %d.\n", n_reg);
  cyg_mutex_unlock(&cliblock);

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_irl  - information about local registers (NRBUF, nr, iread, iwrite)
+-----------------------------------------------------------------------------------------------------*/
void cmd_irl(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_lr  - list n registers (local memory) from index i (0 - oldest)
+-----------------------------------------------------------------------------------------------------*/
void cmd_lr(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dr  - delete registers (local memory)
+-----------------------------------------------------------------------------------------------------*/
void cmd_dr(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_cpt  - check period of transference
+-----------------------------------------------------------------------------------------------------*/
void cmd_cpt(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mpt  - modify period of transference (minutes - 0 deactivate)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mpt(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_cttl  - check threshold temperature and luminosity for processing
+-----------------------------------------------------------------------------------------------------*/
void cmd_cttl(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dttl  - define threshold temperature and luminosity for processing
+-----------------------------------------------------------------------------------------------------*/
void cmd_dttl(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_pr  - process registers (max, min, mean) between instants t1 and t2 (h,m,s)
+-----------------------------------------------------------------------------------------------------*/
void cmd_pr(int argc, char** argv){

}
