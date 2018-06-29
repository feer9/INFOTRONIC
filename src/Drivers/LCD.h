#define		LCD_BUFFER_SIZE	128

#define 	RENGLON_1		0
#define 	RENGLON_2		1
#define		LCD_CONTROL		1
#define		LCD_DATA		0

void displayLCD(char* msg, uint8_t row, uint8_t pos);

void		LCD_send(void);
uint8_t		pushLCD(uint8_t dato, uint8_t control);
int32_t		popLCD(void);
void		configLCD(void);
void		initLCD(void);
