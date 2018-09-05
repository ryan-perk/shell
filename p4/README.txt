# Project 4
Ryan Perkins (811883022)
Andrew Alexander (811599910)

## Instructions
Clean: rm -f *.cpp~ *.o shell 

Compile: make

Run: ./shell

Once the program is running, you can enter any command currently 
available, just as you would on any other interface. Redirection is also 
supported, using the <, >, and >> keys. Some examples are provided below:

1730sh:/dir/$ COMMAND arg1
1730sh:/dir/$ COMMAND > file.txt
1730sh:/dir/$ COMMAND arg1 < file.txt
1730sh:/dir/$ COMMAND arg1 arg2 >> file.txt

To change/add an environment variable, simply use the export command and 
list variables after as seen below:

1730sh:/dir/$ export NAME=WORD
1730sh:/dir/$ export NAME=WORD MESSAGE=hello

All other unix commands should run through the prompt normally.

## Division of Labor
Ryan : (prompt, env variables, launch process)
Andrew : (launch process, redirect)

Ryan worked on the initial prompt, environmental variables, and began 
the launch process, while Andrew finished the launch process and 
redirection. That said, there was a significant amount of cooperation 
do to the small scale of the project and overlapping functions.