/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (projecto SCE)
|
| Autor: Pedro Antunes (IST90170), Carolina Zebre (IST86961), Shaida
| Data:  Dezembro 2020
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cyg/io/io.h>

/* It is assumed that SOM and EOM values do not occur in the message */
#define SOM 0xFD /* start of message */
#define EOM 0xFE /* end of message */

#define RCLK 0xC0 /* read clock */
#define SCLK 0XC1 /* set clock */
#define RTL 0XC2 /* read temperature and luminosity */
#define RPAR 0XC3 /* read parameters */
#define MMP 0XC4 /* modify monitoring period */
#define MTA 0XC5 /* modify time alarm */
#define RALA 0XC6 /* read alarms (clock, temperature, luminosity, active/inactive) */
#define DAC 0XC7 /* define alarm clock */
#define DATL 0XC8 /* define alarm temperature and luminosity */
#define AALA 0XC9 /* activate/deactivate alarms */
#define IREG 0XCA /* information about registers (NREG, nr, iread, iwrite)*/
#define TRGC 0XCB /* transfer registers (curr. position)*/
#define TRGI 0XCC /* transfer registers (index) */
#define NMFL 0XCD /* notification memory (half) full */

#define CMD_OK 0 /* command successful */
#define CMD_ERROR 0xFF /* error in command */


Cyg_ErrNo err;
cyg_io_handle_t serH;


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
  unsigned char bufw[8];
  unsigned char x[] = {SOM, RCLK, EOM};
  for(i=0; i< (unsigned char)sizeof(x); i++){
    bufw[i]=(unsigned char)x[i];
    err = cyg_io_write(serH, bufw, &n);
    printf("Sting sent %s\n", bufw[i]);
    printf("io_write err=%x, n=%d\n", err, n);
  }
}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_sc  - set clock
+-----------------------------------------------------------------------------------------------------*/
void cmd_sc(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_rtl  - read temperature and luminosity
+-----------------------------------------------------------------------------------------------------*/
void cmd_rtl(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_rp  - read parameters (PMON, TALA)
+-----------------------------------------------------------------------------------------------------*/
void cmd_rp(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mmp  - modify monitoring period (seconds - 0 deactivate)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mmp(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_mta  - modify time alarm (seconds)
+-----------------------------------------------------------------------------------------------------*/
void cmd_mta(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_ra  - read alarms (clock, temperature, luminosity, active/inactive-1/0)
+-----------------------------------------------------------------------------------------------------*/
void cmd_ra(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dac  - define alarm clock
+-----------------------------------------------------------------------------------------------------*/
void cmd_dac(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_dtl  - define alarm temperature and luminosity
+-----------------------------------------------------------------------------------------------------*/
void cmd_dtl(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_aa  - activate/deactivate alarms (1/0)
+-----------------------------------------------------------------------------------------------------*/
void cmd_aa(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_ir  - information about registers (NREG, nr, iread, iwrite)
+-----------------------------------------------------------------------------------------------------*/
void cmd_ir(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_trc  - transfer n registers from current iread position
+-----------------------------------------------------------------------------------------------------*/
void cmd_trc(int argc, char** argv){

}

/*----------------------------------------------------------------------------------------------------+
| Function: cmd_tri  - transfer n registers from index i (0 - oldest)
+-----------------------------------------------------------------------------------------------------*/
void cmd_tri(int argc, char** argv){

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
