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
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include<sstream>

pthread_mutex_t Queue_mutex ;//= PTHREAD_MUTEX_INITIALIZER; // initializing mutex 
pthread_mutex_t Queue_mutex1 ;//= PTHREAD_MUTEX_INITIALIZER; // initializing mutex 
pthread_mutex_t Queue_mutex2 ;//= PTHREAD_MUTEX_INITIALIZER; // initializing mutex 
char* filename;
int logenb = 0;
using namespace std;


void header(int handler, int status) 
{
  char header[BUF_SIZE];
  int err;
  err = pthread_mutex_lock(&Queue_mutex2);
  //if (err) {printf("fail to lock_header = %d\n", err);}

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
  err = pthread_mutex_unlock(&Queue_mutex2);
  //if (err) {printf("fail to unlock_header = %d\n", err);}

}


void *connection(void *p){
    int valread = 0;
    char response[BUF_SIZE] = {0}; 
    int new_socket;
    char* token2;
    char* split_response;
    char* split_response1;
    char* fname;
    int i=0;
    int j;
    int fd,error;
    char buf[BUF_SIZE] = {0};
    unsigned char strHex[BUF_SIZE*3];
    char logbuf[BUF_SIZE]={0};
    FILE *fp;
    int num;
  //  pthread_mutex_t Queue_mutex = PTHREAD_MUTEX_INITIALIZER; // initializing mutex 
   
    int *new_socket_thread = (int *)p;
    if (logenb==1)
        fp = fopen(filename,"a");
    valread = recv( *new_socket_thread , response, sizeof(response),0); 
    if (logenb==1)
        printf("filename = \"%s\"\n", filename); 
 //   fprintf(fp,"%s",response);
    printf("\n%s\n", response);
    
 error =pthread_mutex_lock(&Queue_mutex);
 //if (error) {printf("fail to lock_conn = %d\n", error);}
     
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
       printf("%s\n",split_response); // prints every line in header
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

        printf("%s\n",fname); //print file name
        
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
            header(*new_socket_thread, 2); // give an error
            if (logenb==1)
            {              
                sprintf(logbuf + strlen(logbuf), "FAIL: GET %s HTTP/1.1 --- response 400\n",fname);
                sprintf(logbuf + strlen(logbuf), "========\n");
            }
  //        return 0;
        } 
        else if (access(fname, R_OK) != 0) // if file is not readable
        {
            header(*new_socket_thread, 1);
            if (logenb==1)
            {
                sprintf(logbuf + strlen(logbuf), "FAIL: GET %s HTTP/1.1 --- response 400\n",fname);
                sprintf(logbuf + strlen(logbuf), "========\n");
            }
//          return 0;
        } 
        else {
            header(*new_socket_thread, 0);
            if (logenb==1)
            {
                sprintf(logbuf + strlen(logbuf), "GET %s length 0\n",fname);
                sprintf(logbuf + strlen(logbuf), "========\n");
            }
            //printf("%s",logbuf);


            
            fd = open(fname, O_RDWR);
            while(read( fd, buf, 1) == 1) 
            {
                
                printf("%s", buf); 
                send(*new_socket_thread, buf, strlen(buf), 0);     
            }
            close(fd);


            	                  
            //return 0;
        }
        if (logenb==1)
        {    
            fprintf(fp,"%s",logbuf);
            fclose(fp);
        }
    }

   
        
    if (strcmp(split_response, "PUT") == 0)
    {
        fname = strtok(NULL, " ");
        if (fname[0] == '/') fname++;
        printf("%s\n",fname); //print file name
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
    
        header(*new_socket_thread, 0);    
        fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC);
        recv(*new_socket_thread, buf, 50, 0);


        
            /*set strH with nulls*/
        memset(strHex,0,sizeof(strHex));
     
        if (logenb==1)
        {
          /*converting str character into Hex and adding into strH*/
            for(i=0,j=0;i<strlen(buf);i++,j+=3)
            { 
                sprintf((char*)strHex+j,"%02x ",buf[i]);
           //   if ((i+1)%20==0)
           //        {
           //            sprintf((char*)strHex+j,"'\n'");
           //        }
            }


            printf("%s", strHex);    


            sprintf(logbuf + strlen(logbuf), "PUT %s length %lu\n",fname,strlen(buf));
            sprintf(logbuf + strlen(logbuf), "%s\n",strHex); //prints out ist of hex
            sprintf(logbuf + strlen(logbuf), "========\n");
        
        }
        write( fd, buf, strlen(buf)); 
        printf("%s", buf);    
        close(fd);
   
        //return 0;
    }
    
    error = pthread_mutex_unlock(&Queue_mutex);        
    //if (error) {printf("fail to unlock_conn = %d\n", error);}
    if (logenb==1)
    {
        fprintf(fp,"%s",logbuf);
        fclose(fp);
    }
    
return 0;

}









int main(int argc, char const *argv[]) 
{ 
  //  char *filename;
	int server_fd; 
	struct sockaddr_in serverAddr; 
	//int opt = 1; 
	int addrlen = sizeof(serverAddr); 
    char* len;
    int error;
    int file, length;
    int new_socket[100];    
    int opt, err = 0;
    char* max_threads;
    int MX_threads = 4; // default number of threads if none is provided
    int TH_count = 0;
    pthread_t Threads[100]; // initializing threeads
    char* st;
    static char usage[] ="Usage: httpserver -N 8 -l my_logfile.txt localhost 8080";
   //added the const to get rid of error message
	//char const *hello = "Hello from server"; 
	pthread_mutex_init( &Queue_mutex1, NULL); //initializing mutex
    if (argc == 1){
        printf("\n Usage: httpserver -N 8 -l my_logfile.txt localhost 8080");
        return EXIT_FAILURE;
    }
    for (opt = 0; opt<argc; ++opt) {
        if (strcmp((char *) argv[opt], "-N")==0){
            max_threads = (char *) argv[opt + 1];
        }

        if (strcmp((char *) argv[opt], "-l")==0){
            logenb = 1;
            filename = (char *) argv[opt + 1];
        }
    }
    
	/* see what we have */
  MX_threads = atoi(max_threads);
            //    printf("max_threads = %d\n", MX_threads);     
            //    printf("filename = \"%s\"\n", filename);        
   
    
    
  /*  for (i=0; i<MX_threads; i++)
    {
        client_sockets[i] = 0; 
    }
    
  */  
    
    
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
    
    //looping through all the threads
for (TH_count = 0; TH_count < MX_threads; TH_count++) { 
    new_socket[TH_count] = 0; // allows for multiple sockets
    
  error = pthread_mutex_lock(&Queue_mutex1);
if (error) {printf("fail to lock_main = %d\n", error);}
	if (listen(server_fd, 4) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
    addrlen = sizeof(serverAddr);
    puts("Waiting for connection ...");
    
	if ((new_socket[TH_count] = accept(server_fd, (struct sockaddr *)&serverAddr, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
    pthread_create(&Threads[TH_count], NULL, connection, &new_socket[TH_count]); // goes to connection to execute
    pthread_join(Threads[TH_count], NULL);
    error = pthread_mutex_unlock(&Queue_mutex1);
    if (error){printf("fail to unlock_main = %d\n", error);}


    }
    
    close(server_fd);
    return 0;
}


    
    