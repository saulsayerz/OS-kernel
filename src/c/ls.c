#include "header/filesystem.h"

int main(){
  char dipassing[512]; // yang akan dikirim ke sektor
  byte current_dir;
  char kanan[64];
  struct node_filesystem node_fs_buffer;
  struct map_filesystem  map_fs_buffer;
  int i = 0;
  int dir;
  char temp[2];

  clear(dipassing,512);
  clear(kanan,64);
  readSector(dipassing,0x105);
  current_dir = dipassing[0];
  memcpy(kanan,&dipassing[1],64);
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  if (strlen(kanan) > 0) {
    locateIndex(current_dir,kanan,0,&dir);
  }else{
    dir=current_dir;
  }

  if(dir == -1){
    puts("Directory not found! \n");
    exit();
  }
  for (i=0 ; i<64; i++){
    if (node_fs_buffer.nodes[i].parent_node_index == dir && strlen(node_fs_buffer.nodes[i].name)>0) {
      puts(node_fs_buffer.nodes[i].name);
      puts("\n");
    }
  }
  exit();
  while(true);
}