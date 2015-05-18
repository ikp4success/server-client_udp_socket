#ifndef FAT12BOOTENTRY_H
#define FAT12BOOTENTRY_H
#define MAX_LENGTH 1024
#define SEEK_SET_N   1
#define SEEK_CUR_N   0


typedef struct {
    unsigned int BYTES_PER_SECTOR;
    unsigned int SECTORS_PER_CLUSTER;
    unsigned int RESERVED_SECTORS;
    unsigned int NUM_OF_FATS;
    unsigned int MAX_ROOT_DIRS;
    unsigned int TOTAL_SECTORS;
    unsigned int SECTORS_PER_FAT;
    unsigned int SECTORS_PER_TRACK;
    unsigned int NUM_OF_HEADS;
    unsigned int VOLUME_ID;
    unsigned char VOLUME_LABEL[11];
} Fat12Boot;


typedef struct {
    unsigned char FILENAME[8];
    unsigned char EXT[3];
    unsigned char ATTRIBUTES[1];
    unsigned char RESERVED[2];
    unsigned short CREATION_TIME;
    unsigned short CREATION_DATE;
    unsigned short LAST_ACCESS_DATE;
    unsigned short MODIFY_TIME;
    unsigned short MODIFY_DATE;
    unsigned short START_CLUSTER;
    unsigned long FILE_SIZE;
} Fat12Entry;

 //struct detailSend dsd;
  // typedef struct {
  //     int ClientconnectionID ;
  //     char dataMsg[MAX_LENGTH]; 

  //   } dsd;





// void loadBootTest(unsigned int * bytes);

void loadEntry(int start, unsigned char *bytes);

void loadImage(int img);

void traverse(char *  line2);

void structure(char *  line2);

void help(char * line2);

// void showSector(char *  line2,unsigned char *bytes);

// void showFat(char *  line2);

void byte_seek(int len, int mode);
void byte_read(void *buf, int len, unsigned char *bytes);



#endif
