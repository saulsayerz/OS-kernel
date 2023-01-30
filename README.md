# Tugas Besar OS by hobi-bolOS


## Table of Contents
* [Introduction](#introduction)
* [Makefile](#makefile)
* [How to Run](#how-to-run)
* [Specification](#specification)
* [Testing](#testing)
* [Build](#build)
* [Utility Usage](#utility-usage)


## Introduction
Projek ini dibuat oleh hobi-bolOS yang beranggotakan sebagai berikut.
| No. | Name | Student ID |
| :---: | :---: | :---: |
| 1. | Saul Sayers | 13520094 |
| 2. | Nelsen Putra | 13520130 |
| 3. | Rizky Ramadhana | 13520151 |


> Milestone 1
## Makefile
Shell script yang tersedia pada file makefile yang bisa dijalankan di terminal:
| No. | Shell Script | Fungsi |
| :---: | :---: | :---: |
| 1. | all | untuk meng-compile semua program |
| 2. | diskimage | untuk meng-compile diskimage |
| 3. | bootloader | untuk meng-compile bootloader |
| 4. | kernel | untuk meng-compile kernel |
| 5. | stdlib | untuk meng-compile stdlib |
| 6. | run | untuk melakukan run program |
| 7. | build-run | untuk meng-compile sekaligus melakukan run program |


## How to Run
Jalankan perintah dibawah ini di terminal untuk ngecompile dan menjalankan semua program 
```shell 
make build-run
```

Namun, jika Anda ingin menjalankannya saja, cukup jalankan perintah berikut ini
```shell 
make run
```


## Specification
#### 3.1. Persiapan disk image
Pada tahap ini digunakan utility `dd` untuk membentuk image yang bernama `system.img`.

#### 3.2. Bootloader
Bootloader dibuat dengan menggunakan kit yang telah disediakan dan dicompile menggunakan `nasm`.

#### 3.3. Pembuatan kernel dan std_lib
Kernel dan std_lib dibuat menggunakan kit yang disediakan dan sesuai dengan spesifikasi yang diminta.
Fungsi-fungsi yang terdapat pada spesifikasi diimplementasi tanpa penambahan fitur.
Pada tahap ini juga dibuat `bash` script dan makefile yang dapat digunakan untuk membentuk kernel secara otomatis

#### 3.4. Menjalankan sistem operasi
Konfigurasi `if2230.config` dirubah pada penamaan disk imagenya dikarenakan perubahan nama
yang dilakukan pada tahap [pembuatan disk image](#31-persiapan-disk-image) sebelumnya.

#### 3.5. Implementasi interrupt 0x21
Untuk implementasi `printString` dan `readString` digunakan interrupt yang sesuai dengan yang diperlukan.
Bagian untuk operasi string sederhana ditulis pada `std_lib` dan bagian penulisan kelayar ditulis pada
`screen`. Implementasi `handleInterrupt21` mengikuti spesifikasi yang diberikan dan menambahkan beberapa
servis tambahan yang dapat digunakan library nantinya. Pada tahap ini juga diimplentasikan operasi sederhana
mod yang dapat digunakan sistem operasi.


> Milestone 2
## Testing
Sistem Operasi harus terlebih dahulu dibuat menggunakan `make build` dan harus di *run* terlebih dahulu. Setelah di-*run*, *close*, lalu masukkan *test*.
```
make build-run
```
Masukkan `test_case A`. Untuk menggunakan *test case* lain, ganti huruf A dengan huruf lain. Contoh:
```
cd out
./tc_gen A
cd ..
make run
```

## Build
### All File
For build only:
```bash
make all
```
For run only:
```bash
make run
```
For both:
```bash
make build-run
```

### diskimage
```bash
make diskimage
```
diskimage akan ada di `out/system.img`

### bootloader
```bash
make bootloader
```

### kernel
```bash
make kernel
```

## Utility Usage
Misal current working directory saat ini adalah ``` root ```. Sebagai perhatian, kami juga mengimplementasi relative dan absolute pathing. Sehingga command 
```
cd /folder1/folder2
```
ataupun
```
cd ../folder3
```
akan bekerja dengan catatan folder tersebut ada.
### mkdir
```bash
[hobibolOS]:root/$ mkdir
<input-nama-folder-baru>
```
### cd
Pergi ke suatu folder yang terdapat pada current working directory
```bash
[hobibolOS]:root/$ cd
<input-nama-folder-tujuan>
```
Pergi ke folder parent
```bash
[hobibolOS]:root/$ cd
..
```
Pergi ke folder root
```bash
[hobibolOS]:root/$ cd
/
```

### cat
```bash
[hobibolOS]:root/$ cat
<input-nama-file-yang-ingin-dibaca>
```

### ls
```bash
[hobibolOS]:root/$ ls
```

### cp
```bash
[hobibolOS]:root/$ cp
<input-nama-file-sumber>
<input-nama-folder-tujuan>
```
> Milestone 3

Cara melakukan build sama seperti folder 2. 

Implementasi yang dikerjakan pada milestone ini adalah : 
1. **Library dan executeProgram**
Memisahkan beberapa library yang terdapat pada kernel.c dan std_lib menjadi library yang terpisah dari kernel untuk diakses oleh user space. Library tersebut dipanggil user dengan interrupt 21. Berikut adalah beberapa library yang diimplementasikan : 

- Library string : Berisi library yang berkaitan dengan string, yakni strlen, strcmp, dan strcpy.
- Library textio : Berisi library yang berisi puts, gets, dan clearscreen
- Library fileio : Berisi library yang berkaitan dengan pembacaan dan penulisan data pada sektor seperti read,write,readsector,writesector
- Library program : Berisi library yang berkaitan dengan eksekusi library dan keluarnya, yakni exec dan exit

Pada library program, terdapat executeProgram yang dapat digunakan untuk mengeksekusi utility atau fungsi.

2. **Aplikasi shell dan *utility* Program**
- Shell : berhasil diimplementasikan sebuah shell yang terpisah dengan kernel dan dapat dieksekusi dengan executeProgram. Dalam shell sendiri dapat menerima input command untuk mengeksekusi utility lain, dan dapat melakukannya secara global ataupun local. 

- Utility sesuai spesifikasi: Diimplementasikan beberapa utility yang sesuai spesifikasi, yakni :
    - cd : seperti pada milestone 2.
    - ls : seperti pada milestone 2.
    - mkdir : seperti pada milestone 2.
    - cat : seperti pada milestone 2.
    - mv : seperti pada milestone 2.
    - cp : Terdapat sedikit revisi. Pada milestone 2, cp melakukan pencopyan file dari suatu direktori ke direktori lain dengan nama yang sama (seperti perilaku copy-paste). Ditambahkan fitur untuk melakukan copy sedirektori dengan nama file yang berbeda

3. **Message Passing** <br>
Diperlukan sebuah metode untuk mengirim pesan atau data dari suatu program ke program lain. Metode tersebut adalah message passing. Untuk message passing sendiri, kami menggunakan sektor 0x105 untuk menyimpan datanya. Pada sisi yang mengirim data, akan mengirim write sector ke 0x105. Pada sisi yang menerima, akan menggunakan readSector. Sudah dipastikan untuk mengisi 0x105 pada fungsi fillMap()

4. **Multiple Program** <br>
Diterapkan multiple program yang dapat menerima argumen ataupun tidak. Multiple program diimpelementasikan message passing.  Kami memodifikasi fungsi exit untuk melakukan multiple program, dengan memeriksa apakah semua segmen sudah tereksekusi atau belum. Apabila belum, maka lanjut ke segmen selanjutnya dengan increment segmen. Apabila sudah, maka kembali shell. 