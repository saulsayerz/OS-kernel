#include "std_type.h"


#ifndef STD_TYPE_H
#define STD_TYPE_H

// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b);

int mod(int a, int n);


// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n);
// Mengcopy n bytes yang ditunjuk src ke dest

void clear(byte *ptr, unsigned int n);
// Mengosongkan byte array yang memiliki panjang n

void parse(char*string, char* kiri, char* kanan);
//memisahkan kata pertama dan kata setelah spasi pertama

void splitDirFile(char*string, char* kiri, char* kanan);
//memisahkan kata terakhir setelah garing terakhir dan sisanya

#endif
