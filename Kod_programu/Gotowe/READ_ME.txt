************************************
	Autor: Adam Jankowiak
	Przedmiot: PTM_2
	Data: 02.02.2022	
************************************

1.Obsługa karty SD - najbardziej skomplikowane
- diskio.h
- dioskio.c
- ff.h
- ff.c
- ffconf.h
- ffunicode_avr.c
- mmc_avr.h
- mmc_avr_spi.c

Należy przestawić bity w mmc_avr_spi na daną atmege, dodatkowo ustawić Timer 0 w main.c
Wyrzucić wszystko związane z UART-em wyśiwtelaniem komunikatów na komputerze itd.

Uwaga WAŻNE!
W pliku diskio.c należy w wszystkich funkcjach disk_* zakomentować if-y, z niewiadomego powodu program
samodzielnie nie wybiera odpowiednich funkcji. Fukccje muszą uruchamiać podfunkcje mm_*. Przykład:

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
//	switch (pdrv) {
//#ifdef DRV_CFC
//	case DRV_CFC :
//		return cf_disk_initialize();
//#endif
//#ifdef DRV_MMC
//	case DRV_MMC :
//		return mmc_disk_initialize();
//#endif
//	}

	if (pdrv) return STA_NOINIT;	/* Supports only single drive */


	return mmc_disk_initialize();

}





Sprawdz napięcia w module karty, powinny być 3.3V, sprawdz poprawność połączeń!

Wszyskie informacej o karcie SD i wszystkich funkcjach można znaleźć tutaj:
http://elm-chan.org/fsw/ff/00index_e.html - FAT_32 elm-chan






2.Obsługa dalmierza laserowego VL53L0X
Wszystkie pliki znajdują się w katalogu util
- i2cmaster.c
- i2cmaster.h
- millis.c
- millis.h
- VL53L0X.c
- VL53L0X.h

Tak samo jak wyżej, należy ustawić odpowiednie bity dla Timer 1, wyrzucić UART i wyświetlanie komunikatów
na komputerze. 

Wszystkie informacje o dalamierzu:
https://github.com/yetifrisstlama/vl53l0x-non-arduino
https://www.st.com/resource/en/datasheet/vl53l0x.pdf



3. Silniki krokowe
- Step_Motor.h
- Step_Motor.c
Sterowanie pełnokrokowe i półkrokowe. Timer 2



4. LCD - HD44780 gotowa biblioteka
- HD44780.c
- HD44780.h



W którymś momencie potrzebowałem dodać bibliotekę, szczerze nie pamiętam w którym momencie ale działa xD
http://mirekk36.blogspot.com/2012/09/eclipse-avr-gcc-doaczanie-bibliotek-a.html