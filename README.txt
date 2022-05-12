Student: Sofia Meropi Katsaouni  AM: 1115201800070

~~README file for hw1-spring-2022.pdf~~

for C program:

Compile with: make
( clean object files with: make clean )
( clean object files and compile: make comp )
Run with: ./sniffer [-p path]

[ if you run ./sniffer just drag and drop files on the current directory
  if you run ./sniffer -p <other_dir> just drag and drop the files to 
  <other_dir> directory. (you will create the <other_dir> directory)]

Make sure you drag and drop up to 2 files at a time (best if you drop 1
at a time), or else some filenames will not be picked up by read function.

for bash script:
chmod +x finder.sh
    and then
Run with: ./finder.sh [parameters]

!!!!!!!IMPORTANT!!!!!!!!
~ Before you execute the program make sure, < named_fifos > and < out_files > 
directories exist in the current directory, if the directories don't exist 
the program will not execute properly ~ 


Code deficiency:
-All signals except SIGSTOP are not implemented. Therefore expect diffrent
workers to be created for each file.

-I don't remove the www. prefix on urls

*******************************************************
Other than the above the code should execute just fine.
for any questions message me at: sdi1800070@di.uoa.gr
*******************************************************

---- When running the program expect the execution to look like this ----
-------------------------------------------------------------------------
Setting up watches.
Watches established.


filename is: 10.txt
filename received from worker 1 :10.txt
all done with filename: 10.txt.out
            .
            .
            .
--------------------------------------------------------------------------

Terminate program with: Ctrl+C

The code for my program is in the src directory. named_fifos directory is where 
the named fifo files get stored when created, and the out_files directory is 
where i keep the .out files  as soon as they get created. Every time you re-run
 the program both named_fifos and out_files  files will be deleted and the new 
 files of the current execution will be stored there.

The given txt files 10.txt 50.txt and 100.txt worked just fine with my program,
make sure the given files are not bigger that 100.txt or else the buffer won't 
be able to store all the given data thus errors will occur.



******* My code was implemented with the help of the following pdfs.

cgi.di.uoa.gr/~mema/courses/k24/lectures/topic3-UnixFileCommands.pdf
cgi.di.uoa.gr/~mema/courses/k24/lectures/topic4-ForkAndPipes.pdf
cgi.di.uoa.gr/~mema/courses/k24/lectures/topic2-bashShellProgramming.pdf

As for the linked list and queue implementation i used my own files from 
assignements i had for the subject Data Structures ******


There are comments everywhere in the files explaining the thought behind The
implementation. I made sure the code was clean enough for a 2nd person to 
understand. The named pipes and pipes were done exactly like we were taught
in the subject, the deletion of files in the directories also just like the
pdfs above instructed, in order to remove the http prefix i used regex.h 
library. For any further questions message me.



