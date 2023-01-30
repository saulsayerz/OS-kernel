#include "header/filesystem.h"

int main(){
  char dipassing[512]; // yang akan dikirim ke sektor
  byte current_dir;
  char kanan[64];
  struct node_filesystem node_fs_buffer;
  struct file_metadata newFile;
  char filename[50];
  char dest[50];
  char renamed[14];
  char destdir[50];
  int idxFile, idxFolder;
  enum fs_retcode return_code;
  int angka;

  clear(dipassing,512);
  clear(kanan,64);
  readSector(dipassing,0x105);
  current_dir = dipassing[0];
  memcpy(kanan,&dipassing[1],64);
  clear(filename,50);
  clear(dest,50);
  clear(renamed,14);
  clear(destdir,50);
  clear(newFile.node_name,14);
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  parse(kanan,filename,dest);
  locateIndex(current_dir,filename,1,&idxFile);
  if(idxFile==-1){
    locateIndex(current_dir,filename,0,&idxFile);
    if (idxFile == -1){
      puts(filename);
      puts(" does not exist !\n");
      exit();
    }
  }
  strcpy(newFile.node_name,node_fs_buffer.nodes[idxFile].name);
  locateIndex(current_dir,dest,0,&idxFolder);
  if(idxFolder==-1){ // kasus folder gak ada, direname dulu
    splitDirFile(dest,destdir,renamed);
    locateIndex(current_dir,destdir,0,&idxFolder);
    if (idxFolder == -1){ //direktorinya tetep gak ada, gak jadi rename
      puts("Destination directory not found! \n");
      exit();
    }
    else { // direktorinya ada
      clear(newFile.node_name,14);
      strcpy(newFile.node_name,renamed);
    }
  }
  //copy
  newFile.parent_index = node_fs_buffer.nodes[idxFile].parent_node_index;
  read(&newFile,&return_code);
  newFile.parent_index = idxFolder;
  write(&newFile,&return_code);
  if(return_code==0){
    puts("Copy success !\n");
  }else if(return_code==4 || return_code==5  || return_code==6 ){
    puts("Storage is full !");
  }else{
    puts("Error !");
  }
  exit();
}