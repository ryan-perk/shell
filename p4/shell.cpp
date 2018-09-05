/*
 * @author Andrew Alexander, Ryan Perkins
 */
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

char **args;
char *cmd;
int numArg;
bool rdirct = false;

/*
 * (Ryan) find method that divides the read line into an array
 *
 * @param line string to be parsed
 */
void find(string line) {
    char *temp = new char[128];
    int count = 0;
    
    // loops through each character until space is found then adds it to the arguments array, or until it finds a redirection prompt
    for(unsigned int i = 0; (i < line.size()) && (!rdirct); i++) {
        if((line[i] == '<') || (line[i] == '>'))
            rdirct = true;
        
        if(line[i] == ' ') { // searching for delimiter
            args[numArg] = temp; // adds to args array
            temp = new char[1024]; // resets temp after argument added
            numArg++; // increments # of arguments
            count = 0;
        }
        
        else if(rdirct == false) {
            temp[count] = line[i];
            count++;
        }
    }
    
    if(rdirct == false) { // adds final element
    	args[numArg] = temp;
    	numArg++;
    }
    
    args[numArg] = NULL; // make final element of args NULL
    temp = NULL;
}

/*
 * (Ryan) getCurDir method that returns current working directory
 */
string getCurDir() {
    char temp[1024];
    return (getcwd(temp, 1024) ? temp : "");
}

/*
 * (Ryan) exe method that executes the commands passed through the prompt
 */
void exe() {
    pid_t pid;
    int status;
    cmd = args[0];
    
    // fork
    if((pid = fork()) < 0) {
        printf("fork error\n");
        exit(1);
    }
    
    // child
    else if(pid == 0) {
        if(execvp(cmd, args) < 0) { // executes command
            cout << "invalid command: " << cmd << endl;
            exit(1); // exits on failure and prints invalid command
        }
    }
    
    // parent
    else {
        while(wait(&status) != pid); // waits till child finishes
    }
}

/*
 * (Andrew) rdirect method that reads the redirection from the user and performs the redirection
 *
 * @param args list of arguments to be performed
 * @param line string to read redirection from
 */
void rdirect(char** args, string line){
	char *temp2 = new char[128];
	int status = 0, backup;
	//searches for redirection prompt, then copies the address for use later
	for (unsigned int i=0; i < line.size(); ++i){
		if ((line[i] == '<') | (line[i] == '>')){
			if (line[i]=='<'){
				status=1;
			}
			if (line[i]=='>'){
				if (line[i+1]=='>'){
					status=3;
					++i;//adjust for extra space over
				}
				else 
					status=2;
			}
			if (line[i+1]==' ')//moves up
				i+=2;
			else{
				++i;
			}
			for (unsigned int count=0; i<line.size(); ++i){
				temp2[count]=line[i];
				++count;
			}
		}
	}
	int fd;
	switch(status){//determines which redirection to use
		case 1://read STDIN_FILENO from a file
			if ((fd=open(temp2, O_RDONLY)) < 0){
				cout << "Error reading from file!" << endl;
				break;
			}
			backup = dup(STDIN_FILENO);
			if (dup2(fd, STDIN_FILENO) < 0){
				cout << "Error duplicating input!" << endl;
				break;
			}
			exe();
			if (dup2(backup, STDIN_FILENO) < 0){
				cout << "Error returning input!" << endl;
				break;
			}
			if (close(fd) < 0){
				cout << "Error closing file!" << endl;
				break;
			}
			break;
		case 2://write STDOUT_FILENO to a file
			if ((fd=open(temp2, O_WRONLY | O_CREAT, 0666)) < 0){
				cout << "Error reading from file!" << endl;
				break;
			}
			backup = dup(STDOUT_FILENO);
			if (dup2(fd, STDOUT_FILENO) < 0){
				cout << "Error duplicating output!" << endl;
				break;
			}
			exe();
			if (dup2(backup, STDOUT_FILENO) < 0){
				cout << "Error returning output!" << endl;
				break;
			}
			if (close(fd) < 0){
				cout << "Error closing file!" << endl;
				break;
			}
			break;
		case 3://append STDOUT_FILENO to a file
			if ((fd=open(temp2, O_WRONLY | O_APPEND)) < 0){
				cout << "Error reading from file!" << endl;
				break;
			}
			backup = dup(STDOUT_FILENO);
			if (dup2(fd, STDOUT_FILENO) < 0){
				cout << "Error duplicating output!" << endl;
				break;
			}
			exe();
			if (dup2(backup, STDOUT_FILENO) < 0){
				cout << "Error returning output!" << endl;
				break;
			}
			if (close(fd) < 0){
				cout << "Error closing file!" << endl;
				break;
			}
			break;		
		default://in event that none are chosen
			cout << "Error with switch statement!" << endl;
	}
}
/*
 * main method
 */
int main() {
    string line;
    args = new char*[11];
    numArg = 0;
    
    while(1) {
        rdirct=false;
        
        // prompt with current working directoy that separates arguments into an array
        cout << "1730sh:" << getCurDir() << "/$ ";
        getline(cin, line);
        find(line);
        
        // checks for exit
        if(strcmp(args[0], "exit") == 0) {
            return EXIT_SUCCESS;
        }
        
        // checks for export
        else if(strcmp(args[0], "export") == 0) {
            for(int i = 1; i < numArg; i++)
                putenv(args[i]);
        }
        
        // redirection
        else if(rdirct == true)
            rdirect(args, line);
        
        // executes commands
        else
            exe();
        
        // resets argument array after each execute
        for(int i = 0; i < numArg; i++)
            args[i] = NULL;
        cmd = NULL;
        numArg = 0;
    }
    
    return EXIT_SUCCESS;
}//end of program
