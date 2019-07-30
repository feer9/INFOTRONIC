#include "../Drivers/ssd1306.h"

extern volatile uint32_t decimas_oled;

#define REPES_VAL 6

//   fonts info: https://github.com/olikraus/u8g2/wiki/fntlistall
// general info: https://github.com/olikraus/u8g2/wiki/u8g2reference

// ssd1306 driver state machine (ish)
void sm_ssd1306(u8g2_t *u8g2)
{
    static int st = 0, next = -2, repes = REPES_VAL;
    static bool keep_going = true;
    static bool inverted = false;


	if(keep_going && !decimas_oled) {
		st = next++;
	}

	// algunas pruebas para mostrar huevadas en la pantalla
	switch(st) {
	case 0: break;

	case -2:
		u8g2_SetFontMode(u8g2, 1);	// Transparent
		u8g2_SetFontDirection(u8g2, 0);
		u8g2_SetFont(u8g2, u8g2_font_logisoso46_tr);
		u8g2_ClearBuffer(u8g2);
		u8g2_DrawStr(u8g2, 0, 55, "HOLA!");
		u8g2_SendBuffer(u8g2);
		decimas_oled = 10;
		break;

	case -1:
		u8g2_ClearBuffer(u8g2);
		u8g2_DrawXBM(u8g2, 32,0, bitmap_linux_width, bitmap_linux_height, bitmap_linux);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 10;
		break;

	case 1:
		u8g2_ClearBuffer(u8g2);
	//	u8g2_ClearDisplay(u8g2);

		u8g2_SetFont(u8g2, u8g2_font_7x14B_tr);
		u8g2_DrawStr(u8g2, 0, 40, "eaa puto");
		u8g2_SendBuffer(u8g2);
		decimas_oled = 10;
		break;

	case 2:
		u8g2_SetFont(u8g2, u8g2_font_10x20_tr);
		u8g2_DrawStr(u8g2, 0, 60, "sabes q si?");
		u8g2_SendBuffer(u8g2);
		decimas_oled = 5;
		break;

	case 3:
		u8g2_SetFont(u8g2, u8g2_font_cupcakemetoyourleader_tr);
		u8g2_DrawStr(u8g2, 50, 20, "AaHhrrE!");
		u8g2_SendBuffer(u8g2);
		decimas_oled = 10;
		break;

	case 4:
		u8g2_ClearBuffer(u8g2);

		u8g2_DrawCircle(u8g2, 60, 32, 15, U8G2_DRAW_ALL);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 5;
		break;

	case 5:
		u8g2_DrawDisc(u8g2, 60, 32, 12, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 8;
		break;

	case 6:
		u8g2_DrawTriangle(u8g2, 0,0, 0,15, 15,0);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 5;
		break;

	case 7:
		u8g2_DrawTriangle(u8g2, 0,64, 10,64, 0,54);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 5;
		break;

	case 8:
		u8g2_DrawTriangle(u8g2, 128,0, 128,15, 113,0);
		u8g2_DrawTriangle(u8g2, 128,64, 128,54, 118,64);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 8;
		break;

	case 9:
		u8g2_SetFont(u8g2, u8g2_font_unifont_t_symbols);
		u8g2_DrawGlyph(u8g2, 8, 56, 0x2605);
		u8g2_DrawGlyph(u8g2, 14, 50, 0x2605);
		u8g2_DrawGlyph(u8g2, 5, 47, 0x2605);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 4;
		break;

	case 10:
		u8g2_DrawGlyph(u8g2, 100, 56, 0x2603);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 9;
		break;

	case 11:
		u8g2_DrawDisc(u8g2, 60, 32, 12, U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
		u8g2_SendBuffer(u8g2);
		// c: command; a: argument; 0x27:left scroll; 0:dummy; 0:from page 0; 3:speed; 7:to page 7; 0:255:dummy; 0x2f:start
		u8x8_SendF(&u8g2->u8x8, "caaaaaac", 0x27, 0, 0, 0b111, 7, 0, 255, 0x2f);
		decimas_oled = 12;
		break;

	case 12:
		// 0x26 right scroll, lo demas hay q volver a mandarlo si o si
		u8x8_SendF(&u8g2->u8x8, "caaaaaac", 0x26, 0, 0, 0b111, 7, 0, 255, 0x2f);
		decimas_oled = 24;
		break;

	case 13:
		// 0x26 right scroll, lo demas hay q volver a mandarlo si o si
		u8x8_SendF(&u8g2->u8x8, "caaaaaac", 0x27, 0, 0, 0b111, 7, 0, 255, 0x2f);
		decimas_oled = 12;
		break;

	case 14:
		// 0x2e:stop scroll
		u8x8_SendF(&u8g2->u8x8, "c", 0x2e);
		decimas_oled = 12;
		break;
	//	u8g2_SetFont(u8g2, u8g2_font_6x10_tf);
	//	u8g2_SetFontRefHeightAll(u8g2);
	//	u8g2_UserInterfaceMessage(u8g2, "Tijle1", "Twele2", "Dalle3", " Ok \n Cancel ");

	case 15:
		u8g2_ClearBuffer(u8g2);
		u8g2_DrawXBM(u8g2, 32,0, bitmap_mushroom_width, bitmap_mushroom_height, bitmap_mushroom);
		u8g2_SendBuffer(u8g2);
		decimas_oled = 50;
	//	keep_going = false;
		break;

	case 16:
		if((--repes)) {
			if(inverted)
				// todo: implementar algun tipo de funcion que controle la inversion
				// 0xa6: normal display
				u8x8_SendF(&u8g2->u8x8, "c", 0xa6);
			else
				// 0xa7: inverted display
				u8x8_SendF(&u8g2->u8x8, "c", 0xa7);
			inverted = !inverted;
			next--;
			decimas_oled = 3;
		}
		else {
			if(inverted) {
				u8x8_SendF(&u8g2->u8x8, "c", 0xa6);
				inverted = false;
			}
			repes = REPES_VAL;
			next = 1;
			decimas_oled = 10;
		}
		break;

	}
	st = 0;

    return;
}
