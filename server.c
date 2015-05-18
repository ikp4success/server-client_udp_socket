/************* UDP SERVER CODE *******************/
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include "Fat12BootEntry.h"
#include "minsc.h"

#define DELIMS " \t\r\n"
#define LISTENPORT 13114

// #define BACKLOG  5
static int connectionID;
static int client_connectionID = -1;


/**
*   Name:       Immanuel George
*   Due Date:       8 May 2015
*   Project 4:  A Remote Floppy Disk Shell via Server/Client
*   Objective:   Unix shell server/client, to allow user access floopy image from client shell
*
*/
//dsd *dataConstruct;
int fmount(char *imageloc){

  int img = open(imageloc, O_RDONLY);
  return img;
  // loadImage(img);
}

int main(char argc, char** argv){
  wipeTerminalScreen();
  int socket_fd, client_len, nBytes;
  struct sockaddr_in server_addr, client_addr;

  int i;

  struct {
    int connectionID ;
    char dataMsg[MAX_LENGTH]; 
    
    

  } msg_server_send,msg_server_recieve;

    srand(time(NULL));//genereates ID, for unique client validation
    connectionID = rand();
    /* Open UDP Socket */
  socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd < 0) { //socket() failed
      perror("socket");
      exit(EXIT_FAILURE);
    }

     /* Write zero's to server_addr */
    bzero((char *)&server_addr, sizeof(server_addr));

    /* Set up server */
    server_addr.sin_family =AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(LISTENPORT);



    if (argc != 2) {
      printf("[ERROR] Usage: %s <image>\n", argv[0]);
      exit(EXIT_FAILURE);
    }else{
      printf("ImageFile--  %s\n",argv[1]);
      printf("Port Number--  %d\n",LISTENPORT);
      printf("Connected Server\n\n");

        /*Create UDP socket*/




   /* Bind server */
      if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        //bind() fails to open if something is already using the ip:port
        perror("bind");
        exit(EXIT_FAILURE);
      }

    /* Listen for connections*/
    // listen(socket_fd, BACKLOG);


      //     bzero((char *)msg_server_send.dataMsg, MAX_LENGTH);
      // bzero((char *)msg_server_recieve.dataMsg, MAX_LENGTH);


      while(1){

       bzero((char *)msg_server_send.dataMsg, MAX_LENGTH);
       bzero((char *)msg_server_recieve.dataMsg, MAX_LENGTH);

        msg_server_recieve.connectionID = -1;
        msg_server_send.connectionID = connectionID;


       client_len = sizeof(client_addr);

       nBytes = recvfrom(socket_fd, &msg_server_recieve, sizeof(msg_server_recieve), 0, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);

  if (nBytes < 0) { //recvfrom() failed
    perror("socket:recvfrom");
    continue;
  }
  printf("%s:%d >>> %s\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), msg_server_recieve.dataMsg);
    fflush(stdout); //Safety precaution

 // printf("Message from Client-- %s\n",msg_server_recieve.dataMsg );
    if(strstr(msg_server_recieve.dataMsg,"fmount")!=NULL){

      if (client_connectionID != -1) { //Invalid client connection ID
        sprintf(msg_server_send.dataMsg, "[ERROR] -> Client in session. Your Request to Connect has been rejected");
        break;
      }
      int img =fmount(argv[1]);

      char *ptr = msg_server_recieve.dataMsg;

      ptr += strlen("fmount");
            //printf("CHECK HERE--%s\n",ptr );
      while (*ptr == ' '){
        ptr++;

        int sector = atoi(ptr);
            lseek(img, 512 *sector, SEEK_SET); //skip 512 bytes

            

            read(img,&msg_server_send.dataMsg,512);
            break;
          }

             printf("Mounted %s\n", argv[1]);


        }else if(strstr(msg_server_recieve.dataMsg,"fumount")!=NULL){

           if (client_connectionID != -1) { //Invalid client connection ID
            sprintf(msg_server_send.dataMsg, "[ERROR] -> Client in session. Your Request to Connect has been rejected");
            break;
          }

          sprintf(msg_server_send.dataMsg, "fumount");
          printf("umount remote floppy from %s:%d [%d]\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), msg_server_send.connectionID);
           bzero((char *)msg_server_send.dataMsg, MAX_LENGTH);
          bzero((char *)msg_server_recieve.dataMsg, MAX_LENGTH);
        }else if(strstr(msg_server_recieve.dataMsg,"quit")!=NULL){

          //sprintf(msg_server_send.dataMsg, "quit");
           printf("Disconnected from %s:%d [%d]\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), msg_server_send.connectionID);
          
            break;
        }

       


       /*
         * Send response to the client
         * We need to use sendto() because
         * we are using a UDP socket.
         */

         nBytes = sendto(socket_fd, &msg_server_send, sizeof(msg_server_send), 0, (struct sockaddr *)&client_addr, (socklen_t)client_len);

        if (nBytes < 0) { //sendto() failed
          perror("socket:sendto");
          continue;
        }

        printf("%s:%d <<< %s\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), (unsigned char *)msg_server_send.dataMsg);
        fflush(stdout); //Safety precaution



      }
    }


    return 0;
  }

