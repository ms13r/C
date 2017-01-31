1. Name: Miroslav Sanader
FSUID: ms13r
Course: COP4610
Assignment: 1
Due: 16/09/2016

2. The following mini-shell exectutes commands that are given through two modes: batch mode (which reads 
from a batch file), as well as interactive user mode, which allows for the user to input commands. The 
commands are then parsed, andsent to the execvp() function, which then executes the command. A custom struct
that is typedefined as command was created to store a temporary array of instructions for each line, and is 
used when tokenizing the instruction (stored in name), and args, which are passed into execvp after tokenization 
of each command is done.

The shell utilizes strtok() to parse through an input of up to 512 characters, firstby grabbing each command, 
then storing the commands into a struct that contains two items: the instruction itself, as well as an array of 
character pointers that store the instruction and its respective arguments that are used to call execvp() to execute 
the shell command. As each command is executed asynchronously, the instruction is printed to the screen (without args),
and then the child attempts to execute. If the child cannot be forked, then a message is printedto stderr. 

Finally, the final loop waits through all the child processes, and prints out their status code. Any additional 
printing such as bash errors are done by execvp, and if execvp failed, an error isprinted to standard error to notify 
that the command failed. 

Ambiguities such as spaces and multiple semicolons were handled by execvp() and strtok, respectively. For multiple
spacing or whitespace, the shell still attempts to execute the commands, as do regular terminals. It simply
does not yield any results. As for multiple semicolons, they are ignored as they are delimiters in strtok, and are
naturally replaced by a '\0' within the strtok function. The check for null is later done when storing the data within
the array of command structs.

3. The following error checking for fatal errors is performed within the program: checking for incorrect
number of command line arguments, and checking if the batch file that is attemped to be opened is a valid file.
The following error checking is performed for errors that are not fatal, but should not alter data or execute:
input stream receives more than 512 bytes of information (the stream is then flushed using getchar()), the command
attempted does not execute (error message), and the forking failing (error message).

4. Known issues in the project include:
	- Commands such as "grep "int i" shell.c" will yield an error due to tokenization by whitespace
	- Command performed does not print full command arguments list due to altering by strtok
	- Exit status may print later than command execution due to threading

Currently, there are no known warnings. 