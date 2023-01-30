#include "header/filesystem.h"

int main(){
  char dipassing[512]; // yang akan dikirim ke sektor
  byte current_dir;
  char kanan[64];
  struct node_filesystem node_fs_buffer;
  char filename[50];
  char dest[50];
  char renamed[14];
  char destdir[50];
  char temp[14];
  int idxFile, idxFolder;
  struct file_metadata newFile;
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
  clear(temp,14);
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

  locateIndex(current_dir,dest,0,&idxFolder);
  if(idxFolder==-1){ // kasus folder gak ada, direname dulu
    splitDirFile(dest,destdir,renamed);
    locateIndex(current_dir,destdir,0,&idxFolder);
    if (idxFolder == -1){ //direktorinya tetep gak ada, gak jadi rename
      puts("Destination directory not found! \n");
      exit();
    }
    else { // direktorinya ada
      clear(node_fs_buffer.nodes[idxFile].name,14);
      strcpy(temp, node_fs_buffer.nodes[idxFile].name);
      strcpy(node_fs_buffer.nodes[idxFile].name, renamed);
    }
  }
  //pindahin
  if(node_fs_buffer.nodes[idxFile].sector_entry_index==0xFF){
    locateNode(idxFolder, node_fs_buffer.nodes[idxFile].name,0,&angka );
  }else{
    locateNode(idxFolder, node_fs_buffer.nodes[idxFile].name,1,&angka );
  }
  
  if(angka != -1){ 
    clear(node_fs_buffer.nodes[idxFile].name,14);
    strcpy(node_fs_buffer.nodes[idxFile].name, temp);
    puts("Name is already in that directory ! \n");
    exit();
  }
  else{
    node_fs_buffer.nodes[idxFile].parent_node_index = idxFolder;
  }

  writeSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  exit();
}