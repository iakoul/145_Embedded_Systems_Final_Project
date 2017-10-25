/*
* music.c
*
* Created: 6/3/2016
* Author: andyl8
*/
 
#include <avr/io.h>
#include "avr.h"
#include "music.h"

void playNote(unsigned short freq, unsigned int duration){
	int i;
	unsigned long d = 200*duration/freq;
	for(i = 0; i < d; ++i) {
		SET_BIT(PORTB, 3);
		wait_avr(freq);
		CLR_BIT(PORTB, 3);
		wait_avr(freq);
	}
 }
 
void playMusic(Note* song, int n){
	int i;
	for (int i = 0; i < n; ++i) {
		playNote(song[i].freq, song[i].duration);
	}
}