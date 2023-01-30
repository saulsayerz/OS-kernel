#include "header/filesystem.h"

int main(){
  char dipassing[512]; // yang akan dikirim ke sektor
  byte current_dir;
  char kanan[64];
  struct node_filesystem node_fs_buffer;
  struct file_metadata metadata;
  enum fs_retcode return_code;
  int dir ;
  int i = 0 ;

  clear(dipassing,512);
  clear(kanan,64);
  readSector(dipassing,0x105);
  current_dir = dipassing[0];
  memcpy(kanan,&dipassing[1],64);
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  locateIndex(current_dir,kanan,1,&dir);

  if (dir == -1){
    puts("File not found. \n");
    exit();
  }

  clear(metadata.node_name,14);
  strcpy(metadata.node_name, node_fs_buffer.nodes[dir].name);
  metadata.parent_index = node_fs_buffer.nodes[dir].parent_node_index;
  read(&metadata, &return_code);

  if (return_code == FS_R_NODE_NOT_FOUND){
    puts("Node not found.\n");
  }
  else if (return_code == FS_R_TYPE_IS_FOLDER){
    puts("Type is folder.\n");
  }
  else{
      puts(metadata.buffer);
      puts("\n");
  }
  exit();
}