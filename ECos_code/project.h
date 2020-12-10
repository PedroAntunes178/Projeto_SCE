#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>

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
/*-------------------------------------------------------------------------+
| Headers of command functions in cmd.c
+--------------------------------------------------------------------------*/
void process_read(unsigned char *);

/*-------------------------------------------------------------------------+
| Headers of command functions in monitor.c
+--------------------------------------------------------------------------*/
void cmd_sos(int, char**);
void monitor(void);

/*-------------------------------------------------------------------------+
| Headers of command functions in comands.c
+--------------------------------------------------------------------------*/
void cmd_sair(int, char**);
void cmd_ini(int, char**);
void cmd_rc(int, char**);
void cmd_sc(int, char**);
void cmd_rtl(int, char**);
void cmd_rp(int, char**);
void cmd_mmp(int, char**);
void cmd_mta(int, char**);
void cmd_ra(int, char**);
void cmd_dac(int, char**);
void cmd_dtl(int, char**);
void cmd_aa(int, char**);
void cmd_ir(int, char**);
void cmd_trc(int, char**);
void cmd_tri(int, char**);
void cmd_irl(int, char**);
void cmd_lr(int, char**);
void cmd_dr(int, char**);
void cmd_cpt(int, char**);
void cmd_mpt(int, char**);
void cmd_cttl(int, char**);
void cmd_dttl(int, char**);
void cmd_pr(int, char**);

/*-------------------------------------------------------------------------+
| Global variables
+--------------------------------------------------------------------------*/

/* and now a mutex to protect calls to the C library, cmd.c */
extern cyg_mutex_t cliblock;
/*Variaveis globais para escrever e ler do PIC16, cmd.c*/
extern Cyg_ErrNo err;
extern cyg_io_handle_t serH;
/*Variaveis globais correspondentes a mail box. mbx1 é a mail box, cmd.c  */
extern cyg_handle_t mbx1H, mbx2H;
extern cyg_mbox mbx1, mbx2;

//size of registers list, comando.c
int n = 0;
