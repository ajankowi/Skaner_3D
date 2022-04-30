#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_



//Odpowiednie wyjscia dla silnika 1

#define SM_1_IN1_DIR		DDRD
#define SM_1_IN1_PORT		PORTD
#define SM_1_IN1			PD0

#define SM_1_IN2_DIR		DDRD
#define SM_1_IN2_PORT		PORTD
#define SM_1_IN2			PD1

#define SM_1_IN3_DIR		DDRD
#define SM_1_IN3_PORT		PORTD
#define SM_1_IN3			PD2

#define SM_1_IN4_DIR		DDRD
#define SM_1_IN4_PORT		PORTD
#define SM_1_IN4			PD3

//Odpowiednie wyjscia dla silnika 2

#define SM_2_IN1_DIR		DDRC
#define SM_2_IN1_PORT		PORTC
#define SM_2_IN1			PC4

#define SM_2_IN2_DIR		DDRC
#define SM_2_IN2_PORT		PORTC
#define SM_2_IN2			PC5

#define SM_2_IN3_DIR		DDRC
#define SM_2_IN3_PORT		PORTC
#define SM_2_IN3			PC6

#define SM_2_IN4_DIR		DDRC
#define SM_2_IN4_PORT		PORTC
#define SM_2_IN4			PC7









#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef inb
#define	inb(addr)			(addr)
#endif

#ifndef outb
#define	outb(addr, data)	addr = (data)
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg,bit)		reg ^= (_BV(bit))
#endif

/*
 *  Gotowe zaimplementowane: */
 #define 	bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
 #define 	bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))
 #define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
 #define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))


// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))


//  Funkcja inicjalizuje Timer
//

void TimerInit();

void half_step_motor_left(uint32_t angle, uint8_t number);


void half_step_motor_right(uint32_t angle, uint8_t number);


void full_step_motor_left(uint32_t angle, uint8_t number);


void full_step_motor_right(uint32_t angle, uint8_t number);



#endif /* STEPMOTOR_H_ */
