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
#define WDT_Enable()        (WDTCON0bits.SWDTEN = 1)
#define WDT_Disable()       (WDTCON0bits.SWDTEN = 0)


uint8_t alarm = 0;
uint8_t sensor_enable = 1;


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
            WDT_Enable();
            SLEEP();
            WDT_Disable();
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
            WDT_Enable();
            SLEEP();
            WDT_Disable();
            button_1=0;
            INTERRUPT_TMR0InterruptDisable();
            modify();
            INTERRUPT_TMR0InterruptEnable();

            WDT_Enable();
            SLEEP();
            WDT_Disable();
        }
    }

    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();

}
