#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Fat12BootEntry.h"

static int cur_byte = 0;

//  Fat12Boot boot;
// Fat12Entry *entry;
// //dsd vData;




// void loadBootTEST(unsigned int *img){
//     printf("IMG address: %p\n", img);
//     printf("HERE\n");
//    // for(int i = 0; i<512; i++){
//     printf("FILETHINGS---- %d\n", img[4]);
//     lseek(img, 11, SEEK_SET); //skip 11 bytes
//     //lseek();
//     //BYTES_PER_SECTOR (2 bytes)
//     //fread(&boot.BYTES_PER_SECTOR, 2, 1, img);
//     read(img,&boot.BYTES_PER_SECTOR,2);
//     printf("BYTES_PER_SECTOR: %d\n", boot.BYTES_PER_SECTOR);

//     //SECTORS_PER_CLUSTER (2 bytes)
//     //fread(&boot.SECTORS_PER_CLUSTER, 1, 1, img);
//     read(img,&boot.SECTORS_PER_CLUSTER,1);
//     printf("SECTORS_PER_CLUSTER: %d\n", boot.SECTORS_PER_CLUSTER);
//     //RESERVED_SECTORS (2 bytes)
//     //fread(&boot.RESERVED_SECTORS, 2, 1, img);
//     read(img,&boot.RESERVED_SECTORS,2);
//     printf("RESERVED_SECTORS: %d\n", boot.RESERVED_SECTORS);
//     //NUM_OF_FATS (1 byte)
//     //fread(&boot.NUM_OF_FATS, 1, 1, img);
//     read(img,&boot.NUM_OF_FATS,1);
//     printf("NUM_OF_FATS: %d\n", boot.NUM_OF_FATS);
//     //MAX_ROOT_DIRS (2 bytes)
//     //fread(&boot.MAX_ROOT_DIRS, 2, 1, img);
//     read(img,&boot.MAX_ROOT_DIRS,2);
//     printf("MAX_ROOT_DIRS: %d\n", boot.MAX_ROOT_DIRS);
//     //Initialize 'entry'
//     //TOTAL_SECTORS (2 bytes)
//     //fread(&boot.TOTAL_SECTORS, 2, 1, img);
//     read(img,&boot.TOTAL_SECTORS,2);
//     lseek(img, 1, SEEK_CUR); //skip 1 byte

//     //SECTORS_PER_FAT (2 bytes)
//     //fread(&boot.SECTORS_PER_FAT, 2, 1, img);
//     read(img,&boot.SECTORS_PER_FAT,2);
//     //SECTORS_PER_TRACK (2 bytes)
//     //fread(&boot.SECTORS_PER_TRACK, 2, 1, img);
//     read(img,&boot.SECTORS_PER_TRACK,2);
//     //NUM_OF_HEADS (2 bytes)
//     //fread(&boot.NUM_OF_HEADS, 2, 1, img);
//     read(img,&boot.NUM_OF_HEADS,2);
//     lseek(img, 11, SEEK_CUR); //skip 11 bytes

//     //VOUME_ID (4 bytes)
//    // fread(&boot.VOLUME_ID, 4, 1, img);
//     read(img,&boot.VOLUME_ID,4);
//     //VOLUME_LABEL
//     //fread(&boot.VOLUME_LABEL, 11, 1, img);
//     read(img,&boot.VOLUME_LABEL,11);
//     //printf("RET: %d: %s\n", ret, strerror(errno));
//     //printf("IMG: %d\n", img);

//     //Move to beginning of ROOT DIRECTORY
//     lseek(img, ((boot.NUM_OF_FATS * boot.SECTORS_PER_FAT) + 1) * boot.BYTES_PER_SECTOR, SEEK_SET);
//    // }
// }


// void loadEntry(int start, int img) {
//     /* Set reader to 0 */
//     char * bytes;
//     sprintf( bytes, "%d", img );
//     byte_seek(0, SEEK_SET_N);

//     /* Load data into entry */
//     for (int i = start; i < (start + 16); i++) {
//         /* FILENAME (8 bytes) */
//         byte_read(&entry[i].FILENAME, 8, bytes);

//         /* EXT (3 bytes) */
//         byte_read(&entry[i].EXT, 3, bytes);

//         /* ATTRIBUTES (1 byte) */
//         byte_read(&entry[i].ATTRIBUTES, 1, bytes);

//         /* RESERVED (10 bytes) */
//         byte_read(&entry[i].RESERVED, 2, bytes);

//         /* CREATION_TIME (2 bytes) */
//         byte_read(&entry[i].CREATION_TIME, 2, bytes);

//         /* CREATION_DATE (2 bytes) */
//         byte_read(&entry[i].CREATION_DATE, 2, bytes);

//         /* LAST_ACCESS_DATE (2 bytes) */
//         byte_read(&entry[i].LAST_ACCESS_DATE, 2, bytes);

//         byte_seek(2, SEEK_CUR_N); //skip 2 bytes

//         /* MODIFY_TIME (2 bytes) */
//         byte_read(&entry[i].MODIFY_TIME, 2, bytes);

//         /* MODIFY_DATE (2 bytes) */
//         byte_read(&entry[i].MODIFY_DATE, 2, bytes);

//         /* START_CLUSTER (2 bytes) */
//         byte_read(&entry[i].START_CLUSTER, 2, bytes);

//         /* FILE_SIZE (4 bytes) */
//         byte_read(&entry[i].FILE_SIZE, 4, bytes);
//     }
// }


// void loadImage(int img){
  
//     lseek(img, 512, SEEK_SET); //skip 512 bytes
//     // for(int i = 0; i<512; i++){
//     //read(img,&vData.dataMsg[i],1);
//   //   printf(" DATA HERE -- %x\n",vData.dataMsg[i]);
//    //} 
//    read(img,&vData.dataMsg,512);

// }

void byte_seek(int len, int mode) {
    if (mode == SEEK_SET_N) {
        cur_byte = len;
    } else if (mode == SEEK_CUR_N) {
        cur_byte += len;
    }
}

void byte_read(void *buf, int len, unsigned char *bytes) {
    bzero((char *)buf, sizeof(buf));
    memcpy(buf, (bytes+cur_byte), len);
    cur_byte += len;
    

   // printf("BUYUDFSTDFT%p\n", buf);
   // printf("len%d\n", len);
    //printf("currentByte%d\n", cur_byte);
}
