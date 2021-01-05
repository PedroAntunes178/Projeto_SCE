/**
  Generated Main Source File
  Company:
    Microchip Technology Inc.
  File Name:
    main.c
  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs
  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18875
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "I2C/i2c.h"
#include "stdio.h"
#include "project.h"

/*
                         Main application
 */


//#define WDT_Enable()        (WDTCON0bits.SWDTEN = 1)
//#define WDT_Disable()       (WDTCON0bits.SWDTEN = 0)


uint8_t alarm = 0;
uint8_t sensor_enable = 1;

uint8_t flag = 0;
uint8_t buff[]={0,0,0,0,0,0,0,0};
uint8_t buff_pos = 0;

uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
uint8_t old_seconds = 0;

uint8_t alah = 12;
uint8_t alam = 0;
uint8_t alas = 0;
uint8_t alat = 28;
uint8_t alal = 4;
uint8_t nreg = 25;
uint8_t pmon = 3;
uint8_t tala = 5;

uint8_t register_counter = 0;

uint8_t register_not_transfered = 0;
extern uint8_t valid_registers;
extern uint8_t oldest_register;
uint8_t iread = 0;


unsigned char t, old_t;
unsigned char  l, old_l;

void main(void)
{

    // initialize the device
    SYSTEM_Initialize();
    read_memory();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    i2c1_driver_open();
    I2C_SCL = 1;
    I2C_SDA = 1;
    WPUC3 = 1;
    WPUC4 = 1;
    LCDinit();

    LCD_write();

    PWM_Output_D4_Enable();
    PWM6_LoadDutyValue(50);

    INTERRUPT_TMR0InterruptEnable();
    TMR0_SetInterruptHandler(count_time_ISR);

    unsigned char buf[17];

    while (1) {
        checkButtonS1();

        if( button_1==1 && alarm == 1){
            button_1=0;
            alarm = 0;
            LCDcmd(0x8B);
            sprintf(buf, "   ");
            LCDstr(buf);
        }
        else if( button_1==1 && alarm == 0){
            button_1=0;
            INTERRUPT_TMR0InterruptDisable();
            modify();
            INTERRUPT_TMR0InterruptEnable();
        }

        if(seconds != old_seconds){
            old_seconds = seconds;
            count_time();
        }

        init_communication();
    }

    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();

}

void init_communication()
{
    volatile uint8_t rxData;
    uint8_t ready = 0;
    ready = EUSART_is_rx_ready();

    if(ready > 0)
    {
        rxData = EUSART_Read();
        if(rxData == SOM)
        {
            flag =1;
            buff_pos = 0;
        }
        else if(rxData == EOM){
            buff[0] = buff_pos;
            process_cmd(buff);
            buff_pos = 0;
            flag = 0;
        }
        else{
            buff_pos++;
            buff[buff_pos]=rxData;
        }
    }
}

void process_cmd(uint8_t *buff)
{
    uint8_t buf[17];

    if (buff[1] == RCLK){
        r_clock();
    }
    else if (buff[1] == SCLK){
        s_clock(buff);
    }
    else if (buff[1] == RTL){
        r_tl();
    }
    else if (buff[1] == RPAR){
        r_par();
    }
    else if (buff[1] == MMP){
        mmp(buff);
    }
    else if (buff[1] == MTA){
        mta(buff);
    }
    else if (buff[1] == RALA){
        r_ala();
    }
    else if (buff[1] == DAC){
        dac();
    }
    else if (buff[1] == DATL){
        datl();
    }
    else if (buff[1] == AALA){
        aala();
    }
    else if (buff[1] == IREG){
        ireg();
    }
    else if (buff[1] == TRGC){
        trgc(buff);
    }
    else if (buff[1]== TRGI){
        trgi(buff);
    }
}
void r_clock(){
    uint8_t rc[]={SOM, RCLK, 0, 0, 0, EOM};
    rc[2]= (uint8_t)hours;
    rc[3]= (uint8_t)minutes;
    rc[4]= (uint8_t)seconds;

    send_msg(rc, 6);
}
void s_clock(uint8_t buff[8]){
    uint8_t buf[17];

    if(buff[0]!= 4){
        uint8_t e[]={SOM, SCLK, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else if(buff[2]<0 || buff[2]>=24 || buff[3]<0 || buff[3]>=60 || buff[4]<0 || buff[4]>=60){
        uint8_t e[]={SOM, SCLK, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else{
        hours = buff[2];
        minutes = buff[3];
        seconds = buff[4];
        LCDcmd(0x80);
        sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
        LCDstr(buf);

        uint8_t m[]={SOM, SCLK, CMD_OK, EOM};
        send_msg(m, 4);
    }
}
void r_tl(){
  uint8_t rtl[]={SOM, RTL, 0, 0, EOM};
  rtl[2]= (uint8_t)t;
  rtl[3]= (uint8_t)l;

  send_msg(rtl, 5);
}
void r_par(){
  uint8_t rtl[]={SOM, RPAR, 0, 0, EOM};
  rtl[2]= (uint8_t)pmon;
  rtl[3]= (uint8_t)tala;

  send_msg(rtl, 5);
}
void mmp(uint8_t buff[8]){
    if(buff[0]!= 2){
        uint8_t e[]={SOM, MMP, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    if(buff[2]<0 || buff[2]>99)
    {
        uint8_t e[]={SOM, MMP, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else{
        
        pmon = buff[2];
        DATAEE_WriteByte(PMON, pmon);
        write_checksum();
        uint8_t m[]={SOM, MMP, CMD_OK, EOM};
        send_msg(m, 4);
    }
}
void mta(uint8_t buff[8]){
    if(buff[2]<0 || buff[2]>60)
    {
        uint8_t e[]={SOM, MTA,CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else{
        tala = buff[2];
        DATAEE_WriteByte(TALA, tala);
        write_checksum();
        uint8_t m[]={SOM, MTA, CMD_OK, EOM};
        send_msg(m, 4);
    }
}
void r_ala(){
    uint8_t rc[]={SOM, RALA, 0, 0, 0, 0, 0, 0, EOM};
    rc[2]= (uint8_t)alah;
    rc[3]= (uint8_t)alam;
    rc[4]= (uint8_t)alas;
    rc[5]= (uint8_t)alat;
    rc[6]= (uint8_t)alal;
    rc[7]= (uint8_t)sensor_enable;

    send_msg(rc, 9);
}
void dac(){
    if(buff[0]!= 4){
        uint8_t e[]={SOM, DAC, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else if(buff[2]<0 || buff[2]>=24 || buff[3]<0 || buff[3]>=60 || buff[4]<0 || buff[4]>=60){
        uint8_t e[]={SOM, DAC, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else{
        alah = buff[2];
        alam = buff[3];
        alas = buff[4];
        
        DATAEE_WriteByte(ALAH, alah);
        DATAEE_WriteByte(ALAM, alam);
        DATAEE_WriteByte(ALAS, alas);
        write_checksum();

        uint8_t m[]={SOM, DAC, CMD_OK, EOM};
        send_msg(m, 4);
    }
}
void datl(){
    uint8_t buf[17];
    if(buff[0]!= 3){
        uint8_t e[]={SOM, DATL, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else if(buff[2]<0 || buff[2]>50 || buff[3]<0 || buff[3]>7){
        uint8_t e[]={SOM, DATL, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else{
        alat = buff[2];
        alal = buff[3];

        DATAEE_WriteByte(ALAT, alat);
        DATAEE_WriteByte(ALAL, alal);
        write_checksum();
        
        uint8_t m[]={SOM, DATL, CMD_OK, EOM};
        send_msg(m, 4);
    }
}

void aala(){
    uint8_t buf[17];
    uint8_t enable; 
    if(buff[2]<0 || buff[2]>1){
        uint8_t e[]={SOM, AALA, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else {
        enable = buff[2];
        uint8_t m[]={SOM, AALA, CMD_OK, EOM};
        send_msg(m, 4);
        if (enable == 1){
            sensor_enable = 1;
            LCDcmd(0x8F);
            sprintf(buf, "A");
            LCDstr(buf);
        }
        else{
            sensor_enable = 0;
            LCDcmd(0x8F);
            sprintf(buf, "a");
            LCDstr(buf);
        }
    }

}
void ireg(){
    uint8_t ireg_msg[]={SOM, IREG, 0, 0, 0, 0, EOM};
    ireg_msg[2]= nreg;
    ireg_msg[3]= valid_registers;
    ireg_msg[4]= iread;
    ireg_msg[5]= register_counter;

    send_msg(ireg_msg, 7);
}

void trgc(uint8_t buff[8]){
    int i;
    uint8_t buf[17];
    uint8_t n;
    uint16_t mem;
    uint8_t send_trgc[]={0, 0, 0, 0, 0};

    if(buff[2]<0 || buff[2]>nreg){
        uint8_t e[]={SOM, TRGC, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else if(register_not_transfered == 0){
        n=0;
        uint8_t m[]={SOM, TRGC, 0, EOM};
        m[2] = n;
        
        send_msg(m, 4);
    }
    else {
        if(buff[2] > register_not_transfered)
            n = register_not_transfered;
        else 
            n = buff[2];

        uint8_t send_som[]={SOM, TRGC, 0};
        send_som[2] = n;
        send_msg(send_som, 3);
        
        for(i=0; i< n; i++){
            mem = START_REG + iread*5;
            send_trgc[0]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgc[1]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgc[2]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgc[3]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgc[4]= (uint8_t)DATAEE_ReadByte(mem);
            send_msg(send_trgc, 5);
            
            register_not_transfered--;
            if(register_not_transfered == -1){
                register_not_transfered = 0;
            }
            iread ++;
            if(iread == nreg)
                iread = 0;
            
            DATAEE_WriteByte(IREAD, iread);
            DATAEE_WriteByte(RNT, register_not_transfered);
            write_checksum();
        }
        uint8_t send_eom[]={EOM};
        send_msg(send_eom, 1);
    }
}

void trgi(uint8_t buff[8]){
    int a;
    int buf[17];
    uint8_t n;
    uint8_t i;
    uint16_t mem;
    uint8_t send_trgi[]={0, 0, 0, 0, 0};

    if(buff[2]<0 || buff[2]>nreg || buff[3]<0 || buff[3]>=nreg){
        uint8_t e[]={SOM, TRGI, CMD_ERROR, EOM};
        send_msg(e, 4);
    }
    else if(register_not_transfered == 0){
        n=0;
        i = iread;
        uint8_t m[]={SOM, TRGI, 0, 0, EOM};
        m[2] = n;
        m[3] = i;
        
        send_msg(m, 5);
    }
    else {
        n = buff[2];
        i = buff[3];
        if(i>valid_registers){
            i=iread;
        }
        else{
            if(register_counter > iread){
                if(i<iread){
                    register_not_transfered = register_not_transfered + (iread-i);
                }
                if(i>=register_counter){
                    register_not_transfered = nreg-(i-register_counter);
                }
                else {
                    register_not_transfered = register_not_transfered - (i-iread);
                }
            }
            else if(register_counter < iread){
                if(i<iread && i>=register_counter){
                    register_not_transfered = register_not_transfered + (iread-i); 
                }
                else if (i< register_counter){
                    register_not_transfered = register_counter-i;
                }
                else if (i > iread){
                    register_not_transfered = register_not_transfered - (i-iread);
                }   
            } 
        }

        if(n > register_not_transfered)
            n = register_not_transfered;
        iread = i;
        
        /*
        if(buff[3] == 0){
            i = oldest_register;
            iread = i;
            
            if(valid_registers<nreg)
                register_not_transfered =  valid_registers ;
            else
                register_not_transfered = nreg;
            n = register_not_transfered;
        }*/

        uint8_t send_som[]={SOM, TRGI, 0, 0};
        send_som[2] = n;
        send_som[3] = i;
        send_msg(send_som, 4);

        for(a=0; a< n; a++){
            mem = START_REG + iread*5;
            send_trgi[0]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgi[1]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgi[2]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgi[3]= (uint8_t)DATAEE_ReadByte(mem);
            mem++;
            send_trgi[4]= (uint8_t)DATAEE_ReadByte(mem);

            send_msg(send_trgi, 5);
            
            register_not_transfered--;
            if(register_not_transfered == -1){
                register_not_transfered = 0;
            }
            iread ++;
            if(iread == nreg)
                iread = 0;
    
            DATAEE_WriteByte(IREAD, iread);
            DATAEE_WriteByte(RNT, register_not_transfered);
            write_checksum();
        }
        uint8_t send_eom[]={EOM};
        send_msg(send_eom, 1);
    }
}



void send_msg(uint8_t *buffer, uint8_t length){
    volatile uint8_t rxData;

    for(int i=0; i<length; i++){
        if(EUSART_is_tx_ready()){
            rxData = buffer[i];
            EUSART_Write(rxData);
        }
    }
}
