/*
 * keypad.c
 *
 * Created: 5/8/2016 1:52:52 PM
 *  Author: alzhong    
 */ 
#include <avr/io.h>
#include "avr.h"
#include "keypad.h"

unsigned char keypadSymbols[] = {'Z','1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};



unsigned char pressed(unsigned char r,unsigned char c){
	
	DDRC=0x00;          //Set DDRC to all input
	PORTC=0xF0;         //Set PORTC3 to 'N/C'
	
	//Set that row to HIGH/'1'
	SET_BIT(DDRC,r);    //Set rth DDR to output
	CLR_BIT(PORTC,r);   //Set rth PORT to '0'
	
	//Set column C to 'Z'
	SET_BIT(PORTC,c+4);
	
	//Return 1 if pressed, 0 if not
	return (GET_BIT(PINC,c+4)) ? 0 : 1;
}

//Gets the position on the keyboard (r*4 + c + 1)
//by iterating through the buttons and calling
//the *pressed* function
unsigned char get_key(){
	unsigned char r,c;
	for(r=0;r<4;++r){
		for(c=0;c<4;++c){
			if(pressed(r,c)){
				return(r*4 + c + 1);
			}
		}
	}
	return 0;
}

unsigned char keypad_symbol_pressed(){
    return keypadSymbols[get_key()];	
}
