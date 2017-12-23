/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  AIC3204 Tone
 *
 */
#include "stdio.h"
#include "usbstk5515.h"
#include <math.h>
extern Int16 AIC3204_rset( Uint16 regnum, Uint16 regval);
#define XmitL 0x10
#define XmitR 0x20
#define pi 3.14
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204 Tone                                                            *
 *      Output a 1 kHz tone through the HEADPHONE jack                      *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 aic3204_tone_headphone( )
{
    /* Pre-generated sine wave data, 16-bit signed samples */
    Int16 sinetable[48] = {
        0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
        0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
        0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
        0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
        0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
        0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
    };
    Int16 j, i = 0;
    Int16 sample;

    /* Configure AIC3204 */
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 1, 1 );      // Reset codec
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
    AIC3204_rset( 0, 0 );
    /* PLL and Clocks config and Power Up  */
    AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master)
    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6, 7 );      // PLL setting: J=7
    AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D=1680)
    AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D=1680)
    AIC3204_rset( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5, 0x91 );   // PLL setting: Power up PLL, P=1 and R=1
    AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2
    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
    AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 64, 2 );     // Left vol=right vol
    AIC3204_rset( 65, 0);      // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x10, 0x00 );// Unmute HPL , 0dB gain
    AIC3204_rset( 0x11, 0x00 );// Unmute HPR , 0dB gain
    AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    USBSTK5515_wait( 100 );    // wait
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x34, 0x30 );// STEREO 1 Jack
		                       // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 0x37, 0x30 );// IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 0x36, 3 );   // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 0x39, 0xc0 );// CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 0x3b, 0 );   // MIC_PGA_L unmute
    AIC3204_rset( 0x3c, 0 );   // MIC_PGA_R unmute
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
    AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
    
    AIC3204_rset( 0, 0 );    
    USBSTK5515_wait( 200 );    // Wait
    
    /* I2S settings */
    I2S0_SRGR = 0x0;
    I2S0_CR = 0x8010;    // 16-bit word, slave, enable I2C
    I2S0_ICMR = 0x3f;    // Enable interrupts
    
    Int16 output;
        float y[] = {0.0,0.0,0.0};
        float y2[] = {0.0,0.0,0.0};
        float y3[] = {0.0,0.0,0.0};
        float y5[] = {0.0,0.0,0.0};
        float y4[] = {0.0,0.0,0.0};
        float y6[] = {0.0,0.0,0.0};
        float a = 2*cos(2*pi/800);
        float a2 = 2*cos(6*pi/800);
        float a3 = 2*cos(10*pi/800);
        float a4 = 2*cos(14*pi/800);
        float a5 = 2*cos(18*pi/800);
        float a6 = 2*cos(22*pi/800);
        float b = -1.0;
        y[1]=sin(2*pi/800);
        y2[1] = sin(6*pi/800);
        y3[1] = sin(10*pi/800);
        y4[1]=sin(14*pi/800);
        y5[1] = sin(18*pi/800);
        y6[1] = sin(22*pi/800);
for(;;){
		while((XmitR & I2S0_IR) ==0);
			y[0]= a*y[1] + b*y[2];
			y[2] =y[1];
			y[1] = y[0];
			y2[0]= a2*y2[1] + b*y2[2];
			y2[2] =y2[1];
			y2[1] = y2[0];
			y3[0]= a3*y3[1] + b*y3[2];
			y3[2] =y3[1];
			y3[1] = y3[0];
			y4[0]= a4*y4[1] + b*y4[2];
			y4[2] =y4[1];
			y4[1] = y4[0];
			y5[0]= a5*y5[1] + b*y5[2];
			y5[2] =y5[1];
			y5[1] = y5[0];
			y6[0]= a6*y6[1] + b*y6[2];
			y6[2] =y6[1];
			y6[1] = y6[0];
			output = y[0]*pow(2,15) + y2[0]*pow(2,15)/3 + (y3[0]*pow(2,15)/5) + (y4[0]*pow(2,15)/7) + (y5[0]*pow(2,15)/9) + (y6[0]*pow(2,15)/11);
//output = y[0]*pow(2,15) - y2[0]*pow(2,15)/9 + (y3[0]*pow(2,15)/25) - (y4[0]*pow(2,15)/49) + (y5[0]*pow(2,15)/81) - (y6[0]*pow(2,15)/121); for triangular wave
			I2S0_W0_MSW_W = output;
			I2S0_W1_MSW_W = output;
       }

    /* Disble I2S */
    I2S0_CR = 0x00;
   
   
    return 0;
}


