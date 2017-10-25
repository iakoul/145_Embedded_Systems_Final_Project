/*
 * 145a.project5.simon_says.c
 *
 * Created: 6/5/2016 1:28:13 AM
 * Author : alzhong, andyl8
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "avr.h"
#include "lcd.h"
#include "music.h"
#include "keypad.h"

Note LED_Notes[] = {
	{C0, QUARTER}, {E0, QUARTER}, {G0, QUARTER},
	{C1, QUARTER}, {E1, QUARTER}, {G1, QUARTER},
	{C2, QUARTER}, {E2, QUARTER}, {G2, QUARTER}
};

Note Success[] = {
	{C1, EIGHTH}, {C1, EIGHTH}, {C1, EIGHTH}, {C3, QUARTER}
};

Note Failure[] = {
	{C2, EIGHTH}, {B1, EIGHTH}, {As1, EIGHTH}, {A1, HALF}
};

Note Victory[] = {
    {C2, EIGHTH+SIXTEENTH}, {D2, SIXTEENTH}, {E2, EIGHTH}, 
	{F2, EIGHTH},			{G2, EIGHTH},	 {C3, HALF} 
};

#define LED_OUTPUT_DDR          DDRA
#define LED_OUTPUT_PORT         PORTA

#define SPEAKER_OUTPUT_DDR      DDRB
#define SPEAKER_OUTPUT_PORT     PORTB

#define KEYPAD_INPUT_DDR        DDRC

typedef enum {
    INIT,
    PLAYING_PATTERN,
    INPUT_MODE,
    GAME_WIN,
    GAME_LOSE,
    PLAY_AGAIN,
	PIANO
} GAME_STATES;

unsigned char patterns0[3] = {1,5,6};
unsigned char patterns1[5] = {1,5,6,7,8};
unsigned char patterns2[7] = {1,2,3,4,4,2,1};

void clr_all_leds(){
    LED_OUTPUT_PORT = 0x00;
}

void light_up_single_led(int num){
    switch(num){
        case(1):
            SET_BIT(LED_OUTPUT_PORT,0);
            CLR_BIT(LED_OUTPUT_PORT,1);
            CLR_BIT(LED_OUTPUT_PORT,2);

            CLR_BIT(LED_OUTPUT_PORT,3);
            SET_BIT(LED_OUTPUT_PORT,4);
            SET_BIT(LED_OUTPUT_PORT,5);
            break;
        case(4):
            SET_BIT(LED_OUTPUT_PORT,0);
            CLR_BIT(LED_OUTPUT_PORT,1);
            CLR_BIT(LED_OUTPUT_PORT,2);

            SET_BIT(LED_OUTPUT_PORT,3);
            CLR_BIT(LED_OUTPUT_PORT,4);
            SET_BIT(LED_OUTPUT_PORT,5);
            break;
        case(7):
            SET_BIT(LED_OUTPUT_PORT,0);
            CLR_BIT(LED_OUTPUT_PORT,1);
            CLR_BIT(LED_OUTPUT_PORT,2);

            SET_BIT(LED_OUTPUT_PORT,3);
            SET_BIT(LED_OUTPUT_PORT,4);
            CLR_BIT(LED_OUTPUT_PORT,5);
            break;
        case(2):
            CLR_BIT(LED_OUTPUT_PORT,0);
            SET_BIT(LED_OUTPUT_PORT,1);
            CLR_BIT(LED_OUTPUT_PORT,2);

            CLR_BIT(LED_OUTPUT_PORT,3);
            SET_BIT(LED_OUTPUT_PORT,4);
            SET_BIT(LED_OUTPUT_PORT,5);
            break;
        case(5):
            CLR_BIT(LED_OUTPUT_PORT,0);
            SET_BIT(LED_OUTPUT_PORT,1);
            CLR_BIT(LED_OUTPUT_PORT,2);

            SET_BIT(LED_OUTPUT_PORT,3);
            CLR_BIT(LED_OUTPUT_PORT,4);
            SET_BIT(LED_OUTPUT_PORT,5);
            break;
        case(8):
            CLR_BIT(LED_OUTPUT_PORT,0);
            SET_BIT(LED_OUTPUT_PORT,1);
            CLR_BIT(LED_OUTPUT_PORT,2);

            SET_BIT(LED_OUTPUT_PORT,3);
            SET_BIT(LED_OUTPUT_PORT,4);
            CLR_BIT(LED_OUTPUT_PORT,5);
            break;
        case(3):
            CLR_BIT(LED_OUTPUT_PORT,0);
            CLR_BIT(LED_OUTPUT_PORT,1);
            SET_BIT(LED_OUTPUT_PORT,2);

            CLR_BIT(LED_OUTPUT_PORT,3);
            SET_BIT(LED_OUTPUT_PORT,4);
            SET_BIT(LED_OUTPUT_PORT,5);
            break;
        case(6):
            CLR_BIT(LED_OUTPUT_PORT,0);
            CLR_BIT(LED_OUTPUT_PORT,1);
            SET_BIT(LED_OUTPUT_PORT,2);

            SET_BIT(LED_OUTPUT_PORT,3);
            CLR_BIT(LED_OUTPUT_PORT,4);
            SET_BIT(LED_OUTPUT_PORT,5);
            break;
        case(9):
            CLR_BIT(LED_OUTPUT_PORT,0);
            CLR_BIT(LED_OUTPUT_PORT,1);
            SET_BIT(LED_OUTPUT_PORT,2);

            SET_BIT(LED_OUTPUT_PORT,3);
            SET_BIT(LED_OUTPUT_PORT,4);
            CLR_BIT(LED_OUTPUT_PORT,5);
            break;
    }
}


void setup_led_output_ports(){
    for(unsigned char i = 0; i < 6; ++i){
        SET_BIT(LED_OUTPUT_DDR,i);        
    }
}

void setup_speaker_output_ports(){
    SET_BIT(SPEAKER_OUTPUT_DDR,3);
}

void game_win(){
    for(int i = 1; i < 10; ++i){
        light_up_single_led(i);
        wait_avr(10000);
    }
}

void game_lose(){
    for(int i = 9; i > 0; --i){
        light_up_single_led(i);
        wait_avr(10000);
    }
}

void generate_sound(unsigned char* randomList, Note* music_converted, unsigned char num){
	for(int i = 0; i < num; i++){
		music_converted[i] = LED_Notes[randomList[i]-1];
	}
}

Note generate_single(unsigned char key){
	return LED_Notes[key-1];
}

int main(void){
    setup_led_output_ports();
    setup_speaker_output_ports();
	
	ini_lcd();
	clr_lcd();

    GAME_STATES game_state = INIT;
    
    unsigned char button_pressed = 0;
    unsigned char round = 0;
    unsigned char pressed_index = 0;
    unsigned char current_size_of_pattern = 0;
	
	char round_num[2];

    Note music_converted_0[9];
	Note music_converted_1[9];
	Note music_converted_2[9];

    while(1){
        switch(game_state){
            case INIT:
			
				generate_sound(patterns0, music_converted_0, 3);
				generate_sound(patterns1, music_converted_1, 5);
				generate_sound(patterns2, music_converted_2, 7);
				
				wait_avr(30000);
				clr_lcd();
				puts_lcd2("Simon Says Game");
				pos_lcd(1,0);
				puts_lcd2("A to begin");
				
				button_pressed = keypad_symbol_pressed();
				
				round = 0;
				pressed_index = 0;
				current_size_of_pattern = 0;
				
				if(button_pressed == 'A') {
					game_state = PLAYING_PATTERN;
				}
                break;
                
            case PLAYING_PATTERN:
				clr_lcd();
				puts_lcd2("Listen!");
				
                switch(round){
                    case 0:
                        current_size_of_pattern = sizeof(patterns0)/sizeof(unsigned char);
                        for(unsigned char i = 0; i < current_size_of_pattern; ++i){
                            light_up_single_led(patterns0[i]);
                            playNote(music_converted_0[i].freq,music_converted_0[i].duration);
                        }
						
                        clr_all_leds();
                        game_state = INPUT_MODE;
                        break;
						
                    case 1:
                        current_size_of_pattern = sizeof(patterns1)/sizeof(unsigned char);
                        for(unsigned char i = 0; i < current_size_of_pattern; ++i){
                            light_up_single_led(patterns1[i]);
                            playNote(music_converted_1[i].freq,music_converted_1[i].duration);
                        }
						
                        game_state = INPUT_MODE;
                        clr_all_leds();
                        break;
						
                    case 2:
                        current_size_of_pattern = sizeof(patterns2)/sizeof(unsigned char);
                        for(unsigned char i = 0; i < current_size_of_pattern; ++i){
                            light_up_single_led(patterns2[i]);
                            playNote(music_converted_2[i].freq,music_converted_2[i].duration);
                        }
						
                        game_state = INPUT_MODE;
                        clr_all_leds();
                        break;
	
                }
                break;
                
            case INPUT_MODE:
				sprintf(round_num, "Round %d:", (round+1));
				clr_lcd();
				puts_lcd2(round_num);
				
                do { button_pressed = keypad_symbol_pressed(); }
				while(button_pressed == 'Z' || ((button_pressed - '0') < 1) || (button_pressed - '0') > 9);

                button_pressed = button_pressed - '0';
                
                light_up_single_led(button_pressed);
                Note pressed_note= generate_single(button_pressed);
                playNote(pressed_note.freq, pressed_note.duration);
                clr_all_leds();
						                   
                switch(round){
                    case 0:
                        if((button_pressed) != (patterns0[pressed_index]) ) {
                            game_state = GAME_LOSE;
                        }
						else {
							++pressed_index;
							if (pressed_index == current_size_of_pattern) {
								clr_lcd();
								puts_lcd2("Nice!");
								pos_lcd(1,0);
								puts_lcd2("Next Level:");
								playMusic(Success, 4);
								wait_avr(60000);
								++round;
								pressed_index = 0;
								game_state = PLAYING_PATTERN;
							}
                        }
                        break;
                    case 1:
                        if((button_pressed) != (patterns1[pressed_index])) {
                            game_state = GAME_LOSE;
                        }
                        else {
							++pressed_index;
							if (pressed_index == current_size_of_pattern) {
								clr_lcd();
								puts_lcd2("Nice!");
								pos_lcd(1,0);
								puts_lcd2("Next Level:");
								playMusic(Success, 4);
								wait_avr(60000);
								++round;
								pressed_index = 0;
								game_state = PLAYING_PATTERN;
							}
						}
                        break;
                    case 2:
                        if ((button_pressed) != (patterns2[pressed_index])) {
                            game_state = GAME_LOSE;
                        }
                        else {
							++pressed_index;
							if (pressed_index == current_size_of_pattern) {
								playMusic(Success, 4);
								game_state = GAME_WIN;
							}
						}
                        break;
                }
                break;
                
            case GAME_WIN:
				wait_avr(30000);
				clr_lcd();
				puts_lcd2("You win!");
				
				playMusic(Victory, 6);
				game_win();
				clr_all_leds();
				
                game_state = PLAY_AGAIN;
                break;
				
            case GAME_LOSE:
				wait_avr(30000);
				clr_lcd();
				puts_lcd2("Game Over!");
				
				playMusic(Failure, 4);
				game_lose();
				clr_all_leds();
				
                game_state = PLAY_AGAIN;
                break;
				
            case PLAY_AGAIN:
				wait_avr(30000);
				clr_lcd();
				puts_lcd2("Play again?");
				pos_lcd(1,0);
				puts_lcd2("Yes: A     No: B");
				
				button_pressed = keypad_symbol_pressed();
				if(button_pressed == 'A') {
					game_state = INIT;
				}
				else if(button_pressed == 'B') {
					game_state = PIANO;
				}
                break;
				
			case PIANO:
				wait_avr(30000);
				clr_lcd();
				puts_lcd2("Piano mode!");
				while(1){
					do{
						button_pressed = keypad_symbol_pressed();
					} while (button_pressed == 'Z');
        
					button_pressed = button_pressed - '0';
					light_up_single_led(button_pressed);
					Note test = generate_single(button_pressed);
					playNote(test.freq, test.duration);
					clr_all_leds();
				}
            }
    }
}