#include "header/filesystem.h"

int main() {
  char temp[16];
  char string[64];
  char kiri[14];
  char kanan[64];
  char sisa[64];
  char dipassing[512]; // yang akan dikirim ke sektor
  struct file_metadata meta; //program yang akan dijalankan
  int idxbin;
  int idxprogram;
  int retcode;
  char path_str[128];
  char command1[64];
  char commandlanjutan[256];
  // byte current_directory;

  //cari dulu sudah ada message atau belum
  // strcpy(metadata.node_name,"message");
  // metadata.filesize     = 512;
  // metadata.parent_index = FS_NODE_P_IDX_ROOT;

  // read(&metadata,&retcode);
  // if(retcode==1){
  //   current_directory=FS_NODE_P_IDX_ROOT;
  //   metadata.buffer[0]=FS_NODE_P_IDX_ROOT;
  //   write(&metadata,&retcode);
  // }else{
  //   current_directory=metadata.buffer[0];
  // }


  clear(dipassing,512);
  readSector(dipassing,0x105); //currentdirectory
  clear(string,64);
  clear(meta.node_name,14);
  clear(kiri,64);
  clear(kanan,64);
  clear(sisa,64);
  puts("OS@IF2230:~");
  printCWD(path_str, dipassing[0]);
  puts("$");
  gets(string); //ls;cp   ; mv   
  parsesemicolon(string,command1,commandlanjutan);
  memcpy(&dipassing[65],commandlanjutan,256);
  parse(command1,kiri,kanan);

  if(kiri[0]=='.' && kiri[1]=='/'){
    strcpy(meta.node_name,&kiri[2]);
    meta.parent_index = dipassing[0];

    memcpy(&dipassing[1],kanan,64); //CEK
  }else{
    strcpy(meta.node_name,kiri);
    locateNode(0xFF, "bin", 0, &idxbin); //asumsi bin selalu ada di root
    meta.parent_index = idxbin;      // Pastikan parent_index adalah folder bin
    

    memcpy(&dipassing[1],kanan,64); //CEK
  }

  writeSector(dipassing,0x105); //currentdirectory

  // //setting message passing
  
  // memcpy(&metadata.buffer[0],&message.current_directory,1);
  // memcpy(&metadata.buffer[1],message.arg1,64);
  // memcpy(&metadata.buffer[65],message.arg2,64);
  // memcpy(&metadata.buffer[129],message.arg3,64);
  // memcpy(&metadata.buffer[193],&message.next_program_segment,4);
  // memcpy(&metadata.buffer[197],&message.other,317);
  // write(&metadata, &retcode); //KALO UDAH ADA GIMANA

  // if(retcode==3){
  //   read(&metadata,&retcode);
  // }
  puts("\n");
  exec(&meta, 0x3000);
  exit();
  while (true);
}
