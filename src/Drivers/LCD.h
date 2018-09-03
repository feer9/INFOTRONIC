#define		LCD_BUFFER_SIZE	128

#define 	RENGLON_1		0
#define 	RENGLON_2		1
#define		LCD_CONTROL		1
#define		LCD_DATA		0

void		LCD(char* msg, uint8_t row, uint8_t pos);
void		LCD_clear(void);
void		LCD_printCentered(char* msg, uint8_t row);
void		LCD_printDOWN(char* msg);
void		LCD_printUP(char* msg);
void		LCD_print(char* msg);
void		LCD_line(const char* src, char* dest, \
					 uint8_t start, uint8_t len);

void		LCD_send(void);
uint8_t		pushLCD(uint8_t dato, uint8_t control);
int32_t		popLCD(void);
void		configLCD(void);
void		initLCD(void);
