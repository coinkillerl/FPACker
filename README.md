# FPACker
Tool to pack and unpack FPAC (.pac) files used in Trails in the Sky 1st Chapter / Sora no Kiseki the 1st
## Usage
Run ./FPACker for help
## Features
- Unpack all files
- Pack directory (coming soon!)

## Supported Games
- Trails in the Sky 1st Chapter / Sora no Kiseki the 1st

## How to Build
Clone this repository, then install gcc, make and cmake,  then run the following commands

```bash
mkdir build_release
cd build_release
cmake ..
cmake —build . —config release
```

If you are on Windows, you must use Msys2.

## The FPAC file format
This is a quite simple archive format. No compression is used.
#### Header
```c
char fpac_magic[4]  //File signature, value is "FPAC"
u32 n_of_files //Number of files in the archive
u32 first_file_address //Address of the 1st file's 1st data section byte
u32 unk_magic //Unknown u32, seems to always be set to 1
```
#### File Entry
```c
u32 filename_crc32 //crc32 hash of the filename string (excluding null terminator) XOR'd with 0xFFFFFFFF (thanks to hell259 for figuring this out)
u32 padding //Literally nothing
u64 filename_string_address //Address of the first char of the filename string.
u64 file_size //Size of the file in bytes
u64 file_data_address //Address of the first byte of the file's data section
```
#### File
```c
char* filename //NULL-terminated string that represents the full path of the file.
u8 data[file_size] //Uncompressed data of the file
```
