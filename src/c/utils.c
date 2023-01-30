#include "header/utils.h"
#include "header/textio.h"
#include "header/string.h"

printCWD(char* path_str, byte current_dir){
  char arrayofstring[64][14];
  int j;
  int amountofwords = 0 ;
  byte currIndex = current_dir;
  struct node_filesystem   node_fs_buffer;
  int i = 0;

  for(j=0;j<64;j++){
    clear(arrayofstring[j],8);
  }
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  while (currIndex != 0xFF) {
    strcpy(arrayofstring[amountofwords], node_fs_buffer.nodes[currIndex].name);
    amountofwords ++ ;
    currIndex = node_fs_buffer.nodes[currIndex].parent_node_index;
  }
  
  for (i = 0; i < amountofwords; i++)
  {    
    puts("/");
    puts(arrayofstring[amountofwords - i - 1]);
  }
}

void locateNode(byte current_dir, char name[14], int file, int* idx){
  struct node_filesystem node_fs_buffer;
  struct map_filesystem  map_fs_buffer;
  int found=-1;
  int i=0;
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  while(found==-1 && i<64){
    if(node_fs_buffer.nodes[i].parent_node_index==current_dir && strcmp(node_fs_buffer.nodes[i].name,name)){
      if(file==0 && node_fs_buffer.nodes[i].sector_entry_index==0xFF){
        found=i;
      }else if(file==1 && node_fs_buffer.nodes[i].sector_entry_index!=0xFF){
        found=i;
      }else{
        i++;
      }
    }else{
      i++;
    }
  }
  *idx=found;
}

void locateIndex(byte current_dir, char path[64], int file, int *output){
  struct node_filesystem node_fs_buffer;
  char candidate[14];
  byte current_folder;
  int i,j,k;
  int*idx;
  bool found=false;
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  if(path[0]=='/'){
    current_folder=0xFF;
    i=1;
  }else if(path[0]=='.' && path[1]=='/'){
    current_folder=current_dir;
    i=2;
  }else{
    current_folder=current_dir;
    i=0;
  }
  k=0;
  clear(candidate,14);
  j=i;
  while(!found && j<64){
    if(path[j]=='/' || path[j]=='\0'){
      if(strlen(candidate)!=0){ 
        if(strcmp(candidate,"..")){
          if(current_folder!=0xFF){
            current_folder = node_fs_buffer.nodes[current_folder].parent_node_index;
          }
        }else{
          if(path[j]=='\0'){      
            locateNode(current_folder,candidate,file,idx);
          }else{
            locateNode(current_folder,candidate,0,idx);
          }
          if(*idx==-1){
            *output=-1;
            return;
          }else{
            current_folder=*idx;
          }
        }
      
        k=0;
        clear(candidate,14);
      }
      if(path[j]=='\0'){
        found=true;
      }
    }else{
      candidate[k]=path[j];
      k++;
    }
    j++;
  }
  *output=current_folder;
}