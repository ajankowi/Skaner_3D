#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/i2cmaster.h"
#include "util/millis.h"
#include "util/VL53L0X.h"
#include "util/HD44780.h"
#include "util/StepMotor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "ff.h"
#include "diskio.h"
#include "mmc_avr.h"



FATFS FatFs;

FIL Fil;
unsigned int bw;


// Constants
#define BAUD 			115200	// Serial baudrate for user interface


/*-  Defination of Vectors----------------------------------------*/

#define TIMER0_COMP_VECTOR		__vector_10
#define TIMER1_COMPA_VECTOR  	__vector_7
#define TIMER1_COMPB_VECTOR 	__vector_8
#define TIMER2_COMP_VECTOR      __vector_4
#define TIMER2_OVF			    __vector_5

/*-  Definition of ISR -------------------------------------------*/

#define Timer_ISR(N)            \
void N (void)__attribute((signal,used)); \
void N (void)

FILINFO Finfo;

volatile UINT Timer;	/* Performance timer (100Hz increment) */



static
void ioinit (void)
{
	/* Timer 2 - co 10ms, dla karty SD */
		TCCR2 |= ((1 << WGM21 | 1 << CS22 | 1 << CS21) | (1 << CS20));				//preskaler 1024, tryb CTC
		OCR2 = 78;
		TIMSK |= (1 << OCIE2);								//zezwolenie na przerwanie od Timera 0
		sei();
}

void kalibruj(){

	uint16_t pomiar = 0;						// Zmienne pomocnicze
	statInfo_t xTraStats;
	char text[20];
	uint8_t flaga = 1;

	LCD_Clear();
	sprintf(text, "   Kalibracja");				// Wyswietla komunikat
	LCD_GoTo(0, 0);
	LCD_WriteText(text);
	sprintf(text, "    skanera");				// Wyswietla komunikat
	LCD_GoTo(0, 1);
	LCD_WriteText(text);


	while(flaga){
		half_step_motor_left(256,2);			// uruchamia silnik krokowy
		pomiar = readRangeSingleMillimeters( &xTraStats );		// Dokonuje pomiaru odleglosci

		if(pomiar < 200){						// Jezeli pomiar jest mniejszy od 20cm
			flaga = 0;
			LCD_Clear();
			sprintf(text, "  Skalibrowano");	// Wyswietla komunikat
			LCD_GoTo(0, 0);
			LCD_WriteText(text);
			sprintf(text, "     skaner");		// Wyswietla komunikat
			LCD_GoTo(0, 1);
			LCD_WriteText(text);
		}

	}

	half_step_motor_right(512,2);				// Uruchamia 2 silnik krokowy

}



int main(){

	 statInfo_t xTraStats;
	 init();											// Inicjalizacja timera dla czujnika odleg³oœci

	 initVL53L0X(1);									// Inicjalizacja czujnika odleg³oœci

	 ioinit();											// Inicjalizacja po³¹czenia z kart¹
	 disk_initialize(0);								// inicjalizacja karty SD

	 setMeasurementTimingBudget( 500 * 1000UL );		// Ustawienie prescalera na 500ms

	 LCD_Initalize();									// Inicjalizacja wyœwietlacza
	 LCD_Home();

	 	 	 	 	 	 	 	 	 	 	 	 	 	 // Ustawienie PD0 jako PULL-UP
	 cbi(DDRA,7);
	 sbi(PORTA,7);

	 char text[20];										// Tablica pomocnicza
	 uint16_t pomiar = 0;								// Zmienne pomocnicze
	 uint16_t licznik = 0;
	 uint16_t Ppros = 0;								// Dlugosc przeciwprostokatnej

	 uint16_t x = 0;									// Wspolrzedne punktu
	 uint16_t y = 0;
	 uint16_t z = 0;

	 uint16_t r = 130;									// Odleglosc czujnika od srodka
	 float kat = 0;

	 FRESULT fr;     									// Zmienna dla inicjalizacji karty SD
	 char name[12];										// Bufory
	 char buf[50];
	 int b = 0;

	sprintf(text, "   Skaner 3D");						// Wyœwietlanie tekstu
	LCD_Clear();
	LCD_GoTo(0, 0);
	LCD_WriteText(text);

	_delay_ms(5000);

	fr = f_mount(&FatFs, "", 1);						// Po³¹czenie siê z kart¹ SD
	_delay_ms(50);

	while(fr != 0){										// Wyœwietlenie komunikatu o bêdzie
		LCD_Clear();
		LCD_GoTo(0,0);
		LCD_WriteText("f_mount_FAILL");
		sprintf(text, "Blad nr - %d",fr);				// Zwraca numer b³êdu
		LCD_GoTo(0,1);
		LCD_WriteText(text);
		_delay_ms(4000);
		disk_initialize(0);								// ponowna inicjalizacja dysku
		fr = f_mount(&FatFs, "", 1);
	}
	do{
		sprintf(name, "skaner%d.txt",b);				// Tworzenie nowego pliku o danej nazwie
		fr = f_open(&Fil, name, FA_WRITE | FA_CREATE_NEW);
		f_sync(&Fil);									// Tworzenie pliku tekstowego
		b++;
		if(b>9){										// Mo¿liwe jest stworzenie 10 plików
			LCD_Clear();
			LCD_GoTo(0,0);
			LCD_WriteText("f_open_ERR");
			LCD_GoTo(0,1);
			LCD_WriteText("New_file");
			return 0;
		}
	}while(fr!=0);

	kalibruj();											// Kalibracja skanera

	while(!bit_is_clear(PINA,7)){}						// Czekanie na reakcje u¿ytkownika

	sprintf(text, "   Rozpoczeto" );					// Wyœwietlenie komunikatu
	LCD_Clear();
	LCD_GoTo(0,0);
	LCD_WriteText(text);
	LCD_GoTo(0,1);
	LCD_WriteText("   skanowanie");						// Wyœwietlenie komunikatu
	_delay_ms(4000);

	while((z<20)&&(licznik<40)){						// Pêtla powtarzaj¹ca siê 20 razu
		for(int a=1; a<65 ;a++){

			pomiar = readRangeSingleMillimeters( &xTraStats ) - 10;		// Dokonanie pomiaru odleg³oœci

			if(pomiar<r){								// Odrzucanie b³êdnych pomiarów
				licznik = 0;
				kat = (2*M_PI*a*8)/512;					// Oblicza k¹t w radianach
				Ppros = r - pomiar;						// Oblicza odleg³osc od srodka silnika krokowego
				x = Ppros*cosf(kat);					// Wspolrzedna X
				y = Ppros*sinf(kat);					// Wspolrzedna Y
				sprintf(buf, "%d,%d,%d\n",x,y,3*z );	// Zapisanie wspolrzednych do bufora
				f_write(&Fil, buf, sizeof(char)*strlen(buf), &bw);	// Zapisanie bufora na karte SD

				f_sync(&Fil);							// Funkcja zabezpieczajaca otwerty plik txt w razie naglego unieruchomienia
			}											// Zapisane dane nie zostaja stracone
			else{
				licznik++;								// Zwiekszenie licznika
			}
			half_step_motor_right(8,1);					// Obrot silnikiem krokowym

		}
		z++;											// Zwiekszenie zakresu

		sprintf(text, "    Z %d z 20 ",z );				// Wsywietla komunikat
		LCD_Clear();
		LCD_GoTo(0,1);
		LCD_WriteText(text);

		half_step_motor_right(1024,2);					// uruchamia 2 silnik krokowy
	}

	 f_close(&Fil);										// Zamyka plik txt
	 LCD_Clear();
	 LCD_GoTo(0,0);
	 LCD_WriteText("   Skonczono");						// Wyswietla komunikat
	 LCD_GoTo(0,1);
	 LCD_WriteText("  skanowanie!");					// Wyswietla komunikat

	 return 0;
}




ISR(TIMER2_COMP_VECTOR)
{
	Timer++;			/* Performance counter for this module */
	disk_timerproc();	/* Drive timer procedure of low level disk I/O module */
}






							// Fukcja zwracajaca RTC do zapisu plikow
DWORD get_fattime(void)
{
 return ((DWORD)(2013 - 1980) << 25) /*Year 2013*/
				| ((DWORD)7 << 21)   /*Month 6*/
				| ((DWORD)28 << 16)  /*Mday 28*/
				| ((DWORD)0 << 11)   /*Hour 0*/
				| ((DWORD)0 << 5)    /*Min 0*/
				| ((DWORD)0 << 1);    /*Sec 0*/
}
