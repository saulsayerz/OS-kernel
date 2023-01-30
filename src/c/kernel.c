#include "header/kernel.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

int main() {
  char dipassing[512];
  struct file_metadata meta;
  fillMap();
  clearScreen();
  makeInterrupt21();
  printString("Welcome to hobi-bolOS's kernel.\nInitializing shell...\n\n");
  //shell();
  //WRITE SECTOR CURDIR DULU BOS


  //start shell
  clear(dipassing,512);
  dipassing[0] = 0xFF;
  writeSector(dipassing,0x105);
  clear(meta.node_name,14);
  strcpy(meta.node_name,"shell");
  meta.parent_index = 0;
  executeProgram(&meta, 0x2000);

  while (true);
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        case 0x2:
            readSector(BX, CX);
            break;
        case 0x3:
            writeSector(BX, CX);
            break;
        case 0x4:
            read(BX, CX);
            break;
        case 0x5:
            write(BX, CX);
            break;
        case 0x6:
            executeProgram(BX,CX);
            break;
        case 0x7:
            clearScreen();
            break;
        default:
            printString("Invalid Interrupt");
    }
}


void printString(char *string)
{
    int i=0;
    while(string[i]!='\0'){ //melakukan iterasi sampai ujung string
        interrupt(0x10,(0xe << 8)+ string[i],0,0,0);
        if(string[i]=='\n'){
          interrupt(0x10,(0xe << 8)+ '\r',0,0,0); //meniru perilaku printf, bila ada \n maka berganti baris dan kursor berpindah ke ujung kiri
        }
        i++;
    }
}
void readString(char *string)
{
  int i=0;
  char cc;
  char blank = ' ';
  int CX,DX,AX= (0x3 << 8);
  do{
    cc = interrupt(0x16,0,0,0,0);
    if(cc!='\r'){ //selama bukan enter, terima input char dan proses
      if (cc != 0x8) { // Apabila bukan backspace, tampilkan ke layar dan simpan ke string
        string[i]=cc;
        interrupt(0x10,(0xe << 8)+cc,0,0,0); //cetak ke layar
        i++;
      }
      else if (cc == 0x8 && i > 0) { // kasus handle backspace
        DX = cursorCoordinates();
        if (((DX << 8)>>8) == 0) { // Kasus dia paling kiri ya naikin baru hapus
          interrupt(0x10,(0x2 << 8),0,0, ((DX>>8 +1)<<8) + 79);
          interrupt(0x10,(0xe << 8)+blank,0,0,0); // cetak blank character
          interrupt(0x10,(0x2 << 8),0,0, ((DX>>8 +1)<<8) + 79);
          i--;
        }
        else{
          interrupt(0x10,(0xe << 8)+cc,0,0,0); // lakukan backspace sekali
          interrupt(0x10,(0xe << 8)+blank,0,0,0); // cetak blank character
          interrupt(0x10,(0xe << 8)+cc,0,0,0); // lakukan backspace lagi
          i--;
        }
      }
      else{
        // UNTUK KASUS i = 0 TAPI INPUT BACKSPACE TIDAK PERLU DIHANDLE, DO NOTHING
      }
    }
  }while( cc !='\r');
  string[i]='\0';
  printString("\n");
}

void clearScreen()
{
  // Dari spesifikasi clearScreen (Dengan ukuran 80 x 25):
  // Dengan interrupt 0x10 untuk mengubah video mode 3, menghapus layar, memindahkan kursor, mengganti buffer warna menjadi putih 
  int int_mode = 0x10;
  int AX, BX, CX, DX;

  // 1. Mengubah video mode menjadi 3
  AX = 3; // Menggunakan AH = 0 untuk set video mode dan AL = 3 untuk modenya
  BX = 0;
  CX = 0;
  DX = 0;
  interrupt(int_mode, AX, BX, CX, DX);

  // 2. Menghapus layar dengan scroll up window agar bersih dan mengubah warna buffer
  AX = (6 << 8); // Menggunakan AH = 6 untuk scroll up dan AL = 0 agar clear
  BX = (0xF << 8); // Menggunakan BH = 0xF agar warna background putih 
  CX = 0; // Koordinat dari pojok kiri atas, dengan CH = y = 0 dan CL = x = 0
  DX = (24<<8) + 79 ; // Koordinat dari pojok kanan bawah, dengan CH = y = 24 dan CL = x = 79
  interrupt(int_mode, AX, BX, CX, DX);

  // 3. Set cursor ke pojok atas kiri
  AX = (2 << 8); // Menggunakan AH = 2 untuk set posisi cursor
  BX = 0; // Menggunakan BH = 0 untuk page-number nya
  CX = 0;
  DX = 0; // Menggunakan DH = row = 0 dan DL = 0 = col
  interrupt(int_mode, AX, BX, CX, DX);
}

void readSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0200 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}

void writeSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0300 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
  struct node_filesystem   node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;
  bool found = false;
  int i =0;
  byte partition[16];
  int idx;
  // Tambahkan tipe data yang dibutuhkan

  // Masukkan filesystem dari storage ke memori buffer
  readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.;
  //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
  //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
  //    dan keluar.  
  while(!found && i<64){
    if(strcmp(node_fs_buffer.nodes[i].name,metadata->node_name) && metadata->parent_index==node_fs_buffer.nodes[i].parent_node_index){
      found=true;
    }else{
      i++;
    }
  }

  if(!found){
    *return_code=FS_R_NODE_NOT_FOUND;
    return;
  }


  // 2. Cek tipe node yang ditemukan
  //    Jika tipe node adalah file, lakukan proses pembacaan.
  //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
  //    dan keluar.
  if(node_fs_buffer.nodes[i].sector_entry_index==0xFF){
    *return_code=FS_R_TYPE_IS_FOLDER;
    return;
  }


  // Pembacaan
  // 1. memcpy() entry sector sesuai dengan byte S
  // 2. Lakukan iterasi proses berikut, i = 0..15
  // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
  // 4. Jika byte bernilai 0, selesaikan iterasi
  // 5. Jika byte valid, lakukan readSector() 
  //    dan masukkan kedalam buffer yang disediakan pada metadata
  // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
  // 7. Tulis retcode FS_SUCCESS dan ganti filesize 
        //pada akhir proses pembacaan.
  idx = node_fs_buffer.nodes[i].sector_entry_index;
  memcpy(partition,sector_fs_buffer.sector_list[idx].sector_numbers,16);
  i=0;
  found=false;
  while(i<16 && !found){
    if(partition[i]==0){
      found=true;
    }else{
      readSector(metadata->buffer + (512*i), partition[i]);
      i++;
    }
  }
  metadata->filesize = i*512;
  *return_code=FS_SUCCESS;
}


void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
  struct node_filesystem   node_fs_buffer;
  struct sector_filesystem sector_fs_buffer;
  struct map_filesystem    map_fs_buffer;
  // Tambahkan tipe data yang dibutuhkan
  int map = 0;
  int sektor = 0;
  bool kelar = false;
  int tertulis = 0;
  bool found = false;
  int i =0;
  int j = 0;
  int k=0;
  int empty_space=0;

  // Masukkan filesystem dari storage ke memori
  readSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);
  readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
  //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
  //    Jika ditemukan node yang cocok, tuliskan retcode 
  //    FS_W_FILE_ALREADY_EXIST dan keluar. 
  while(!found && i<64){
    if(strcmp(node_fs_buffer.nodes[i].name,metadata->node_name) && metadata->parent_index==node_fs_buffer.nodes[i].parent_node_index){
      found=true;
    }else{
      i++;
    }
  }

  if(found){
    *return_code=FS_W_FILE_ALREADY_EXIST;
    return;
  }
  // 2. Cari entri kosong pada filesystem node dan simpan indeks.
  //    Jika ada entry kosong, simpan indeks untuk penulisan.
  //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
  //    dan keluar.
  found=false;
  i=0; //i menyimpan node kosong
  while(!found && i<64){
    if(strlen(node_fs_buffer.nodes[i].name)==0){
      found=true;
    }else{
      i++;
    }
  }
  if(!found){
    *return_code=FS_W_MAXIMUM_NODE_ENTRY;
    return;
  }

  // 3. Cek dan pastikan entry node pada indeks P adalah folder.
  //    Jika pada indeks tersebut adalah file atau entri kosong,
  //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
  if(metadata-> parent_index != 0xFF){
      if (node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != 0xFF){
        *return_code = FS_W_INVALID_FOLDER;
        return;
      }
  }


  // 4. Dengan informasi metadata filesize, hitung sektor-sektor 
  //    yang masih kosong pada filesystem map. Setiap byte map mewakili 
  //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
  //    Jika empty space tidak memenuhi, tuliskan retcode
  //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
  //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
  //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
  //    Jika tersedia empty space, lanjutkan langkah ke-5.
  for(j=0;j<512;j++){
    if(!map_fs_buffer.is_filled[j]){
      empty_space++;
    }
  }

  if(empty_space*512<metadata->filesize || metadata->filesize>8192){
    *return_code=FS_W_NOT_ENOUGH_STORAGE;
    return;
  }

  // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
  //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk 
  //    penulisan.
  //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
  //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
  //    Selain kondisi diatas, lanjutkan ke proses penulisan.


  found=false;
  while(k<32 && !found){
    if(sector_fs_buffer.sector_list[k].sector_numbers[0]==0){
      found=true;
    }else{
      k++;
    }
  }

  if(!found){
    *return_code=FS_W_MAXIMUM_SECTOR_ENTRY;
    return;
  }

  // Penulisan
  // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
  node_fs_buffer.nodes[i].parent_node_index = metadata->parent_index;
  strcpy(node_fs_buffer.nodes[i].name, metadata->node_name);
  // 2. Jika menulis folder, tuliskan byte S dengan nilai 
  //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8

  // asumsi folder filesize 0, file lebih dari 0
  if (metadata->filesize == 0 ) { // kasus folder, langsung skip ke langkah 8 aja
    node_fs_buffer.nodes[i].sector_entry_index = FS_NODE_S_IDX_FOLDER; // 
  }
  // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
  else{
    node_fs_buffer.nodes[i].sector_entry_index = k ;
  // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
  // 5. Persiapkan variabel buffer untuk entry sector kosong
  // 6. Lakukan iterasi berikut dengan kondisi perulangan 
        //(penulisan belum selesai && i = 0..255)
  //    1. Cek apakah map[i] telah terisi atau tidak
  //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
  //    3. Tandai map[i] terisi
  //    4. Ubah byte ke-j buffer entri sector dengan i
  //    5. Tambah nilai j dengan 1
  //    6. Lakukan writeSector() dengan file pointer buffer pada metadata 
  //       dan sektor tujuan i
  //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
  //       filesize pada metadata, penulisan selesai
      while (!kelar && map < 256){ 
        if (map_fs_buffer.is_filled[map] == false){
          map_fs_buffer.is_filled[map] = true;
          sector_fs_buffer.sector_list[k].sector_numbers[sektor]=map;
          writeSector(metadata->buffer + (sektor*512), map);
          sektor++;
          tertulis += 512;
          if (tertulis > metadata->filesize){
            kelar = true;
          }
        }
        map ++ ;
      }
    }
  // 7. Lakukan update dengan memcpy() buffer entri sector dengan 
  //    buffer filesystem sector
  // step ini diskip karena penulisan langsung di filesistem, tidak pada buffer

  // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
  //    menggunakan writeSector() pada sektor yang sesuai
  writeSector(map_fs_buffer.is_filled, 0x100);
  writeSector(&(node_fs_buffer.nodes[0]), 0x101);
  writeSector(&(node_fs_buffer.nodes[32]), 0x102);
  writeSector(sector_fs_buffer.sector_list, 0x103);
  // 9. Kembalikan retcode FS_SUCCESS
  
  *return_code = FS_SUCCESS;
}

// void shell() {
//   char input_buf[64];
//   char path_str[128];
//   char kiri[14];
//   char kanan[64];
//   byte current_dir = FS_NODE_P_IDX_ROOT;

//   while (true) {
//     printString("OS@IF2230:~");
//     printCWD(path_str, current_dir);
//     printString("$");
//     readString(input_buf);
//     parse(&input_buf, kiri, kanan);
   
//     if (strcmp(kiri, "cd")){
//       cd(&current_dir,kanan);
//     }
//     else if (strcmp(kiri, "ls")){
//       ls(current_dir, kanan);
//     }
//     else if (strcmp(kiri, "mv")){
//       mv(current_dir, kanan);
//     }
//     else if (strcmp(kiri, "mkdir")){
//       mkdir(current_dir,kanan);
//     }
//     else if (strcmp(kiri, "cat")){
//       cat(current_dir, kanan);
//     }
//     else if (strcmp(kiri, "cp")){
//       cp(current_dir,kanan);
//     }
//     else if (strcmp(kiri, "cls")){
//       clearScreen();
//     }else if (strcmp(input_buf, "test")) {
//       struct file_metadata meta;
//       strcpy(meta.node_name,"shell");
//       meta.parent_index = 0;
//       executeProgram(&meta, 0x2000);
//     }else 
//       printString("Unknown command\r\n");
//   }
// }


void fillMap() {
  struct map_filesystem map_fs_buffer;
  int i;

  // Load filesystem map
  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

  /* 
    Edit filesystem map disini 
                             */
  for(i=0;i<=31;i++){
    map_fs_buffer.is_filled[i]=true;
  }

  map_fs_buffer.is_filled[105]=true;

  for(i=256;i<=511;i++){
    map_fs_buffer.is_filled[i]=true;
  }

  // Update filesystem map
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}

void executeProgram(struct file_metadata *metadata, int segment) {
  enum fs_retcode fs_ret;
  byte buf[8192];
  
  memcpy(metadata->buffer,buf,8192);
  read(metadata, &fs_ret);
  if (fs_ret == FS_SUCCESS) {
    int i = 0;
    for (i = 0; i < 8192; i++) {
      if (i < metadata->filesize)
        putInMemory(segment, i, metadata->buffer[i]);
      else
        putInMemory(segment, i, 0x00);
    }
    launchProgram(segment);
  }else{
    printString("exec: file not found\r\n");
  }
}

