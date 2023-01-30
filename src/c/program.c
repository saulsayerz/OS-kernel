#include "header/program.h"
#include "header/filesystem.h"
#include "header/utils.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);


void exec(struct file_metadata *metadata, int segment) {
    interrupt(0x21, 0x6, metadata, segment, 0x0);
}

void exit() {
    int current_segment;
    struct file_metadata meta;
    char dipassing[512];
    char kiri[64];
    char kanan[256];
    char command[14];
    char param[50];
    clear(kiri,64);
    clear(kanan,256);
    clear(meta.node_name, 14);
    clear(dipassing,512);
    clear(command,14);
    clear(param,50);
    readSector(dipassing,0x105);
    locateNode(0xFF,"bin",0,&meta.parent_index);
    if(dipassing[65]=='\0'){
        strcpy(meta.node_name,"shell");
        exec(&meta, 0x2000);
    }else{
        //baca command yg belum tereksekusi
        parsesemicolon(&dipassing[65],kiri,kanan);
        //simpan command lanjutan kembali ke 0x105
        clear(&dipassing[65],256);
        memcpy(&dipassing[65],kanan,256);
        
        //eksekusi command dan simpan param
        parse(kiri,command,param);

        strcpy(meta.node_name,command);
        clear(&dipassing[1],64);
        memcpy(&dipassing[1],param,64);
        
        writeSector(dipassing,0x105);
        current_segment=getCurrentSegment();
        exec(&meta, current_segment+0x1000);

    }
    

    
}