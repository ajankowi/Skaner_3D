#include <avr/interrupt.h>
#include "millis.h"

//Automatycznie ustawia Timer

volatile uint32_t g_Millis=0;		//Global var. for counting milliseconds.
									//Don't touch without disabling interrupts.
	//Inicjalizacja TIMER1
void initMillis() {
	TCCR1A = (1<<WGM13);
	TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10);//TOP=OCR1A, 1024 prescaler
	OCR1A = T0_RELOAD;				//244: Przerwanie co 15.68 ms
	TIMSK = (1<<OCIE1A);
}

//Zwraca ilsc milisekind od startu
uint32_t millis(){
    uint32_t m;
    uint8_t oldSREG = SREG;
     // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();
    m = g_Millis;
    SREG = oldSREG;
    return m;
}

ISR( TIMER1_COMPA_vect ){			//Wywo³ywana co 15.68 ms
	static uint8_t usFract=0;
	g_Millis += MILLIS_INC;
	usFract  += MILLIS_INC_FRACT>>3;// 680 / 8 =  85.0
	if( usFract >= 1000>>3 ){		//1000 / 8 = 125.0
		usFract -= 1000>>3;			//Fractional part added up to 1 ms
		g_Millis++;
	}
}
