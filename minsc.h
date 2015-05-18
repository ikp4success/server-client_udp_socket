#ifndef minsc_H_   /* Include guard */
#define minsc_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #include <errno.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include "Fat12BootEntry.h"
#define low 1
#define high 0



// void welcomeMessageFlop() {
// 	printf("\n//*******WELCOME TO FLOPPY SHELL(Client Side)*******\n");
// 	printf("\nPlease go over the read me before using this shell\n\n");
// }

// void shellLabel() {
// 	char cwd[1024];
// 	if((getcwd(cwd, sizeof(cwd)))!=NULL){
// 		getcwd(cwd, sizeof(cwd));
// 		printf("[%s]shell$ ", cwd);
// 	}
// 	else{
// 		perror("command failed");
// 	}
// }

void wipeTerminalScreen() {
	printf("\033[H\033[J");
}

void printError() {
	printf("Error:- Please Mount Remote Floppy Disk first");
	printf("\n");
}

void printCmdError() {
  printf("Error:- command does not exist, try <help> command for more assistance");
  printf("\n");
}



int equals(char * str1, char *str2) {
	int i;
	for (i = 0; i < strlen(str1); i++) {
		if (str1[i] != str2[i])
			return 0;
	}
	return 1;
}


void printPrompt(char *label) {

  printf("[\033[1;32m%s\033[0m]\033[1;36m$\033[0m ", label);
  //Or
  //write(1,"$",sizeof("$"));
}

int countWords(char s2[MAX_LENGTH]){
  //char s2[MAX_LENGTH];
 // s2=s;
  char c = '\0';
  int count = 0,nob,pos = high;

  // for (i = 0;s2[i] != '\0';i++)
  // {
  //   if (s2[i] == ' ')
  //     ++count;    
  // }
  // while(1&&(c==getchar())){
  //   if (c!=EOF){
  //     ++nc;
  //   }
  // }

 for (int i = 0;s2[i] != '\0';i++){
 
      if (s2[i] == ' ') {
         // counting number of blank spaces.
         pos = high;
         ++nob;
      } else if (pos == high) {
         // counting number of words.
         pos = low;
         ++count;
      }
    }



  return count;
}

#endif
