/* 
 * File:   Esclavo1.c
 * Author: Pablo Caal & Jorge Cerón
 * 
 * Created on 24 de mayo de 2022, 06:00 PM
 */


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF              // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                 // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF              // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V           // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000          // Frecuencia de oscilador en 1 MHz
#define IN_MIN 0                
#define IN_MAX 255                  // Valores de entrada a Potenciometro
#define OUT_MIN1 135
#define OUT_MAX1 590                // Valores para el servomotor MG996R ---> en realidad debería de ser 125 y 600 pero se disminuyeron para que giraran casi 180 grados

/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t TEMPORAL;                   // Bandera para potenciómetro 3
unsigned short POT_3;               // Valor de lectura del potenciómetro 1  
unsigned short POT_4;               // Valor de lectura del potenciómetro 2

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
unsigned short map(uint8_t val, uint8_t in_min, uint8_t in_max, 
            unsigned short out_min, unsigned short out_max);

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){    
    if (PIR1bits.SSPIF){                // ¿Recibió datos el esclavo? 
            TEMPORAL = SSPBUF;
        
        if(PORTBbits.RB7 == 0){
            POT_3 = map(TEMPORAL, IN_MIN, IN_MAX, OUT_MIN1, OUT_MAX1);      // Almacenar el resgitro ADRESH en variable POT2
        }
        else if(PORTBbits.RB7 == 1){
            POT_4 = map(TEMPORAL, IN_MIN, IN_MAX, OUT_MIN1, OUT_MAX1);      // Almacenar el resgitro ADRESH en variable POT2
        }
        PIR1bits.SSPIF = 0;             // Limpiamos bandera de interrupción
    }
    return;
}

/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){
        __delay_ms(50);
        CCPR1L = (uint8_t)(POT_3>>2);                               // 8 bits mas significativos en CPR1L
        CCP1CONbits.DC1B = POT_3 & 0b11;
        
        CCPR2L = (uint8_t)(POT_4>>2);                               // 8 bits mas significativos en CPR1L 01010101"01" -> 01010101
        CCP2CONbits.DC2B0 = POT_4 & 0b01; 
        CCP2CONbits.DC2B1 = (POT_4 & 0b10)>>1;                      // 2 bits menos significativos en DC2B0 y DC2B1
    }
    return;
}

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){   
    TRISC = 0b00011000;         // SDI y SCK entradas, SD0 como salida
    
    // Configuración del oscilador interno    
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    // Configuración de puertos
    ANSEL = 0x00;
    ANSELH = 0x00;              // I/O digitales
        
    TRISA = 0b00100000;         // SS como entrada
    TRISB = 0b10000000;         // PORTD como salida
    
    PORTA = 0x00;               // Limpieza del PORTA
    PORTC = 0x00;               // Limpieza del PORTC
    PORTB = 0x00;               // Limpieza del PORTD
    
    // Configuración de interrucpiones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    PIR1bits.SSPIF = 0;         // Limpieza de bandera de SPI (Se debe limpiar manualmente por medio de software)
    PIE1bits.SSPIE = 1;         // Habilitar interrupciones de SPI
    
    // Configuración de SPI
    // Configuración del ESCLAVO
    // SSPCON <5:0>
    SSPCONbits.SSPM = 0b0100;   // SPI Esclavo, SS hablitado
    SSPCONbits.CKP = 0;         // Reloj inactivo en 0
    SSPCONbits.SSPEN = 1;       // Habilitamos pines de SPI
    // SSPSTAT<7:6>
    SSPSTATbits.CKE = 1;        // Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 0;        // Dato al final del pulso de reloj (Siempre debe estar apagado para esclavos)
    
    // Configuración PWM
    TRISCbits.TRISC2 = 1;           // Deshabilitar salida de CCP1 (Se pone como entrada)
    TRISCbits.TRISC1 = 1;           // Deshabilitar salida de CCP2 (Se pone como entrada)
    PR2 = 250;                      // Periodo de 4 ms
    
    // Configuracion CCP
    CCP1CON = 0;                    // Apagar CCP1
    CCP2CON = 0;
    CCP1CONbits.P1M = 0;            // Modo sigle output
    CCP1CONbits.CCP1M = 0b1100;     // PWM
    CCP2CONbits.CCP2M = 0b1100;     // PWM
    
    CCPR1L = 125>>2;                 // REVISAR   <-----------------
    CCP1CONbits.DC1B = 125 & 0b11;   // 
    CCPR2L = 125>>2;
    CCP2CONbits.DC2B0 = 125 & 0b01; 
    CCP2CONbits.DC2B1 = 125 & 0b10;  // 
    
    PIR1bits.TMR2IF = 0;            // Limpiar bandera de TMR2
    T2CONbits.T2CKPS = 0b01;        // Prescaler 1:4
    T2CONbits.TMR2ON = 1;           // Encender TMR2
    while (!PIR1bits.TMR2IF);       // Esperar un ciclo del TMR2
    PIR1bits.TMR2IF = 0;
    
    TRISCbits.TRISC2 = 0;           // Habilitar salida de PWM
    TRISCbits.TRISC1 = 0;           // Habilitar salida de PWM
}

/*------------------------------------------------------------------------------
 * FUNCIONES 
 ------------------------------------------------------------------------------*/
// Función para hacer la interpolación lineal del valor de la entrada analógica 
unsigned short map(uint8_t x, uint8_t x0, uint8_t x1, 
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}