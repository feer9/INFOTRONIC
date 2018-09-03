#include <Aplicacion.h>
/*
typedef struct _RTC
{
	uint8_t displayStatus;
	char hour[3];
} RTC;
*/
void displayClock()
{
	char line[17] = "Hour: ";
	char aux[5];

	intToStr((int)LPC_RTC->HOUR, aux, 2);
	strcat(line, aux);
	strcat(line, ":");
	intToStr((int)LPC_RTC->MIN, aux, 2);
	strcat(line, aux);
	strcat(line, ":");
	intToStr((int)LPC_RTC->SEC, aux, 2);
	strcat(line, aux);
	LCD_printCentered(line, RENGLON_1);

	strcpy(line, "Date: ");
	intToStr((int)LPC_RTC->YEAR, aux, 4);
	strcat(line, aux);
	strcat(line, "/");
	intToStr((int)LPC_RTC->MONTH, aux, 2);
	strcat(line, aux);
	strcat(line, "/");
	intToStr((int)LPC_RTC->DOM, aux, 2);
	strcat(line, aux);
	LCD_printCentered(line, RENGLON_2);
}

//char itoc(int i)

