#include "header/filesystem.h"

int main(){
  char dipassing[512]; // yang akan dikirim ke sektor
  byte current_dir;
  char kanan[64];
  enum fs_retcode * return_code;
  struct file_metadata * metadata;
  char buangan[14];
  char sisanya[64];
  int panjang;

  clear(dipassing,512);
  clear(kanan,64);
  readSector(dipassing,0x105);
  current_dir = dipassing[0];
  memcpy(kanan,&dipassing[1],64);
  metadata->parent_index=current_dir;
  metadata->filesize=0;

  if (strlen(kanan)<=0) {
    puts("Filename not given! \n");
    exit();
  }

  while (strlen(kanan) > 0) {
    parse(kanan, buangan, sisanya);
    clear(kanan,64);
    strcpy(kanan,sisanya);
    
    clear(metadata->node_name,14);
    strcpy(metadata->node_name, buangan);

    write(metadata,return_code);

    if (*return_code == FS_W_FILE_ALREADY_EXIST){
      puts("mkdir failed : File already exists.\n");
    }
    else if (*return_code == FS_W_MAXIMUM_NODE_ENTRY){
      puts("mkdir failed : Maximum node entry.\n");
    }
    else if (*return_code == FS_W_MAXIMUM_SECTOR_ENTRY){
      puts("mkdir failed : Maximum sector entry.\n");
    }
    else {
      puts("mkdir success.\n");
    }
  }
  exit();
}