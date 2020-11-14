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

#define LCD_ADDR 0x4e   // 0x27 << 1
#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01

#define MAGIC_NUMBER 0xF000 // this memory slot will save if we already runned the program ones or not
#define NREG 0xF001 //number of data registers
#define PMON 0xF002  //sec monitoring period
#define TALA 0xF003  //sec duration of alarm signal (PWM)
#define ALAH 0xF004 //hours of alarm clock
#define ALAM 0xF005  //minutes of alarm clock
#define ALAS 0xF006  //seconds of alarm clock
#define ALAT 0xF007 //oC threshold for temperature alarm
#define ALAL 0xF008  //threshold for luminosity level alarm
#define ALAF 0xF009  //alarm flag initially disabled
#define CLKH 0xF00A  //initial value for clock hours
#define CLKM 0xF00B  //initial value for clock minutes
#define LAST_REG 0xF00C //this memory slot will save the last register number we used
#define CHECKSUM 0xF00D //The sum is going to be saved in byte 0xF00D and 0xF00E cause its 16 bits

#define START_REG  0xF00F        // EEPROM starting address if there are 25 reg then the last reg will be at F027
        
#define WDT_Enable()        (WDTCON0bits.SWDTEN = 1)
#define WDT_Disable()       (WDTCON0bits.SWDTEN = 0)

uint8_t alarm = 0;
uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
uint8_t alarm_secs = -1;
uint8_t sensor_enable = 1;

uint8_t alah = 12;
uint8_t alam = 0;
uint8_t alas = 0;
uint8_t alat = 28;
uint8_t alal = 4;
uint8_t nreg = 25;
uint8_t pmon = 3;
uint8_t tala = 5;
unsigned char t, old_t;
unsigned char  l, old_l;

uint8_t register_counter = 0;

void LCDsend(unsigned char c)
{
  IdleI2C();
  StartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x00); IdleI2C();
  WriteI2C(c); IdleI2C();
  StopI2C();    
}
unsigned char LCDrecv(unsigned char mode)
{
  unsigned char hc;
  unsigned char lc;
  
  IdleI2C();
  StartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x00); IdleI2C();
  WriteI2C(0xf0 | LCD_BL | LCD_RW | mode); IdleI2C();
  WriteI2C(0xf0 | LCD_BL | LCD_EN | LCD_RW | mode); IdleI2C();
  __delay_us(1);
  RestartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x01); IdleI2C();
  hc = ReadI2C(); IdleI2C();
  NotAckI2C();
  RestartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x00); IdleI2C();
  WriteI2C(0xf0 | LCD_BL | LCD_RW | mode); IdleI2C();
  WriteI2C(0xf0 | LCD_BL | LCD_EN | LCD_RW | mode); IdleI2C();
  __delay_us(1);
  RestartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x01); IdleI2C();
  lc = ReadI2C(); IdleI2C();
  NotAckI2C();
  RestartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x00); IdleI2C();
  WriteI2C(0xf0 | LCD_BL | LCD_RW | mode); IdleI2C();
  StopI2C();
  return ((hc&0xf0) | ((lc>>4)&0x0f));
}

void LCDsend2x4(unsigned char c, unsigned char mode)
{
  unsigned char hc;
  unsigned char lc;
  
  hc = c & 0xf0;
  lc = (c << 4) & 0xf0;
    
  IdleI2C();
  StartI2C(); IdleI2C();
  WriteI2C(LCD_ADDR | 0x00); IdleI2C();
  WriteI2C(hc | LCD_BL | mode); IdleI2C();
  WriteI2C(hc | LCD_BL | LCD_EN | mode); IdleI2C();
  __delay_us(1);
  WriteI2C(hc | LCD_BL | mode); IdleI2C();
  WriteI2C(lc | LCD_BL | mode); IdleI2C();
  WriteI2C(lc | LCD_BL | LCD_EN | mode); IdleI2C();
  __delay_us(1);
  WriteI2C(lc | LCD_BL | mode); IdleI2C();
  StopI2C();    
  __delay_us(50);
}

void LCDinit(void)
{
    __delay_ms(50);
    LCDsend(0x30);
    LCDsend(0x34); __delay_us(500); LCDsend(0x30);
    __delay_ms(5);
    LCDsend(0x30);
    LCDsend(0x34); __delay_us(500); LCDsend(0x30);
    __delay_us(100);
    LCDsend(0x30);
    LCDsend(0x34); __delay_us(500); LCDsend(0x30);
    __delay_us(100);
    LCDsend(0x20);
    LCDsend(0x24); __delay_us(500); LCDsend(0x20);
    __delay_ms(5);

    LCDsend2x4(0x28, 0);
    LCDsend2x4(0x06, 0);
    LCDsend2x4(0x0f, 0);
    LCDsend2x4(0x01, 0);
    __delay_ms(2);
}

void LCDcmd(unsigned char c)
{
  LCDsend2x4(c, 0);
}

void LCDchar(unsigned char c)
{
  LCDsend2x4(c, LCD_RS);
}

void LCDstr(unsigned char *p)
{
  unsigned char c;
  
  while((c = *p++)) LCDchar(c);
}

int LCDbusy()
{
    if(LCDrecv(0) & 0x80) return 1;
    return 0;
}

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
            sensor_enable = 1;
            LCDcmd(0x8B);
            sprintf(buf, "   ");
            LCDstr(buf);
            LCDcmd(0x8F);
            sprintf(buf, "a");
            LCDstr(buf);
        }
        else if( button_1==1 && alarm == 0){
            button_1=0;
            INTERRUPT_TMR0InterruptDisable();
            modify();
            INTERRUPT_TMR0InterruptEnable();
        }
    }

    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
    
}

void read_memory(void)
{
    uint8_t magic_nr = 55;
    uint16_t mem;
    uint16_t count;
    if (DATAEE_ReadByte(MAGIC_NUMBER)==magic_nr){
        mem = START_REG + 5*DATAEE_ReadByte(LAST_REG);
        mem = mem + 3;
        t = DATAEE_ReadByte(mem);
        mem ++;
        l = DATAEE_ReadByte(mem);
       
        alah = DATAEE_ReadByte(ALAH);
        alam = DATAEE_ReadByte(ALAM);
        alas = DATAEE_ReadByte(ALAS);
        alat = DATAEE_ReadByte(ALAT);
        alal = DATAEE_ReadByte(ALAL);
        nreg = DATAEE_ReadByte(NREG);
        pmon = DATAEE_ReadByte(PMON);
        tala = DATAEE_ReadByte(TALA);
        register_counter =  DATAEE_ReadByte(LAST_REG);
        count = alah + alam + alas + alat + alal + nreg + pmon + tala;
        
        if (DATAEE_ReadByte(CHECKSUM)!=count){
            alah = 12;
            alam = 0;
            alas = 0;
            alat = 28;
            alal = 4;
            nreg = 25;
            pmon = 3;
            tala = 5;
            register_counter = 0;
            DATAEE_WriteByte(ALAH, alah);
            DATAEE_WriteByte(ALAM, alam);
            DATAEE_WriteByte(ALAS, alas);
            DATAEE_WriteByte(ALAT, alat);
            DATAEE_WriteByte(ALAL, alal);
            DATAEE_WriteByte(NREG, nreg);
            DATAEE_WriteByte(PMON, pmon);
            DATAEE_WriteByte(TALA, tala);
            DATAEE_WriteByte(LAST_REG, register_counter);
            write_checksum();
            
        }
    }
    else{
        DATAEE_WriteByte(MAGIC_NUMBER, magic_nr);
        DATAEE_WriteByte(ALAH, alah);
        DATAEE_WriteByte(ALAM, alam);
        DATAEE_WriteByte(ALAS, alas);
        DATAEE_WriteByte(ALAT, alat);
        DATAEE_WriteByte(ALAL, alal);
        DATAEE_WriteByte(NREG, nreg);
        DATAEE_WriteByte(PMON, pmon);
        DATAEE_WriteByte(TALA, tala);
        DATAEE_WriteByte(LAST_REG, register_counter);
        write_checksum();
    }
}

void write_checksum()
{
    uint16_t count;
    
    count = alah + alam + alas + alat + alal + nreg + pmon + tala;
    
    DATAEE_WriteByte(CHECKSUM, count);
}
void LCD_write()
{
    unsigned char buf[17];
    
    LCDcmd(0x80);
    sprintf(buf, "%02d:%02d:%02d", hours, minutes,seconds);
    LCDstr(buf);

    LCDcmd(0xc0);
    sprintf(buf, "%02d C", t);
    LCDstr(buf);

    LCDcmd(0xcD);
    sprintf(buf, "L %01d", l);
    LCDstr(buf);
    
    LCDcmd(0x8F);
    sprintf(buf, "a");
    LCDstr(buf);
}
unsigned char get_Temprature(void)
{
	unsigned char value;
    do{
        IdleI2C();
        StartI2C(); IdleI2C();

        WriteI2C(0x9a | 0x00); IdleI2C();
        WriteI2C(0x01); IdleI2C();
        RestartI2C(); IdleI2C();
        WriteI2C(0x9a | 0x01); IdleI2C();
        value = ReadI2C(); IdleI2C();
        NotAckI2C(); IdleI2C();
        StopI2C();
    } while (!(value & 0x40));

        IdleI2C();
        StartI2C(); IdleI2C();
        WriteI2C(0x9a | 0x00); IdleI2C();
        WriteI2C(0x00); IdleI2C();
        RestartI2C(); IdleI2C();
        WriteI2C(0x9a | 0x01); IdleI2C();
        value = ReadI2C(); IdleI2C();
        NotAckI2C(); IdleI2C();
        StopI2C();

	return value;
}

void get_Luminosity(char *buf){
    l = ADCC_GetSingleConversion(channel_ANA0)>>13;
    sprintf(buf, "L %01d", l);
}


void count_time_ISR() { 
    unsigned char buf[17];
    uint16_t mem;
    uint8_t last_reg;
    
    seconds = seconds + 1;
    
    if( alarm_secs == seconds){
        alarm_secs = -1;
        PWM6_LoadDutyValue(50);
    }
    LED_4_Toggle();
    if(seconds==60){
        minutes += 1;
        NOP();
        LCDcmd(0x83);
        sprintf(buf, "%02d:", minutes);
        LCDstr(buf);

        seconds = 0;
    }
    
    if(minutes==60){
        hours += 1;
        minutes = 0;
        NOP();
        LCDcmd(0x80);
        sprintf(buf, "%02d:%02d:", hours, minutes);
        LCDstr(buf);
    }
    if(hours==24){
        hours = 0;
        NOP();
        LCDcmd(0x80);
        sprintf(buf, "%02d:", hours);
        LCDstr(buf);
    }
    
    NOP();
    LCDcmd(0x86);
    sprintf(buf, "%02d", seconds);
    LCDstr(buf);
    
    if(pmon != 0){
        if ((seconds%pmon) == 0){
            sensor();
            
            last_reg = DATAEE_ReadByte(LAST_REG);
            mem = START_REG + last_reg*5 + 3;
            old_t = DATAEE_ReadByte(mem);
            mem++;
            old_l = DATAEE_ReadByte(mem);

            if(t != old_t || l != old_l){
                save_sensor(); 
            }
            check_alarm();
        }   
    }
    
    WDT_Enable();
    SLEEP();
    WDT_Disable();
}

void sensor()
{
    unsigned char buf[17];
    if (sensor_enable == 1)
    {
        NOP();
        t = get_Temprature();

        LCDcmd(0xc0);
        sprintf(buf, "%02d C ", t);
        LCDstr(buf);

        NOP();
        get_Luminosity(buf);
        LCDcmd(0xcD);
        LCDstr(buf);
    }

}

void check_alarm()
{
    uint8_t alarm_check = 0;
    unsigned char buf[17];
    if (sensor_enable == 1) {
        if(hours >= alah && (minutes >= alam || seconds >= alas)){
            LCDcmd(0x8B);
            sprintf(buf, "C");
            LCDstr(buf);

            alarm_check = 1;
        }

        if ( t >= alat ){
            LCDcmd(0x8C);
            sprintf(buf, "T");
            LCDstr(buf);
            LED_2_SetHigh();

            alarm_check = 1;
        }
        else{
            LED_2_SetLow();
        }

        if ( l <= alal ){
            LCDcmd(0x8D);
            sprintf(buf, "L");
            LCDstr(buf);
            LED_1_SetHigh();

            alarm_check = 1;
        }
        else{
            LED_1_SetLow();
        }
    }
    if(alarm_check == 1 || sensor_enable == 0){
        alarm_secs = seconds + tala;
        if (alarm_secs > 60){
            alarm_secs = alarm_secs-60;
        }
        PWM6_LoadDutyValue(500);
        alarm = 1;
        LCDcmd(0x8F);
        sprintf(buf, "A");
        LCDstr(buf);  
    }

}

void save_sensor()
{      
    uint16_t Addr;   
    
    Addr = START_REG+register_counter*5;
    
    DATAEE_WriteByte(Addr, hours);
    Addr ++;
    DATAEE_WriteByte(Addr, minutes);
    Addr ++;
    DATAEE_WriteByte(Addr, seconds);
    Addr ++;
    DATAEE_WriteByte(Addr, t);
    Addr ++;
    DATAEE_WriteByte(Addr, l);
    
    DATAEE_WriteByte(LAST_REG, register_counter);
    register_counter++;
    
    if (register_counter == nreg)
        register_counter = 0;
}

void checkButtonS1(void) {
    
    if (btn1State == NOT_PRESSED) {
        if (SWITCH_S1_PORT == LOW) {
            __delay_ms(100);
            btn1State = PRESSED;
        }
    } else if (SWITCH_S1_PORT == HIGH) {
        btn1State = NOT_PRESSED;
        button_1 = 1;
    }
}

    

void checkButtonS2(void) {

    if (btn2State == NOT_PRESSED) {
    if (SWITCH_S2_PORT == LOW) {
        __delay_ms(100);
        btn2State = PRESSED;
    }
    } else if (SWITCH_S2_PORT == HIGH) {
        btn2State = NOT_PRESSED;
        button_2 = 1;
    }
}

void modify(void)
{
    int i = 0;
    unsigned char buf[17];
    
    LCDcmd(0x8B);
    sprintf(buf, "CTL");
    LCDstr(buf);
                
    change_hours();
    change_minutes();
    change_seconds();
    change_clock_alarm();
    change_temp_alarm();
    change_lumi_alarm();
    change_alarm();
       
    LCDcmd(0x8B);
    sprintf(buf, "   ");
    LCDstr(buf);
}

void change_hours(void)
{
    unsigned char buf[17];
    
    while(1)
    {
        LCDcmd(0x80);
        checkButtonS1();
        checkButtonS2();
        
        if (button_2==1){
            button_2=0;
            hours = hours+1;
            if (hours == 24){
                hours = 0;
            }
            LCDcmd(0x80);
            sprintf(buf, "%02d", hours);
            LCDstr(buf);
        }
        
        if (button_1==1){
            button_1=0;
            break;
        }
    }
}

void change_minutes(void)
{
    unsigned char buf[17];
    
    while(1){
        LCDcmd(0x83);
        checkButtonS1();
        checkButtonS2();
        if (button_2==1){
            button_2=0;
            minutes = minutes+1;
            if(minutes == 60){
                minutes = 0;
            } 
            LCDcmd(0x83);
            sprintf(buf, "%02d", minutes);
            LCDstr(buf);
        }
    
        if (button_1==1){
            button_1=0;
            break;
        }    
    }
}

void change_seconds(void)
{
    unsigned char buf[17];
    
    while(1)
    {
        LCDcmd(0x86);
        checkButtonS1();
        checkButtonS2();
        
        if (button_2==1){
            button_2 = 0;
            seconds=seconds+1; 
            if(seconds == 60){
                seconds = 0;
            }
            LCDcmd(0x86);
            sprintf(buf, "%02d", seconds);
            LCDstr(buf);
        }
        if (button_1==1){
            button_1 = 0;
            break;
        } 
    }       
}
void change_clock_alarm(void)
{
    unsigned char buf[17];
    uint8_t i = 0;
    LCDcmd(0x80);
    sprintf(buf, "%02d:%02d:%02d", alah, alam, alas);
    LCDstr(buf);
    LCDcmd(0x8B);
    
    while(i != 3){
        checkButtonS1();
        checkButtonS2();
        
        if (button_1==1){
            button_1=0;
            i++;
        }

        if(i==0){
            if (button_2==1){
                button_2 = 0;
                alah++;
                if(alah == 24){
                    alah = 0;
                }
                DATAEE_WriteByte(ALAH, alah);
                write_checksum();
                LCDcmd(0x80);
                sprintf(buf, "%02d", alah);
                LCDstr(buf);
                LCDcmd(0x8B);  
            }
        }
        if(i==1){
            if (button_2==1){
                button_2 = 0;
                alam++;
                if(alam == 60){
                    alam = 0;
                }
                DATAEE_WriteByte(ALAM, alam);
                write_checksum();
                LCDcmd(0x83);
                sprintf(buf, "%02d", alam);
                LCDstr(buf);
                LCDcmd(0x8B); 
            }
        }
        if(i==2){
            if (button_2==1){
                button_2 = 0;
               alas++;
               if(alas == 60){
                   alas = 0;
               }
               DATAEE_WriteByte(ALAS, alas);
               write_checksum();
               LCDcmd(0x86);
               sprintf(buf, "%02d", alas);
               LCDstr(buf);
               LCDcmd(0x8B);
            }   
        }
    }
    LCDcmd(0x80);
    sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
    LCDstr(buf);
}

void change_temp_alarm()
{
    unsigned char buf[17];
    LCDcmd(0xc0);
    sprintf(buf, "%02d C", alat);
    LCDstr(buf);
    LCDcmd(0x8C);
    while(1)
    {
        checkButtonS1();
        checkButtonS2();
        
        if (button_2==1){
            button_2 = 0;
            alat++;  
            if (alat == 51){
                alat = 0;
            }
            DATAEE_WriteByte(ALAT, alat);
            write_checksum();
            LCDcmd(0xc0);
            sprintf(buf, "%02d C", alat);
            LCDstr(buf);
            LCDcmd(0x8C);
        }
     
        if (button_1==1){
            button_1 = 0;
           break;
        } 
    }
    LCDcmd(0xc0);
    sprintf(buf, "%02d C", t);
    LCDstr(buf);
}

void change_lumi_alarm()
{
    unsigned char buf[17];
    LCDcmd(0xcD);
    sprintf(buf, "L %01d", alal);
    LCDstr(buf);
    LCDcmd(0x8D);
    while(1)
    {
        checkButtonS1();
        checkButtonS2();
        
        if (button_2==1){
            button_2 = 0;
            alal++;  
            if(alal == 8){
                alal = 0;
            }   
            DATAEE_WriteByte(ALAL, alal);
            write_checksum();
            LCDcmd(0xcD);
            sprintf(buf, "L %01d", alal);
            LCDstr(buf);
            LCDcmd(0x8D);
        }
     
        if (button_1==1){
            button_1 = 0;
           break;
        } 
    }
    LCDcmd(0xcD);
    sprintf(buf, "L %01d", l);
    LCDstr(buf);
}

void change_alarm(void)
{
    unsigned char buf[17];
    while(1)
    {
        checkButtonS1();
        checkButtonS2();

        if (alarm == 1){
            LCDcmd(0x8F);
            sprintf(buf, "A");
            LCDstr(buf);
        }
        else{
            LCDcmd(0x8F);
            sprintf(buf, "a");
            LCDstr(buf);
        }
        
        if (button_2==1){
            button_2 = 0;
            if (alarm == 1){ //Deactivates alarms
                alarm = 0;
                sensor_enable = 1;
                LCDcmd(0x8F);
                sprintf(buf, "a");
                LCDstr(buf);    
            }
            else{ //Activate alarm
                alarm = 1;
                sensor_enable = 0;
                LCDcmd(0x8F);
                sprintf(buf, "A");
                LCDstr(buf);
            }
        }
        if (button_1==1){
            button_1 = 0;
           break;
        } 
    }
}

void PWM_Output_D4_Enable (void){
PPSLOCK = 0x55;
PPSLOCK = 0xAA;
PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS
// Set D4 as the output of PWM6
RA6PPS = 0x0E;
PPSLOCK = 0x55;
PPSLOCK = 0xAA;
PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
}

void PWM_Output_D4_Disable (void){
PPSLOCK = 0x55;
PPSLOCK = 0xAA;
PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS
// Set D4 as GPIO pin
RA6PPS = 0x00;
PPSLOCK = 0x55;
PPSLOCK = 0xAA;
PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS
}