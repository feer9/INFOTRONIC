#ifndef DRIVERS_VARIOS_H_
#define DRIVERS_VARIOS_H_

#include "regsLPC1769.h"

int32_t ipow(int32_t base, uint8_t exp);

char* extract_substr(char const *s, char *sub, size_t from, size_t to);
char* reverse(char *str, int len);
char* intToStr(int x, char str[], int d);

#endif /* DRIVERS_VARIOS_H_ */
