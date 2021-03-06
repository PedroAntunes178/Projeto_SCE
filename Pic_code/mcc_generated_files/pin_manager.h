/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18875
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set channel_ANA0 aliases
#define channel_ANA0_TRIS                 TRISAbits.TRISA0
#define channel_ANA0_LAT                  LATAbits.LATA0
#define channel_ANA0_PORT                 PORTAbits.RA0
#define channel_ANA0_WPU                  WPUAbits.WPUA0
#define channel_ANA0_OD                   ODCONAbits.ODCA0
#define channel_ANA0_ANS                  ANSELAbits.ANSA0
#define channel_ANA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define channel_ANA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define channel_ANA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define channel_ANA0_GetValue()           PORTAbits.RA0
#define channel_ANA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define channel_ANA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define channel_ANA0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define channel_ANA0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define channel_ANA0_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define channel_ANA0_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define channel_ANA0_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define channel_ANA0_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set LED_1 aliases
#define LED_1_TRIS                 TRISAbits.TRISA4
#define LED_1_LAT                  LATAbits.LATA4
#define LED_1_PORT                 PORTAbits.RA4
#define LED_1_WPU                  WPUAbits.WPUA4
#define LED_1_OD                   ODCONAbits.ODCA4
#define LED_1_ANS                  ANSELAbits.ANSA4
#define LED_1_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define LED_1_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define LED_1_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define LED_1_GetValue()           PORTAbits.RA4
#define LED_1_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define LED_1_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define LED_1_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define LED_1_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define LED_1_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define LED_1_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define LED_1_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define LED_1_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set LED_2 aliases
#define LED_2_TRIS                 TRISAbits.TRISA5
#define LED_2_LAT                  LATAbits.LATA5
#define LED_2_PORT                 PORTAbits.RA5
#define LED_2_WPU                  WPUAbits.WPUA5
#define LED_2_OD                   ODCONAbits.ODCA5
#define LED_2_ANS                  ANSELAbits.ANSA5
#define LED_2_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define LED_2_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define LED_2_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define LED_2_GetValue()           PORTAbits.RA5
#define LED_2_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define LED_2_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define LED_2_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define LED_2_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define LED_2_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define LED_2_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define LED_2_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define LED_2_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set LED_3 aliases
#define LED_3_TRIS                 TRISAbits.TRISA6
#define LED_3_LAT                  LATAbits.LATA6
#define LED_3_PORT                 PORTAbits.RA6
#define LED_3_WPU                  WPUAbits.WPUA6
#define LED_3_OD                   ODCONAbits.ODCA6
#define LED_3_ANS                  ANSELAbits.ANSA6
#define LED_3_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define LED_3_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define LED_3_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define LED_3_GetValue()           PORTAbits.RA6
#define LED_3_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define LED_3_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define LED_3_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define LED_3_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define LED_3_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define LED_3_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define LED_3_SetAnalogMode()      do { ANSELAbits.ANSA6 = 1; } while(0)
#define LED_3_SetDigitalMode()     do { ANSELAbits.ANSA6 = 0; } while(0)

// get/set LED_4 aliases
#define LED_4_TRIS                 TRISAbits.TRISA7
#define LED_4_LAT                  LATAbits.LATA7
#define LED_4_PORT                 PORTAbits.RA7
#define LED_4_WPU                  WPUAbits.WPUA7
#define LED_4_OD                   ODCONAbits.ODCA7
#define LED_4_ANS                  ANSELAbits.ANSA7
#define LED_4_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define LED_4_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define LED_4_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define LED_4_GetValue()           PORTAbits.RA7
#define LED_4_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define LED_4_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define LED_4_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define LED_4_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define LED_4_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define LED_4_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define LED_4_SetAnalogMode()      do { ANSELAbits.ANSA7 = 1; } while(0)
#define LED_4_SetDigitalMode()     do { ANSELAbits.ANSA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode()         do { ANSELBbits.ANSB0 = 1; } while(0)
#define RB0_SetDigitalMode()        do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set SWITCH_S1 aliases
#define SWITCH_S1_TRIS                 TRISBbits.TRISB4
#define SWITCH_S1_LAT                  LATBbits.LATB4
#define SWITCH_S1_PORT                 PORTBbits.RB4
#define SWITCH_S1_WPU                  WPUBbits.WPUB4
#define SWITCH_S1_OD                   ODCONBbits.ODCB4
#define SWITCH_S1_ANS                  ANSELBbits.ANSB4
#define SWITCH_S1_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define SWITCH_S1_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define SWITCH_S1_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define SWITCH_S1_GetValue()           PORTBbits.RB4
#define SWITCH_S1_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define SWITCH_S1_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define SWITCH_S1_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define SWITCH_S1_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define SWITCH_S1_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define SWITCH_S1_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define SWITCH_S1_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define SWITCH_S1_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set SCL1 aliases
#define SCL1_TRIS                 TRISCbits.TRISC3
#define SCL1_LAT                  LATCbits.LATC3
#define SCL1_PORT                 PORTCbits.RC3
#define SCL1_WPU                  WPUCbits.WPUC3
#define SCL1_OD                   ODCONCbits.ODCC3
#define SCL1_ANS                  ANSELCbits.ANSC3
#define SCL1_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SCL1_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SCL1_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SCL1_GetValue()           PORTCbits.RC3
#define SCL1_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SCL1_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define SCL1_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define SCL1_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define SCL1_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define SCL1_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define SCL1_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define SCL1_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set SDA1 aliases
#define SDA1_TRIS                 TRISCbits.TRISC4
#define SDA1_LAT                  LATCbits.LATC4
#define SDA1_PORT                 PORTCbits.RC4
#define SDA1_WPU                  WPUCbits.WPUC4
#define SDA1_OD                   ODCONCbits.ODCC4
#define SDA1_ANS                  ANSELCbits.ANSC4
#define SDA1_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SDA1_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SDA1_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SDA1_GetValue()           PORTCbits.RC4
#define SDA1_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SDA1_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define SDA1_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define SDA1_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define SDA1_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define SDA1_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define SDA1_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define SDA1_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set SWITCH_S2 aliases
#define SWITCH_S2_TRIS                 TRISCbits.TRISC5
#define SWITCH_S2_LAT                  LATCbits.LATC5
#define SWITCH_S2_PORT                 PORTCbits.RC5
#define SWITCH_S2_WPU                  WPUCbits.WPUC5
#define SWITCH_S2_OD                   ODCONCbits.ODCC5
#define SWITCH_S2_ANS                  ANSELCbits.ANSC5
#define SWITCH_S2_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SWITCH_S2_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SWITCH_S2_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SWITCH_S2_GetValue()           PORTCbits.RC5
#define SWITCH_S2_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SWITCH_S2_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define SWITCH_S2_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define SWITCH_S2_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define SWITCH_S2_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define SWITCH_S2_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define SWITCH_S2_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define SWITCH_S2_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/