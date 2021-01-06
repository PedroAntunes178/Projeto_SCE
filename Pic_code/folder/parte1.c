#include "../mcc_generated_files/mcc.h"
#include "../I2C/i2c.h"
#include "stdio.h"
#include "../project.h"

extern uint8_t alarm;
extern uint8_t sensor_enable;

extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;
uint8_t alarm_secs = -1;


extern uint8_t alah = 12;
extern uint8_t alam = 0;
extern uint8_t alas = 0;
extern uint8_t alat = 28;
extern uint8_t alal = 4;
extern uint8_t nreg = 25;
extern uint8_t pmon = 3;
extern uint8_t tala = 5;

extern unsigned char t, old_t;
extern unsigned char  l, old_l;

uint8_t old_register_counter = 0;
uint8_t oldest_register = 0;
uint8_t first = 0;
uint8_t valid_registers = 0;
uint8_t register_counter = 0;
extern uint8_t register_not_transfered ;
extern uint8_t iread;

void read_memory(void)
{
    uint8_t magic_nr = 55;
    uint16_t mem;
    uint16_t count;
    if (DATAEE_ReadByte(MAGIC_NUMBER)==magic_nr){
        first = 1;
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
        iread = DATAEE_ReadByte(IREAD);
        oldest_register = DATAEE_ReadByte(OLDR);
        register_not_transfered = DATAEE_ReadByte(RNT);
        register_counter = DATAEE_ReadByte(LAST_REG);
        valid_registers = register_counter;
        count = alah + alam + alas + alat + alal + nreg + pmon + tala + iread + register_not_transfered;
        
        if (DATAEE_ReadByte(CHECKSUM)!= count){
            alah = 12;
            alam = 2;
            alas = 0;
            alat = 28;
            alal = 4;
            nreg = 25;
            pmon = 3;
            tala = 5;
            iread = 0;
            first = 0;
            oldest_register = 0;
            valid_registers = 0;
            register_not_transfered = 0;
            register_counter = 0;
            DATAEE_WriteByte(ALAH, alah);
            DATAEE_WriteByte(ALAM, alam);
            DATAEE_WriteByte(ALAS, alas);
            DATAEE_WriteByte(ALAT, alat);
            DATAEE_WriteByte(ALAL, alal);
            DATAEE_WriteByte(NREG, nreg);
            DATAEE_WriteByte(PMON, pmon);
            DATAEE_WriteByte(TALA, tala);
            DATAEE_WriteByte(IREAD, iread);
            DATAEE_WriteByte(OLDR, oldest_register);
            DATAEE_WriteByte(RNT, register_not_transfered);
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
        DATAEE_WriteByte(IREAD, iread);
        DATAEE_WriteByte(OLDR, oldest_register);
        DATAEE_WriteByte(RNT, register_not_transfered);
        DATAEE_WriteByte(LAST_REG, register_counter);
        write_checksum();
    }
}

void write_checksum()
{
    uint16_t count;
    
    count = alah + alam + alas + alat + alal + nreg + pmon + tala + iread + register_not_transfered;
    
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
    sprintf(buf, "A");
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
    seconds = seconds + 1;
}

void count_time(){
    unsigned char buf[17];
    uint16_t mem;
    uint8_t last_reg;

    
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
}

void sensor()
{
    unsigned char buf[17];

    NOP();
    //t = get_Temprature();
    t = 25;
    LCDcmd(0xc0);
    sprintf(buf, "%02d C ", t);
    LCDstr(buf);

    NOP();
    get_Luminosity(buf);
    LCDcmd(0xcD);
    LCDstr(buf);
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
    if(alarm_check == 1){
        alarm_secs = seconds + tala;
        if (alarm_secs > 60){
            alarm_secs = alarm_secs-60;
        }
        PWM6_LoadDutyValue(500);
        alarm = 1; 
    }
}

void save_sensor()
{      
    uint16_t Addr; 
    unsigned char buf[17];
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
    
    old_register_counter = register_counter;
    valid_registers++;
    if(valid_registers > 25)
        valid_registers = 25;
    
    register_counter++;
    
    register_not_transfered++;
    if (register_not_transfered > nreg){
        register_not_transfered = nreg;
    }
    
    if (iread==old_register_counter && register_not_transfered == nreg){
        iread++;
        if(iread == nreg)
            iread = 0;
    }
    if (register_counter == nreg){
        first = 1;
        register_counter = 0;
    }
    
    if (first == 1)
    {
        oldest_register ++;
        if (oldest_register == nreg){
            oldest_register = 0;
        }
        DATAEE_WriteByte(OLDR, oldest_register);
    }
    
    if (register_not_transfered > nreg/2){
        LCDcmd(0xc7);
        LCDstr("M");
        uint8_t nmfl_msg[]={SOM, NMFL, 0, 0, 0, 0, EOM};
        nmfl_msg[2]= nreg;
        nmfl_msg[3]= valid_registers;
        nmfl_msg[4]= iread;
        nmfl_msg[5]= register_counter;

        send_msg(nmfl_msg, 7);
    }
    else {
        LCDcmd(0xc7);
        sprintf(buf," ");
        LCDstr(" ");
    }
    
    
    DATAEE_WriteByte(IREAD, iread);
    DATAEE_WriteByte(RNT, register_not_transfered);
    write_checksum();
}

void checkButtonS1(void) 
{
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

        if (sensor_enable == 1){
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
            if (sensor_enable == 1){ //Deactivates alarms
                sensor_enable = 0;
                LCDcmd(0x8F);
                sprintf(buf, "a");
                LCDstr(buf);    
            }
            else{ //Activate alarm
                sensor_enable = 1;
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
