// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#include <string.h> 
#include <iostream> 
#define PORT 8080
#define BUF_SIZE 1024
using namespace std;


void header(int handler, int status) 
{
  char header[BUF_SIZE];
  if (status == 0) 
  {
    sprintf(header, "HTTP/1.1 200 OK\r\n\r\n");
  } 
  else if (status == 1) 
  {
    sprintf(header, "HTTP/1.1 403 Forbidden\r\n\r\n");
  } 
  else 
  {
    sprintf(header, "HTTP/1.1 404 Not Found\r\n\r\n");
  }
  send(handler, header, strlen(header), 0);
  printf("%s",header);
}


int main(int argc, char const *argv[]) 
{ 
  //  char *filename;
    char buf[BUF_SIZE] = {0};
	int server_fd, new_socket, valread; 
	struct sockaddr_in serverAddr; 
	//int opt = 1; 
	int addrlen = sizeof(serverAddr); 
	char response[BUF_SIZE] = {0}; 
    char* split_response;
    char* split_response1;
    char* fname;
    char* token2;
    char* len;
    int file, length;
    int fd;
    int i=0;
    char* st;

    //added the const to get rid of error message
	//char const *hello = "Hello from server"; 
	
    if(argc < 2)
    {
        printf("\n Usage:");
        return(0);
    }
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_addr.s_addr = INADDR_ANY; 
	serverAddr.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&serverAddr, 
								sizeof(serverAddr))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&serverAddr, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

    valread = recv( new_socket , response, sizeof(response),0); 
      
    // Returns first token 
    char* token = strtok(response, "\r\n"); 


    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while (token != NULL) { 
        if (strstr(token, "Content-Length:"))
            {
                token2 = strtok(NULL, " ");
 ////               printf("%s \n", token2);
            }
        token = strtok(NULL, " ");
    } 
    
    split_response = strtok(response,"\r\n"); // split every line in the header/response
    while(split_response != NULL)
    {
 //    printf("%s\n",split_response); // prints every line in header
       split_response = strtok(NULL,"\r\n");

    }
    
    split_response = strtok(response, " "); // 
    if ((strcmp(split_response, "GET") != 0) && (strcmp(split_response, "PUT") != 0)) return 0;
    
    
    if (strcmp(split_response, "GET") == 0)
    {
        fname = strtok(NULL, " ");
        if (fname[0] == '/') fname++; // if the next argument's first character is a /
        if (strlen(fname) != 27) 
        {
            printf("\nERROR: File name must be exactly 27 characters  \n"); //print file name
            return 0;
        }
        //printf("%s\n",fname); //print file name
        
        while (fname[i])
        {
            if ((isalnum(fname[i]) == 0) && (fname[i]!= '-') && (fname[i]!= '_'))
            {    
                printf("Error: Filename should not include anything besides the alphabet, hyphens, or underscores");
                return 0;
            }
            i++;
        }        
    
        if(access(fname, F_OK) != 0)  //if file is not accesible
        {
            header(new_socket, 2); // give an error
            return 0;
        } 
        else if (access(fname, R_OK) != 0) // if file is not readable
        {
            header(new_socket, 1);
            return 0;
        } 
        else {
            header(new_socket, 0);    
            
            fd = open(fname, O_RDWR);
            while(read( fd, buf, 1) == 1) 
            {
                printf("%s", buf); 
                send(new_socket, buf, strlen(buf), 0);     
            }
            close(fd);


            	                  
            return 0;
        }
    }

   
        
    if (strcmp(split_response, "PUT") == 0)
    {
        fname = strtok(NULL, " ");
        if (fname[0] == '/') fname++;
    //    printf("%s\n",fname); //print file name
        if (strlen(fname) != 27) 
        {
            printf("\nERROR: File name must be exactly 27 characters  \n"); //print file name
            return 0;
        }
        //printf("%s\n",fname); //print file name
        
        while (fname[i])
        {
            if ((isalnum(fname[i]) == 0) && (fname[i]!= '-') && (fname[i]!= '_'))
            {    
                printf("Error: Filename should not include anything besides the alphabet, hyphens, or underscores");
                return 0;
            }
            i++;
        }        
    
        header(new_socket, 0);    
        fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC);
        recv(new_socket, buf, 50, 0);
        write( fd, buf, 50); 
        printf("%s", buf);    
        close(fd);
        
       
        return 0;
    }
    
    
return 0;

}
