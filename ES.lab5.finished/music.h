/*
* music.h
*
* Created: 6/3/2016
* Author: andyl8
*/
 
#ifndef MUSIC_H_
#define MUSIC_H_

#define C0	382
#define Cs0 361
#define D0  341
#define Ds0 321
#define E0  303
#define F0  286
#define Fs0 270
#define G0  255
#define Gs0 241
#define A0  227
#define As0 215
#define B0  202
#define C1  191
#define Cs1 180
#define D1  170
#define Ds1 161
#define E1  152
#define F1  143
#define Fs1 135
#define G1  128
#define Gs1 120
#define A1  114
#define As1 107
#define B1  101
#define C2   95
#define Cs2  90
#define D2   85
#define Ds2  80
#define E2   76
#define F2   72
#define Fs2  68
#define G2   64
#define Gs2  60
#define A2   57
#define As2  54
#define B2   51
#define C3   48
#define Cs3  45
#define D3   43

#define WHOLE     400
#define HALF      200
#define QUARTER   100
#define EIGHTH     50
#define SIXTEENTH  25
#define THSECOND   13

typedef struct {
	unsigned short freq;
	unsigned int duration;
} Note;

void playNote(unsigned short freq, unsigned int duration);
void playMusic(Note* song, int n);

#endif /* MUSIC_H_ */