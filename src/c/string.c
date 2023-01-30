#include "header/string.h"

unsigned int strlen(char *string)
// Mengembalikan panjang suatu null terminated string
{
    int i=0;
    char cc=string[i];
    while(cc!='\0'){
        i+=1;
        cc=string[i];
    }
    return i;
}
bool strcmp(char *s1, char *s2)
// Mengembalikan true jika string sama
{
    int panjang1 = strlen(s1);
    int panjang2 = strlen(s2);
    if(panjang1!=panjang2){
        return false;
    }else{
        bool sama=true;
        int i=0;
        while(sama && i<panjang1){
            if(s1[i]==s2[i]){
                i++;
            }else{
                sama=false;
            }
        }
        return sama;
    }
}

void strcpy(char *dst, char *src)
// Melakukan penyalinan null terminated string
{
    int i=0;
    char cc=src[0];
    while(cc!='\0'){
        dst[i]=src[i];
        i++;
        cc=src[i];
    }
}