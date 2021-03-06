# server-client_udp_socket
Unix shell server/client, to allow user access floopy image from client shell


CIS340 Systems programing--Cleveland State University Computer Science

Project4: A Remote Floppy Disk Shell (Due at 11:59:59pm on 05/08/2015 (EST))

You are asked to develop a Floppy Disk Shell environment that allows users to access a floppy disk mounted on a remote computer. Your Floppy Disk Shell would give use an illusion that s/he is operating on a locally mounted floppy disk. You must use either C or C++ in this assignment. Your implementation must work on Linux machine in BU04 lab.

Required Modules:

[30 points] Server daemon: a single threaded UDP Floppy Disk server, which accepts three API commands:
Request for new connection.
Request for data in a specific sector of the floppy disk.
Request for disconnect.
An floppy image file is specified as the server execution argument; the server will always keep the image file open during the execution.
Note: the server does not accept the floppy commands, including structure, traverse and showsector. Instead, the server only provides the raw data, which are the contents of a certain sector from the floppy disk.
The server daemon is expected to serve only one client at a time.

Upon a successful connection, the client will receive a handle (similar to a file descriptor) from the floppy server and use the handle as the reference in all future remote requests.

When a connected client requests for data or disconnection, the floppy daemon must check whether the client's handle is consistent with its port number and inet address associated. If not, the request will be rejected.

Note: the server daemon can only access the floppy disk through the primitive operation, such as read(). Your implementation is not allowed to use system() to execute shell commands. 
[60 points] Client Shell (a floppy console): the shell environment allows users to access the remote floppy by using the following commands:
help: show the commands supported in the floppy shell.
fmount [hostname]: mount the remote floppy disk.
fumount [hostname]: umount (disconnect) the remote floppy disk.
structure: to list the structure of the floppy disck. For example:
flop: structure -l
		number of FAT:			    2
		number of sectors used by FAT:	    9
		number of sectors per cluster:	    1
		number of ROOT Entries:		  224
		number of bytes per sector:	  512
		---Sector #---     ---Sector Types---
		      0                  BOOT       
		   01 -- 09              FAT1         
		   10 -- 18              FAT2         
		   19 -- 32              ROOT DIRECTORY

traverse [-l]: list the content in the root directory. For example:
flop: traverse
/2006BH~1.PDF                           
/INS-SO~1.PDF                           
/OSDI-2~1.PDF                           
/P335-B~1.PDF                           
/STRIBL~1.PDF                           
/CIS340                                 	< DIR >
/CIS340/.                               	< DIR >
/CIS340/..                              	< DIR >

When switch -l is turned on (long list), all detailed information is shown. For example:
flop: traverse -l
	*****************************
	** FILE ATTRIBUTE NOTATION **
	**                         **
	** R ------ READ ONLY FILE **
	** S ------ SYSTEM FILE    **
	** H ------ HIDDEN FILE    **
	** A ------ ARCHIVE FILE   **
	*****************************

-A---     02/03/2012 23:21:21            285679     /2006BH~1.PDF                                    3
-A---     02/03/2012 23:21:21            208464     /INS-SO~1.PDF                                  561
-A---     02/03/2012 23:21:21            195726     /OSDI-2~1.PDF                                  969
-A---     02/03/2012 23:21:21            201572     /P335-B~1.PDF                                 1352
-A---     02/03/2012 23:21:21            417125     /STRIBL~1.PDF                                 1746
-----     02/03/2012 22:35:15        < DIR >          /CIS340                                       2561
-----     02/03/2012 22:35:15        < DIR >          /CIS340/.                                     2561
-----     02/03/2012 22:35:15        < DIR >          /CIS340/..                                       0

showsector [sector number]: show the content (in the form of hex dump) of the specified sector number (512 bytes for each sector). For example:
flop: showsector 10
         0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f

    0   f0   ff   ff    0   40    0    5   60    0    7   80    0    9   a0    0    b
   10   c0    0    d   e0    0    f    0    1   11   20    1   13   40    1   15   60
   20    1   17   80    1   19   a0    1   1b   c0    1   1d   e0    1   1f    0    2
   30   21   20    2   23   40    2   25   60    2   27   80    2   29   a0    2   2b
   40   c0    2   2d   e0    2   2f    0    3   31   20    3   33   40    3   35   60
   50    3   37   80    3   39   a0    3   3b   c0    3   3d   e0    3   3f    0    4
   60   41   20    4   43   40    4   45   60    4   47   80    4   49   a0    4   4b
   70   c0    4   4d   e0    4   4f    0    5   51   20    5   53   40    5   55   60
   80    5   57   80    5   59   a0    5   5b   c0    5   5d   e0    5   5f    0    6
   90   61   20    6   63   40    6   65   60    6   67   80    6   69   a0    6   6b
   a0   c0    6   6d   e0    6   6f    0    7   71   20    7   73   40    7   75   60
   b0    7   77   80    7   79   a0    7   7b   c0    7   7d   e0    7   7f    0    8
   c0   81   20    8   83   40    8   85   60    8   87   80    8   89   a0    8   8b
   d0   c0    8   8d   e0    8   8f    0    9   91   20    9   93   40    9   95   60
   e0    9   97   80    9   99   a0    9   9b   c0    9   9d   e0    9   9f    0    a
   f0   a1   20    a   a3   40    a   a5   60    a   a7   80    a   a9   a0    a   ab
  100   c0    a   ad   e0    a   af    0    b   b1   20    b   b3   40    b   b5   60
  110    b   b7   80    b   b9   a0    b   bb   c0    b   bd   e0    b   bf    0    c
  120   c1   20    c   c3   40    c   c5   60    c   c7   80    c   c9   a0    c   cb
  130   c0    c   cd   e0    c   cf    0    d   d1   20    d   d3   40    d   d5   60
  140    d   d7   80    d   d9   a0    d   db   c0    d   dd   e0    d   df    0    e
  150   e1   20    e   e3   40    e   e5   60    e   e7   80    e   e9   a0    e   eb
  160   c0    e   ed   e0    e   ef    0    f   f1   20    f   f3   40    f   f5   60
  170    f   f7   80    f   f9   a0    f   fb   c0    f   fd   e0    f   ff    0   10
  180    1   21   10    3   41   10    5   61   10    7   81   10    9   a1   10    b
  190   c1   10    d   e1   10    f    1   11   11   21   11   13   41   11   15   61
  1a0   11   17   81   11   19   a1   11   1b   c1   11   1d   e1   11   1f    1   12
  1b0   21   21   12   23   41   12   25   61   12   27   81   12   29   a1   12   2b
  1c0   c1   12   2d   e1   12   2f    1   13   31   21   13   33   41   13   35   61
  1d0   13   37   81   13   39   a1   13   3b   c1   13   3d   e1   13   3f    1   14
  1e0   41   21   14   43   41   14   45   61   14   47   81   14   49   a1   14   4b
  1f0   c1   14   4d   e1   14   4f    1   15   51   21   15   53   41   15   55   61

quit: quit the floppy shell.
Note: On the client side, there is no floppy image (disk). The raw data (sectors) are provided by the server process over the network. The client is supposed to request (one or more) sectors from the server to satisfy the user's command.
Communications: all the data communications between the server and the client should be based on UDP datagrams. Your implementation does not need to deal with transmission errors (actually it does not happen very often if the server and the client are sitting on the same LAN).
[10 points] Write-up: you are required to write a README document (either in TXT or PDF format) that describes your project design detail and the execution sequence (with the commands). In particular, please explicitly state which part, if there is any, does not work and the possible reasons why that module does not work. For those working modules, please give a brief (in short) sample output.
Hints:

A UDP example program in C: udpexample.tgz
Installation instructions:
 $ tar zxvf udpexample.tgz 
 $ cd udp 
 $ make 
For your convenience, we provide an example floppy image: imagefile.img. 
Submission:

Create a folder with the name of your assigned group name, e.g., Group_A.
Copy all source code files/directories into the created folder.
Make sure you have sshed to grail.cba.csuohio.edu, and go to the parent directory of Group_A, then type:
$ turnin -c cis340w -p proj4 Group_A


EXISTING BUGS
------------------
Because of time constraints,the shell unfortunately, cannot check to see if one client is connected to enable disabling of multiple client connections.
