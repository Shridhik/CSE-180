#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>

int main(int argc, char *argv[])
{
char buf[32]; //direct variable declaration for 32 blocks of memory 
int file, i; 
char* a = "-";
i = 1; // counter
	if (argc == 1) // if there is exactly one argument (just dog function)
	{
		while ( read(STDIN_FILENO,buf,1)==1) //read input 
		   write(STDOUT_FILENO, buf,1);  // write output
	}
	else  // if there are multiple arguments
	{
	while (i<argc)  // go through each of the arguments 
	{
		file = open(argv[i], O_RDONLY ); // open file 
	
        
        // Checks for -
        
      	if (strcmp(argv[i], a)==0) // if there is exactly one argument (just dog function)
	{
		while ( read(STDIN_FILENO,buf,1)==1) //read input 
		   write(STDOUT_FILENO, buf,1);  // write output
          break;
	}  
        
        
     
        
        
		if (file < 0) // if the file does not exist, throw an error
		{
			perror (argv[i] ); 
			break; //continue;
		}
	i++; // increment counter to read next file
		while (read (file, buf, 1 ) == 1) // if the file exists-read file
	       		write(STDOUT_FILENO, buf, 1 ); // write to output
 		close (file); // close file

	
	}

	}
}
