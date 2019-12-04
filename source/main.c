/*
 * Final Project
 * Linus Im - 862043824
 * Code regarding LCD Custom Characters was retrieved from ElectronicWings.com at https://www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-
 *
 * */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
#define DATA_BUS PORTC
#define CONTROL_BUS PORTD
#define RS 6
#define E 7




volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerISR() {
	TimerFlag = 1;
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}



void delay_ms(int miliSec) {
	int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2);
}

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);						 
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) {
	  LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);
   }
}

void LCD_DisplayString( unsigned char column, const char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}













unsigned char direction = 0x00; // Variable storing either nothing, left (0x01), or right (0x02)
unsigned char button = 0x00; // Variable storing button press (0x01), or no press (0x00)
unsigned char score = 0x00; // Variable storing victory count (up to 5)
unsigned char button3; // Declaration of button3 (A2)

unsigned char testAlien = 0x04; // 
unsigned char testPlayer = 0x04; // Two variables locating alien and player (0x01 to 0x07)

enum Button_states { Off, Press, Dud } Button_state;
void ButtonTick() {
	switch(Button_state) {
		case Off:
			if (button3) {
				Button_state = Press;
			} else {
				Button_state = Off;
			}
			break;
		case Press:
			Button_state = Dud;
			break;
		case Dud:
			if (!button3){
				Button_state = Off;
			} else {
				Button_state = Dud;
			}
			break;
		default:
			break;
	}

	switch(Button_state) {
		case Off:
			button = 0x00;
			break;
		case Press:
			button = 0x01;
			break;
		case Dud:
			button = 0x00;
			break;
		default:
			break;
	}
}

enum Player_states { Player1, Player2, Player3, Player4, Player5, Player6, Player7 } Player_state;
void PlayerTick() {
	switch(Player_state) {
		case Player1:
			if (direction == 0x02) {
				Player_state = Player2;
			} else {
				Player_state = Player1;
			}
			break;
		case Player2:
			if (direction == 0x01) {
				Player_state = Player1;
			} else if (direction == 0x02) {
				Player_state = Player3;
			} else {
				Player_state = Player2;
			}
			break;
		case Player3:
			if (direction == 0x01) {
				Player_state = Player2;
			} else if (direction == 0x02) {
				Player_state = Player4;
			} else {
				Player_state = Player3;
			}
			break;
		case Player4:
			if (direction == 0x01) {
				Player_state = Player3;
			} else if (direction == 0x02) {
				Player_state = Player5;
			} else {
				Player_state = Player4;
			}
			break;
		case Player5:
			if (direction == 0x01) {
				Player_state = Player4;
			} else if (direction == 0x02) {
				Player_state = Player6;
			} else {
				Player_state = Player5;
			}
			break;
		case Player6:
			if (direction == 0x01) {
				Player_state = Player5;
			} else if (direction == 0x02) {
				Player_state = Player7;
			} else {
				Player_state = Player6;
			}
			break;
		case Player7:
			if (direction == 0x01) {
				Player_state = Player6;
			} else {
Player_state = Player7;
			}
			break;
		default:
			break;
	}
	
	switch (Player_state) {
		case Player1: 
			testPlayer = 0x01;
			break;
		case Player2:
			testPlayer = 0x02;
			break;
		case Player3: 
			testPlayer = 0x03;
			break;
		case Player4:
			testPlayer = 0x04;
			break;
		case Player5: 
			testPlayer = 0x05;
			break;
		case Player6:
			testPlayer = 0x06;
			break;
		case Player7: 
			testPlayer = 0x07;
			break;
		default:
			break;
	}
}

enum Alien_states { Alien1, Alien2, Alien3, Alien4, Alien5, Alien6, Alien7 } Alien_state;
void AlienTick() {
	switch(Alien_state) {
		case Alien1:
			Alien_state = Alien2;
			break;
		case Alien2:
			Alien_state = Alien3;
			break;
		case Alien3:
			Alien_state = Alien4;
			break;
		case Alien4:
			Alien_state = Alien5;
			break;
		case Alien5:
			Alien_state = Alien6;
			break;
		case Alien6:
			Alien_state = Alien7;
			break;
		case Alien7:
			Alien_state = Alien1;
			break;
		default:
			break;
	}
	
	switch (Alien_state) {
		case Alien1: 
			testAlien = 0x01;
			break;
		case Alien2: 
			testAlien = 0x02;
			break;
		case Alien3: 
			testAlien = 0x03;
			break;
		case Alien4: 
			testAlien = 0x04;
			break;
		case Alien5: 
			testAlien = 0x05;
			break;
		case Alien6: 
			testAlien = 0x06;
			break;
		case Alien7: 
			testAlien = 0x07;
			break;
		default:
			break;
	}
}



int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char char1[8] = { 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00 };
	unsigned char char2[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 };	

	uint8_t scoreData;
	scoreData = eeprom_read_byte((uint8_t*)46);

	if (scoreData == 0x00) {
		score = 0x00;
	} else if (scoreData == 0x01) {
		score = 0x01;
	} else if (scoreData == 0x02) {
		score = 0x02;
	} else if (scoreData == 0x03) {
		score = 0x03;
	} else if (scoreData == 0x04) {
		score = 0x04;
	} else if (scoreData == 0x05) {
		score = 0x05;
	}

	TimerSet(400);
	TimerOn();
	Button_state = Off;
	Player_state = Player4;
	Alien_state = Alien4;
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);	

    while (1) {
	button3 = ~PINA & 0x04;
	ButtonTick();
    	PlayerTick();
	AlienTick();
   	
	LCD_ClearScreen();
	
	if ( (testAlien == testPlayer) && (button == 0x01) ) {
		LCD_Cursor(1);
		LCD_WriteData('V');
		LCD_WriteData('i');
		LCD_WriteData('c');
		LCD_WriteData('t');
		LCD_WriteData('o');
		LCD_WriteData('r');
		LCD_WriteData('y');
		if (score != 0x05) {
			score = score + 1;
		}		
	} else {

 	
	if (testAlien == 0x01) {
		LCD_Cursor(1);
		LCD_WriteData(char1);
	} else if (testAlien == 0x02) {
		LCD_Cursor(2);
		LCD_WriteData(char1);
   	} else if (testAlien == 0x03) {
		LCD_Cursor(3);
		LCD_WriteData(char1);
	} else if (testAlien == 0x04) {
		LCD_Cursor(4);
		LCD_WriteData(char1); 
	} else if (testAlien == 0x05) {
		LCD_Cursor(5);
		LCD_WriteData(char1);
   	} else if (testAlien == 0x06) {
		LCD_Cursor(6);
		LCD_WriteData(char1);
	} else if (testAlien == 0x07) {
		LCD_Cursor(7);
		LCD_WriteData(char1); 
	}
	
	LCD_Cursor(17);
	if (testPlayer == 0x01) {
		LCD_Cursor(17);
		LCD_WriteData(char2);
	} else if (testPlayer == 0x02) {
		LCD_Cursor(18);
		LCD_WriteData(char2);
   	} else if (testPlayer == 0x03) {
		LCD_Cursor(19);
		LCD_WriteData(char2);
	} else if (testPlayer == 0x04) {
		LCD_Cursor(20);
		LCD_WriteData(char2); 
	} else if (testPlayer == 0x05) {
		LCD_Cursor(21);
		LCD_WriteData(char2);
   	} else if (testPlayer == 0x06) {
		LCD_Cursor(22);
		LCD_WriteData(char2);
	} else if (testPlayer == 0x07) {
		LCD_Cursor(23);
		LCD_WriteData(char2); 
	}

      }
	
	LCD_Cursor(16);
	if (score == 0x00) {
		LCD_WriteData('0');
		eeprom_write_byte(46, 0x00);
	} else if (score == 0x01) {
		LCD_WriteData('1');
		eeprom_write_byte(46, 0x01);
	} else if (score == 0x02) {
		LCD_WriteData('2');
		eeprom_write_byte(46, 0x02);
	} else if (score == 0x03) {
		LCD_WriteData('3');
		eeprom_write_byte(46, 0x03);
	} else if (score == 0x04) {
		LCD_WriteData('4');
		eeprom_write_byte(46, 0x04);
	} else if (score == 0x05) {
		LCD_WriteData('5');
		eeprom_write_byte(46, 0x05);
	}

	while(!TimerFlag);
	TimerFlag = 0;			
    }
    return 1;
}
