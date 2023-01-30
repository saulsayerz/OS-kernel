#include "header/textio.h"

int puts(char *string) {
  interrupt(0x21, 0, string, 0, 0);
}

int gets(char *string) {
  interrupt(0x21, 1, string, 0, 0);
}

int clearscreen(){
  interrupt(0x21, 7, 0, 0, 0);
}
