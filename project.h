/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PROJECT_H
#define	PROJECT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
/**
  Section: Macro Declarations
 */
    #define _XTAL_FREQ          500000
    #define PRESSED             1
    #define NOT_PRESSED         0
    #define RUNNING             1
    #define NOT_RUNNING         0
    #define LAST                16
    #define LEDs                LATA
    #define LEDs_SetLow()       do { LATA = 0; } while(0)

    #define INTERRUPT_TMR0InterruptEnable()         do { TMR0IE = 1; } while(0)
    #define INTERRUPT_TMR0InterruptDisable()        do { TMR0IE = 0; } while(0)
/**
  Section: Variable Definitions
 */
    uint8_t projectState = NOT_RUNNING;

/*
void Timer1(void);

  @Summary
    Performs the Interrupt Lab.

  @Description
    The LEDs rotate at a constant speed of ~0.5s from left to right.

  @Preconditions
    SYSTEM_Initialize() functions should have been called before calling this function.

  @Returns
    true ou false dependendo se existe overflow do timer

  @Param
    none
*/

unsigned char get_Temprature(void);
/**
  @Summary
    Parte logica do projecto

  @Description
    Conta o tempo

  @Preconditions
    SYSTEM_Initialize() functions should have been called before calling this function.

  @Returns
    char com a Temperatura

  @Param
    none
*/

void get_Luminosity(char *);
/**
  @Summary
    Parte logica do projecto

  @Description
    Conta a luminiosidade

  @Preconditions
    SYSTEM_Initialize() functions should have been called before calling this function.

  @Returns
   char com a luminiosidade

  @Param
    none
*/
void LCD_write(void);
void count_time_ISR(void);
void sensor(void);
void save_sensor(void);


#endif	/* PROJECT_H */
/**
 End of File
 */