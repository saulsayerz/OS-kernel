#include "header/std_lib.h"

// Opsional : Tambahkan implementasi kode C

// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b)
{
    return a / b;
}

int mod(int a, int n)
{
    return a - (a/n)*n;
}


// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n)
// Mengcopy n bytes yang ditunjuk src ke dest
{
    int i;
    for(i=0;i<n;i++){
        dest[i]=src[i];
    }
}

void clear(byte *ptr, unsigned int n)
// Mengosongkan byte array yang memiliki panjang n
{
    int i;
    for(i=0;i<n;i++){
        ptr[i]='\0';
    }
}

void parse(char*string, char* kiri, char* kanan){
    int i = 0;
    int j = 0;
    char cc=string[i];
    clear(kiri,14);
    clear(kanan,64);

    while(cc!='\0'&& cc!=' ' && cc!='\r'){
        kiri[i] = string[i];
        i ++ ;
        cc=string[i];
    }
    
    // kasus ada argumen di kanannya, tambahin.
    // kalau ndak ada ya kanan biarin kosong
    if (cc == ' '){
        while (cc == ' ') {
            i++ ;
            cc = string[i];
        }
        while(cc!='\0' && cc!='\r'){
            kanan[j] = string[i];
            i ++ ;
            j++;
            cc=string[i];
        }
    }
}

void splitDirFile(char*string, char* kiri, char* kanan){
    int i = strlen(string)-1;
    int panjang = strlen(string);
    bool adaslash = false;

    clear(kanan,14);
    clear(kiri,36);

    while (!adaslash && i>=0){
        if (string[i] == '/'){
            adaslash = true;
        }
        else{
            i--;
        }
    }

    strcpy(kanan, &string[i+1]);
    strcpy(kiri,string);
    kiri[i] = '\0';
    clear(&kiri[i], panjang - i);
}

void parsesemicolon(char*string, char* kiri, char* kanan){
    int i = 0;
    int j = 0;
    char cc=string[i];
    clear(kiri,14);
    clear(kanan,64);

    while(cc!='\0'&& cc!=';' && cc!='\r'){
        kiri[i] = string[i];
        i ++ ;
        cc=string[i];
    }
    if(kiri[i-1]==' '){
        kiri[i-1]='\0';        
    }

    
    // kasus ada argumen di kanannya, tambahin.
    // kalau ndak ada ya kanan biarin kosong
    if (cc == ';'){
        do{
            i++ ;
            cc = string[i];
        }while(cc==' ');
        while(cc!='\0' && cc!='\r'){
            kanan[j] = string[i];
            i ++ ;
            j++;
            cc=string[i];
        }
    }
}


