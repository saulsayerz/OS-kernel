#include "header/filesystem.h"

int main(){
  char dipassing[512]; // yang akan dikirim ke sektor
  byte current_dir;
  char kanan[64];
  struct node_filesystem node_fs_buffer;
  int i = 0;
  byte c = 0;
  int j,k,output;

  clear(dipassing,512);
  clear(kanan,64);
  readSector(dipassing,0x105);
  current_dir = dipassing[0];
  memcpy(kanan,&dipassing[1],64);
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  if(kanan[0]=='\0'){
    puts("Parameter is not enough !\n");
  }else{
    locateIndex(current_dir,kanan,0,&output);
    if(output==-1){
      puts("Directory is not valid !\n");
    }else{
      current_dir=output;
    }
  } 
  dipassing[0] = current_dir;
  writeSector(dipassing,0x105);
  exit();
}