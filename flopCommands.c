  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <stdbool.h>
  #include "Fat12BootEntry.h"


  #define DELIMS " \t\r\n"
Fat12Boot boot;
Fat12Entry *entry;






void traverse(char *  line2){

	if (strstr(line2,"l") !=NULL){
		printf("    *****************************\n");
		printf("    ** FILE ATTRIBUTE NOTATION **\n");
		printf("    **                         **\n");
		printf("    ** R ------ READ ONLY FILE **\n");
		printf("    ** S ------ SYSTEM FILE    **\n");
		printf("    ** H ------ HIDDEN FILE    **\n");
		printf("    ** A ------ ARCHIVE FILE   **\n");
		printf("    *****************************\n");
		printf("\n");

		for (int i = 0; i < boot.MAX_ROOT_DIRS; i++) {
			if (entry[i].FILENAME[0] != 0x00 && entry[i].START_CLUSTER != 0) {
				char attr[6] = {'-', '-', '-', '-', '-'};
				unsigned char a = entry[i].ATTRIBUTES[0];
				if (a == 0x01)
					attr[0] = 'R';
				if (a == 0x02)
					attr[1] = 'H';
				if (a == 0x04)
					attr[2] = 'S';
				if (a == 0x20)
					attr[5] = 'A';
				if (a == 0x10) {
					for (int j = 0; j < 6; j++)
						attr[j] = '-';
				}

				if (entry[i].ATTRIBUTES[0] == 0x10) {
					printf("%.6s    %d %d       < DIR >      /%.8s                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, entry[i].START_CLUSTER);
					printf("%.6s    %d %d       < DIR >      /%.8s/.                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, entry[i].START_CLUSTER);
					printf("%.6s    %d %d       < DIR >      /%.8s/..                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, 0);
				} else {
					printf("%.6s    %d %d       %lu      /%.8s.%.3s                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILE_SIZE, entry[i].FILENAME, entry[i].EXT, entry[i].START_CLUSTER);
				}
			}
		}
	} else {
		for (int i = 0; i < boot.MAX_ROOT_DIRS; i++) {
			if (entry[i].FILENAME[0] != 0x00 && entry[i].START_CLUSTER != 0) {
				if (entry[i].ATTRIBUTES[0] == 0x10) {
					printf("/%.8s                       < DIR >\n", entry[i].FILENAME);
					printf("/%.8s/.                     < DIR >\n", entry[i].FILENAME);
					printf("/%.8s/..                    < DIR >\n", entry[i].FILENAME);
				} else {
					printf("/%.8s.%.3s\n", entry[i].FILENAME, entry[i].EXT);
				}
			}
		}
	}

}

void structure(char * line2){

	printf("        number of FAT:                      %d\n", boot.NUM_OF_FATS);
	printf("        number of sectors used by FAT:      %d\n", boot.SECTORS_PER_FAT);
	printf("        number of sectors per cluster:      %d\n", boot.SECTORS_PER_CLUSTER);
	printf("        number of ROOT Entries:             %d\n", boot.MAX_ROOT_DIRS);
	printf("        number of bytes per sector          %d\n", boot.BYTES_PER_SECTOR);
	if (strstr(line2,"l") !=NULL) {
		printf("        ---Sector #---      ---Sector Types---\n");
		printf("             0                    BOOT\n");
		for (int i = 0; i < boot.NUM_OF_FATS; i++)
			printf("          %02d -- %02d                FAT%d\n", (boot.SECTORS_PER_FAT * i) + 1, boot.SECTORS_PER_FAT * (i + 1), i);

		printf("          %02d -- %02d                ROOT DIRECTORY\n", boot.SECTORS_PER_FAT * boot.NUM_OF_FATS, (boot.MAX_ROOT_DIRS / 16) + (boot.SECTORS_PER_FAT * boot.NUM_OF_FATS));
	}

}




void help(char * line2){




  	write(1,"Here are some commands supported by the floppy console:\n",70-14);
	write(1,"\tfmount: mounts the floppy images in the server.\n",70-14);
	write(1,"\t   Example: 'fmount [hostname]'\n",70-14);
  	write(1,"\tfumount: Unmounts the floppy images.\n",53-14);
  	write(1,"\t   Example: 'fumount'\n",38-14);
  	write(1,"\tstructure: Lists the structure of the floppy disk.\n",67-14);
  	write(1,"\t   Example: 'structure [-l]'\n",-14);
  	write(1,"\ttraverse: Shows contents with root directory.\n",62-14);
  	write(1,"\t   Example: 'traverse [-l]'\n",44-14);
  	write(1,"\tshowfat: Show FAT tables.\n",42-14);
  	write(1,"\t   Example: 'showfat'\n",38-14);
  	write(1,"\tshowsector: Shows the contents of a sector.\n",60-14);
  	write(1,"\t            Add '> [file]' to create file and save sector info.\n",80-14);
  	write(1,"\t   Example: 'showsector [number]'\n",50-14);
  	write(1,"\t            'showsector [number] > [file]'\n",59-14);
  	write(1,"\thelp: Shows commands.\n",38-14);
  	write(1,"\t   Example: 'help'\n",35-14);
  	write(1,"\texit: Exits the floppy program.\n",48-14);
  	write(1,"\t   Example: 'exit'\n",35-14);


}
