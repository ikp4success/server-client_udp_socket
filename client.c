/************* UDP CLIENT CODE *******************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "minsc.h"
#include "Fat12BootEntry.h"
#include <stdlib.h>
#include <netdb.h>
#include <time.h>



#define DELIMS " \t\r\n"

#define PORT 13114

static int connectionID;
static int server_connectionID = -1;



/**
*   Name:       Immanuel George
*   Due Date:       8 May 2015
*   Project 4:  A Remote Floppy Disk Shell via Server/Client
*   Objective:   Unix shell server/client, to allow user access floopy image from client shell
*
*/

//dsd dataConstruct;

char *cmd;
char line[MAX_LENGTH];
char line2[MAX_LENGTH];

Fat12Boot boot;
Fat12Entry *entry;
int isMounted = 0;


void loadBoot(unsigned char *bytes){

 /// printf("HERER\n");


    byte_seek(11, SEEK_SET_N); //skip 11 bytes
    

    /* BYTES_PER_SECTOR (2 bytes) */


    byte_read(&boot.BYTES_PER_SECTOR, 2, bytes);

    /* SECTORS_PER_CLUSTER (1 byte) */
    byte_read(&boot.SECTORS_PER_CLUSTER, 1, bytes);


    /* RESERVED_SECTORS (2 bytes) */
    byte_read(&boot.RESERVED_SECTORS, 2, bytes);


    /* NUM_OF_FATS (1 byte) */
    byte_read(&boot.NUM_OF_FATS, 1, bytes);
     // printf("NUM_OF_FATS%p\n", &boot.NUM_OF_FATS);

    /* MAX_ROOT_DIRS (2 bytes) */
    byte_read(&boot.MAX_ROOT_DIRS, 2, bytes);

    /* TOTAL_SECTORS (2 bytes) */
    byte_read(&boot.TOTAL_SECTORS, 2, bytes);

    byte_seek(1, SEEK_CUR_N); //skip 1 byte

    /* SECTORS_PER_FAT (2 bytes) */
    byte_read(&boot.SECTORS_PER_FAT, 2, bytes);

    /* SECTORS_PER_TRACK (2 bytes) */
    byte_read(&boot.SECTORS_PER_TRACK, 2, bytes);

    /* NUM_OF_HEADS (2 bytes) */
    byte_read(&boot.NUM_OF_HEADS, 2, bytes);

    byte_seek(11, SEEK_CUR_N); //skip 11 bytes

    /* VOLUME_ID (4 bytes) */
    byte_read(&boot.VOLUME_ID, 4, bytes);
    
    /* VOLUME_LABEL (11 bytes) */
    byte_read(&boot.VOLUME_LABEL, 11, bytes);




  }

  void loadEntry(int start, unsigned char *bytes) {
    /* Set reader to 0 */

    byte_seek(0, SEEK_SET_N);

    /* Load data into entry */
    for (int i = start; i < (start + 16); i++) {
        /* FILENAME (8 bytes) */
      byte_read(&entry[i].FILENAME, 8, bytes);

        /* EXT (3 bytes) */
      byte_read(&entry[i].EXT, 3, bytes);

        /* ATTRIBUTES (1 byte) */
      byte_read(&entry[i].ATTRIBUTES, 1, bytes);

        /* RESERVED (10 bytes) */
      byte_read(&entry[i].RESERVED, 2, bytes);

        /* CREATION_TIME (2 bytes) */
      byte_read(&entry[i].CREATION_TIME, 2, bytes);

        /* CREATION_DATE (2 bytes) */
      byte_read(&entry[i].CREATION_DATE, 2, bytes);

        /* LAST_ACCESS_DATE (2 bytes) */
      byte_read(&entry[i].LAST_ACCESS_DATE, 2, bytes);

        byte_seek(2, SEEK_CUR_N); //skip 2 bytes

        /* MODIFY_TIME (2 bytes) */
        byte_read(&entry[i].MODIFY_TIME, 2, bytes);

        /* MODIFY_DATE (2 bytes) */
        byte_read(&entry[i].MODIFY_DATE, 2, bytes);

        /* START_CLUSTER (2 bytes) */
        byte_read(&entry[i].START_CLUSTER, 2, bytes);

        /* FILE_SIZE (4 bytes) */
        byte_read(&entry[i].FILE_SIZE, 4, bytes);
      }
    }

    int main(int argc, char** argv){
   // welcomeMessageFlop();
      wipeTerminalScreen();

      int sock_fd, serv_len, nBytes;
      struct sockaddr_in serv_addr;
      struct hostent *serv;

      struct {
        int connectionID ;
        char dataMsg[MAX_LENGTH];
        


      } msg_client_send,msg_client_recieve;

       srand(time(NULL));//genereates ID, for unique client validation
       connectionID = rand();



       printf("Port Number--  %d\n",PORT);
       printf("Connected Client to Server\n");


       if (argc > 1) {
        printf("[ERROR] Usage: %s Run just ./client without any arguments \n", argv[0]);
        exit(EXIT_FAILURE);
      }



      while(1){
       // printf("client: ");
        printPrompt("Client:");

        bzero((char *)msg_client_send.dataMsg, sizeof(msg_client_send.dataMsg));
        bzero((char *)msg_client_recieve.dataMsg, sizeof(msg_client_recieve.dataMsg));
        msg_client_recieve.connectionID = -1;
        msg_client_send.connectionID = connectionID;


        if (!fgets(line, MAX_LENGTH, stdin)) break;
        strncpy(line2,line,MAX_LENGTH);

       //cmd = (char *)malloc(sizeof(char) * MAX_LENGTH);







        // Parse and execute command
        if ((cmd = strtok(line, DELIMS))) {
          // Clear errors
          errno = 0;


          if (strcmp(cmd, "fmount") == 0 ) {
            // if (msg_client_recieve.connectionID != msg_client_send.connectionID) { //Invalid client connection ID
            //   sprintf(msg_client_send.dataMsg, "[ERROR] -> Client in session already");
            //   break;
            // }
            if(isMounted >0){
              printf("Remote Server is already connected\n");
            }else{

              char * fCommand;
              char _cfmd[MAX_LENGTH]="";
              fCommand =strtok(line2," ");
              while (fCommand !=NULL){
                if(strcmp(fCommand, "fmount") != 0){

         //serv = gethostbyname(argv[1]);
         //strcpy(_cfmd,fCommand);
           // if (strcmp(cmd, "fmount") == 0 ){

           // }
                  strcpy(_cfmd,fCommand);

                  int i =0;
                  sscanf (fCommand,"%s %*s %d",_cfmd,&i);
              // strcpy(_cfmd,fCommand);
                }




                fCommand = strtok(NULL," ");

              }
            //printf("%s\n", _cfmd);
              printf("Attempting to Connect...\n");
            //fflush(stdout); //Safety precaution
              if(strcmp(_cfmd, "fmount") != 0){

                serv = gethostbyname(_cfmd);
                if(serv){
                 printf("Found `%s\n`... ", _cfmd);



                /* Open UDP socket */
                 sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                 if (sock_fd < 0) {
                  perror("socket");
                  break;
                }

                /* Write zero's to serv_addr */
                bzero((char *)&serv_addr, sizeof(serv_addr));

                /* Set up server */
                serv_addr.sin_family = AF_INET;
                //bcopy(serv->h_addr, (char *)&serv_addr.sin_addr, serv->h_length);
                //serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
                memcpy(&serv_addr.sin_addr, serv->h_addr_list[0], serv->h_length);
                serv_addr.sin_port = htons(PORT);
                serv_len = sizeof(serv_addr);

                      /* Save server UUID */
                server_connectionID = msg_client_recieve.connectionID;

                bzero((char *)msg_client_send.dataMsg, sizeof(msg_client_send.dataMsg));
                bzero((char *)msg_client_recieve.dataMsg, sizeof(msg_client_recieve.dataMsg));

                msg_client_send.connectionID = connectionID;
                msg_client_recieve.connectionID = -1;

                sprintf(msg_client_send.dataMsg, "fmount 0");

                nBytes = sendto(sock_fd, &msg_client_send, sizeof(msg_client_send), 0, (struct sockaddr *)&serv_addr, (socklen_t)serv_len);
                if (nBytes < 0) {
                  perror("sendto");
                  break;
                }

                /* Get response from server */
                nBytes = recvfrom(sock_fd, &msg_client_recieve, sizeof(msg_client_recieve), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&serv_len);
                if (nBytes < 0) {
                  perror("recvfrom");
                  break;
                }






            //printf("dataConstruct.dataMsg address: %s\n", msg_client_recieve.dataMsg);
                loadBoot((unsigned char *)msg_client_recieve.dataMsg);
  //}   

             /* malloc Fat12Entry */

                entry = (Fat12Entry *)malloc(sizeof(Fat12Entry) * boot.MAX_ROOT_DIRS);
                //free(entry);


             /* Request all ROOT_DIRECTORY sector entries */
                for (int i = 0; i < 14; i++) {
                    /* Clear msg_in and msg_out */
                  bzero((char *)&msg_client_send.dataMsg, sizeof(msg_client_send.dataMsg));
                  bzero((char *)&msg_client_recieve.dataMsg, sizeof(msg_client_recieve.dataMsg));
              //fflush(stdout); //Safety precaution
                  msg_client_send.connectionID = connectionID;
                  msg_client_recieve.connectionID = -1;

                    /* Get ROOT DIRECTORY sector from server */
                  sprintf(msg_client_send.dataMsg, "fmount %d", ((boot.NUM_OF_FATS * boot.SECTORS_PER_FAT) + 1) + i);
                  nBytes = sendto(sock_fd, &msg_client_send, sizeof(msg_client_send), 0, (struct sockaddr *)&serv_addr, (socklen_t)serv_len);
                  if (nBytes < 0) {
                    perror("sendto");
                    break;
                  }

                    /* Get response from server */
                  nBytes = recvfrom(sock_fd, &msg_client_recieve, sizeof(msg_client_recieve), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&serv_len);
                  if (nBytes < 0) {
                    perror("recvfrom");
                    break;
                  }

                    /* Load bytes into Fat12Entry */
                  loadEntry(16 * i, (unsigned char *)msg_client_recieve.dataMsg);


                }

                printf("Connected Succesfully\n");

                isMounted = 1;
              }else{
                printf("Could not find--%s\n",_cfmd );
                herror("gethostbyname");
              }
            }else{
              printf("Error occured when connecting...\n");
              printf("[ERROR] Usage: fmount <server>\n");
            }


          }


        }else if (strcmp(cmd, "showsector") == 0) {
            // if (server_connectionID != -1) { //Invalid client connection ID
            //   printf(msg_client_send.dataMsg, "[ERROR] -> Server is not connected");
            //   break;
            // }
          if(isMounted == 0){
            printError();
          }else{
            if(countWords(line2)>1){
              int sector;
              char *sm;
              sm = strtok(line2," ");

              while(sm !=NULL){
               if(strcmp(sm, "showsector") != 0){
                int smi = atoi( sm );
                sector = smi;
         // showSector(line2,(unsigned char *)msg_client_recieve.dataMsg);
              }
              sm= strtok(NULL," ");
            }
            if(sector)
         /* Send GETSECTOR command to server */
              sprintf(msg_client_send.dataMsg, "fmount %d", sector);
            nBytes = sendto(sock_fd, &msg_client_send, sizeof(msg_client_send), 0, (struct sockaddr *)&serv_addr, (socklen_t)serv_len);
            if (nBytes < 0) {
              perror("sendto");
              break;
            }

                /* Get response from server */
            nBytes = recvfrom(sock_fd, &msg_client_recieve, sizeof(msg_client_recieve), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&serv_len);
            if (nBytes < 0) {
              perror("recvfrom");
              break;
            }

                // /* Check for valid server UUID */
            
        //     if (server_connectionID != msg_client_recieve.connectionID) { //Invalid client connection ID
        // printf("[ERROR] Invalid server connectionID - Connection rejected\n");
        // break;
    //  }
                 //loadBoot((unsigned char *)msg_client_recieve.dataMsg);
                /* Read response */
            printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
            for (int i = 0; i < boot.BYTES_PER_SECTOR; i++) {
              if (i % 16 == 0 || i == 0) {
                printf("\n");
                printf("%4x", i);
              }
              printf("%5x", (unsigned char)msg_client_recieve.dataMsg[i]);
            }
            printf("\n");

          }else{

           printf("[ERROR] Usage: showsector <sector>\n");
         }
       }


     }else if (strcmp(cmd, "showfat") == 0 ) {
        // if (server_connectionID != -1) { //Invalid client connection ID
        //   printf(msg_client_send.dataMsg, "[ERROR] -> Server is not connected");
        //   break;
        // }
      if(isMounted == 0){
        printError();
      }else{
       if(countWords(line2) > 1){
        printf("[ERROR] Usage: showfat, try <help> for more assistance \n");
        break;
      }else{
        char * free = "FREE";
        int sectors = (boot.NUM_OF_FATS * boot.SECTORS_PER_FAT);
        int count = 0;
        printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");

                /* Send GETSECTOR command to server */
        for (int i = 1; i <= sectors; i++) {
          sprintf(msg_client_send.dataMsg, "fmount %d", i);
          nBytes = sendto(sock_fd, &msg_client_send, sizeof(msg_client_send), 0, (struct sockaddr *)&serv_addr, (socklen_t)serv_len);
          if (nBytes < 0) {
            perror("sendto");
            break;
          }

                    /* Get response from server */
          nBytes = recvfrom(sock_fd, &msg_client_recieve, sizeof(msg_client_recieve), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&serv_len);
          if (nBytes < 0) {
            perror("sendto");
            break;
          }

                    // /* Check server UUID */
                    // if (serv_uuid != msg_client_recieve.uuid) {
                    //     printf("[ERROR] Invalid server UUID - Connection rejected\n");
                    //     break;
                    // }

      //       if (server_connectionID != msg_client_recieve.connectionID) { //Invalid client connection ID
      //   printf("[ERROR] Invalid server connectionID - Connection rejected\n");
      //   break;
      // }

          for (int j = 0; j < boot.BYTES_PER_SECTOR; j++) {
            if (count % 16 == 0 || count == 0) {
              printf("\n");
              printf("%4x", count);
            }

            if( (unsigned char)msg_client_recieve.dataMsg[j]== 0){
              printf("%5s",free);
            }
            else{
             printf("%5x", (unsigned char)msg_client_recieve.dataMsg[j]);
           }
                       // printf("%5x", (unsigned char)msg_client_recieve.dataMsg[j]);
           count++;
         }
       }
       printf("\n");
     }
   }

 }else if (strcmp(cmd, "fumount") == 0 ) {
        // if (server_connectionID != -1) { //Invalid client connection ID
        //   sprintf(msg_client_send.dataMsg, "[ERROR] -> Server is not connected");
        //   break;
        // }
  if(isMounted == 0){
    printf("Cannot umount remote floopy, because floppy wasn't mounted in the first place\n");
    printf("try <help> command for more assistance\n");
  }else{
    if(countWords(line2) > 1){
      printf("[ERROR] Usage: fumount , try <help> for more assistance\n");
      break;
    }else{
     sprintf(msg_client_send.dataMsg, "fumount");
     nBytes = sendto(sock_fd, &msg_client_send, sizeof(msg_client_send), 0, (struct sockaddr *)&serv_addr, (socklen_t)serv_len);
     if (nBytes < 0) {
      perror("sendto");

      break;
    }

                /* Close and write zero's to server */
    close(sock_fd);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    bzero((char *)msg_client_send.dataMsg, sizeof(msg_client_send.dataMsg));
    bzero((char *)msg_client_recieve.dataMsg, sizeof(msg_client_recieve.dataMsg));
    server_connectionID = -1;
    isMounted = 0;
  }
}


}else if (strcmp(cmd, "structure") == 0 ) {

          // printf("NUMOFFATS-- %d\n", boot.NUM_OF_FATS);

 if(isMounted == 0){
  printError();
}else{
  structure(line2);
}



}else if (strcmp(cmd, "traverse") == 0 ) {

 if(isMounted == 0){
  printError();
}else{
  traverse(line2);
}


}else if (strcmp(cmd, "help") == 0 ) {
  help(line2);
}else if (strcmp(cmd, "quit") == 0 ) {

  printf("Quitting flopp shell.......\n");
  sprintf(msg_client_send.dataMsg, "quit");
     nBytes = sendto(sock_fd, &msg_client_send, sizeof(msg_client_send), 0, (struct sockaddr *)&serv_addr, (socklen_t)serv_len);
     if (nBytes < 0) {
      printf("Could not quit server shell, because you either did not fmount [hostname] or you fumount\n");
      printf("To quit server shell remotely, you have to fmount [hostname], then quit\n");
      perror("sendto");

      break;
    }

     close(sock_fd);
    bzero((char *)&serv_addr, sizeof(serv_addr));

  break;
}
else{
  printCmdError();
}
}
}
return 0;
}
