// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#include <errno.h>
#include <string.h> 
#include <iostream> 
#define PORT 8080
#define BUF_SIZE 65536
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include<sstream>

char* filename;
char* fname = NULL;
char* fnary0 = NULL;
char* fnary1 = NULL;
char* fnary2 = NULL;
char* fnary3 = NULL;
int counter = 1;
int logenb = 0;
int cacheenb = 0;
int page;
int ref_f1 = 0;
int ref_f2 = 0;
int ref_f3 = 0;
int ref_f4 = 0;
FILE *fp;
char buf1[BUF_SIZE] = {0};
char buf2[BUF_SIZE] = {0};
char buf3[BUF_SIZE] = {0};
char buf4[BUF_SIZE] = {0};
char logbuf[BUF_SIZE]={0};
using namespace std;


void header(int handler, int status) 
{
  char header[BUF_SIZE];
  int err;
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
  //if (err) {printf("fail to unlock_header = %d\n", err);}

}


void *connection(void *p){
    int valread = 0;
    char response[BUF_SIZE] = {0}; 
    int new_socket;
    char* token2;
    char* split_response;
    char* split_response1;
    int i=0;
    int j;
    int fd;
    int replace = 0;
    int filed[4],error;
    char *str = NULL;
    char buf[BUF_SIZE] = {0};
    char *pbuf = buf;
    ssize_t n;
    size_t len = 0;
    int m=0;
    int mxlen = 0;
    unsigned char strHex[BUF_SIZE*3];
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
    
 //if (error) {printf("fail to lock_conn = %d\n", error);}
     
    // Returns first token 
    char* token = strtok(response, "\r\n"); 

    printf("\n token=%s",token);
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
    printf("\n split_response1=%s",split_response);

    while(split_response != NULL)
    {
       printf("%s\n",split_response); // prints every line in header
       split_response = strtok(NULL,"\r\n");

    }
    
    split_response = strtok(response, " "); // 
    printf("\n split_response2=%s",split_response);
    
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
                printf("\n logbuf: %s",logbuf);
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
                printf("\n logbuf: %s",logbuf);
            }
//          return 0;
        } 
        
        else {
                header(*new_socket_thread, 0);
            
                //printf("%s",logbuf);
                if (cacheenb == 1)
                {
                    if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 1) && (ref_f4 == 1) && (strcmp(fnary0,fname)!=0) && (strcmp(fnary1,fname)!=0) &&(strcmp(fnary2,fname)!=0) && (strcmp(fnary3,fname)!=0) )
                    {
                        replace = counter % 5;
//                       printf("\nreplace:%d", replace); 
//                       printf("\ncounter:%d", counter); 
//                       printf("\nfname:%s", fname); 

                        if (replace == 0)
                        {
                            ref_f1 = 0;
                            strcpy(buf1, "");
                        }
                       if (replace == 1)
                       {
                            ref_f2 = 0;
                            strcpy(buf2, "");
                       }
                       if (replace == 2)
                       {
                            ref_f3 = 0;
                            strcpy(buf3, "");
                       }
                       if (replace == 3)
                       {
                            ref_f4 = 0;
                            strcpy(buf4, "");
                       }
//                       printf("\nref_f1:%d , ref_f2:%d , ref_f3:%d , ref_f4: %d", ref_f1, ref_f2, ref_f3, ref_f4); 
//                       printf("\nfnary0:%s , fnary1:%s , fnary2:%s , fnary3: %s", fnary0, fnary1, fnary2, fnary3); 
                    }
                }
                else
                {
                    if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 1) && (ref_f4 == 1))
                    {
                        replace = counter % 5;
//                        printf("\nreplace:%d", replace); 
//                        printf("\ncounter:%d", counter); 
//                        printf("\nfname:%s", fname); 

                        if (replace == 0)
                        {
                            ref_f1 = 0;
                            strcpy(buf1, "");
                        }
                       if (replace == 1)
                       {
                            ref_f2 = 0;
                            strcpy(buf2, "");
                       }
                       if (replace == 2)
                       {
                            ref_f3 = 0;
                            strcpy(buf3, "");
                       }
                       if (replace == 3)
                       {
                            ref_f4 = 0;
                            strcpy(buf4, "");
                       }
//                       printf("\nref_f1:%d , ref_f2:%d , ref_f3:%d , ref_f4: %d", ref_f1, ref_f2, ref_f3, ref_f4); 
//                       printf("\nfnary0:%s , fnary1:%s , fnary2:%s , fnary3: %s", fnary0, fnary1, fnary2, fnary3); 
                    }
                }

                if ((ref_f1 == 1) && (cacheenb == 1))
                {
                    if (strcmp(fnary0,fname)==0)
                    {
                        ref_f1 = 1;
//                      printf("\n FILE referenced: %s", fnary0);                 
//                      printf("\n BUF1r:%s", buf1);
                        send(*new_socket_thread, buf1, strlen(buf1), 0);
                        if (logenb==1)
                        {
                            sprintf(logbuf + strlen(logbuf), "GET %s length 0[was in chache]\n",fname);
                            sprintf(logbuf + strlen(logbuf), "========\n");                    
                        }
                        return(0);
                    }
                }

                if (ref_f1 == 0)
                {
                    counter++; 
                    fnary0 = strdup(fname);
                    filed[0] = open(fname, O_RDWR);
                    ref_f1 = 1;
                    while(read( filed[0], buf, 1) == 1) 
                    {              
                        printf("%s", buf); 
                        send(*new_socket_thread, buf, strlen(buf), 0);
                        strncat(buf1, buf,1);
                    }

//                    printf("BUF1:%s", buf1); 

                    if (logenb==1)
                    {
                        if (cacheenb == 1)
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0[was not in chache]\n",fname);
                        else
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0\n",fname);
                            
                        sprintf(logbuf + strlen(logbuf), "========\n");                    
                    }
                    
                    close(filed[0]);
                  return(0);
                }


                if ((ref_f1 == 1) && (ref_f2 == 1) && (cacheenb == 1))
                {
                    if (strcmp(fnary1,fname)==0)
                    {
                        ref_f2 = 1;
//                        printf("\n FILE referenced: %s", fnary1);                 
//                        printf("\n BUF2r:%s", buf2);
                        send(*new_socket_thread, buf2, strlen(buf2), 0);
                        if (logenb==1)
                        {
                            sprintf(logbuf + strlen(logbuf), "GET %s length 0[was in chache]\n",fname);
                            sprintf(logbuf + strlen(logbuf), "========\n");                    
                        }
                        return(0);
                    }
               }   

                if ((ref_f1 == 1) && (ref_f2 == 0))
               {
                    counter++; 
                    fnary1 = strdup(fname);
                    filed[1] = open(fname, O_RDWR);
                    ref_f2 = 1;
                    while(read( filed[1], buf, 1) == 1) 
                    {              
                        printf("%s", buf);
                        send(*new_socket_thread, buf, strlen(buf), 0);     
                        strncat(buf2, buf,1);
                    }
  //                  printf("\n BUF2:%s", buf2); 
                    if (logenb==1)
                    {
                        if (cacheenb == 1)
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0[was not in chache]\n",fname);
                        else
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0\n",fname);
                            
                        sprintf(logbuf + strlen(logbuf), "========\n");                    
                    }                 
                    close(filed[1]);
                   return(0);
                }


                if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 1) && (cacheenb == 1))
                {
                    if (strcmp(fnary2,fname)==0)
                    {
                        ref_f3 = 1;
//                        printf("\n FILE referenced: %s", fnary2);                 
//                        printf("\n BUF3r:%s", buf3);
                        send(*new_socket_thread, buf3, strlen(buf3), 0);
                        if (logenb==1)
                        {
                            sprintf(logbuf + strlen(logbuf), "GET %s length 0[was in chache]\n",fname);
                            sprintf(logbuf + strlen(logbuf), "========\n");                    
                        }                        
                        return(0);
                    }
                }    


                if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 0))
                {
                    counter++; 
                    fnary2 = strdup(fname);
                    filed[2] = open(fname, O_RDWR);
                    ref_f3 = 1;

                    while(read( filed[2], buf, 1) == 1) 
                    {              
                        printf("%s", buf); 
                        send(*new_socket_thread, buf, strlen(buf), 0);     
                        strncat(buf3, buf,1);
                    }
//                    printf("\n BUF3:%s", buf3);
                    if (logenb==1)
                    {
                        if (cacheenb == 1)
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0[was not in chache]\n",fname);
                        else
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0\n",fname);
                            
                        sprintf(logbuf + strlen(logbuf), "========\n");                    
                    }               
                    close(filed[2]);
                    return(0);
                }



                if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 1) && (ref_f4 == 1) && (cacheenb == 1))
                {
                    if (strcmp(fnary3,fname)==0)
                    {
                        ref_f4 = 1;
//                        printf("\n FILE referenced: %s", fnary3);                 
//                        printf("\n BUF4r:%s", buf4);
                        send(*new_socket_thread, buf4, strlen(buf4), 0);
                        if (logenb==1)
                        {
                            sprintf(logbuf + strlen(logbuf), "GET %s length 0[was in chache]\n",fname);
                            sprintf(logbuf + strlen(logbuf), "========\n");                    
                        }                        
                        return(0);
                    }
               }           


                if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 1) && (ref_f4 == 0))
                {
                    counter++; 
                    fnary3 = strdup(fname);
                    filed[3] = open(fname, O_RDWR);
                    ref_f4 = 1;
                    while(read( filed[3], buf, 1) == 1) 
                    {              
                        printf("%s", buf); 
                        send(*new_socket_thread, buf, strlen(buf), 0);     
                        strncat(buf4, buf,1);
                    }
//                    printf("\n BUF4:%s", buf4); 
                    close(filed[3]);
                    if (logenb==1)
                    {
                        if (cacheenb == 1)
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0[was not in chache]\n",fname);
                        else
                                sprintf(logbuf + strlen(logbuf), "GET %s length 0\n",fname);
                            
                        sprintf(logbuf + strlen(logbuf), "========\n");                    
                    }
//                    printf("\n FILE1 : %s", fnary0); 
//                    printf("\n FILE2 : %s", fnary1); 
//                    printf("\n FILE3 : %s", fnary2); 
//                    printf("\n FILE4 : %s", fnary3); 
//                    printf("\n Counter : %d", counter); 
                    return(0);
                }
            }
    }


        
    if (strcmp(split_response, "PUT") == 0)
    {
        fname = strtok(NULL, " ");
        if (fname[0] == '/') fname++;
//        printf("%s\n",fname); //print file name
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
        
//        printf("\n BEFORE HEADER");    
        header(*new_socket_thread, 0);
//        printf("\n AFTER HEADER ref_f1: %d, ref_f2: %d",ref_f1,ref_f2); 
        
        if (ref_f1 == 0)
        {
//            printf("\n in ref1");            
            counter++;
            fnary0 = strdup(fname);
            fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            ref_f1 = 1;
//            printf("\n FIRST LOOP");
            
            while((m = recv(*new_socket_thread, buf, BUF_SIZE,0)) > 0){
                mxlen += m;
//                printf("\n mxlen = %d , m = %d, buf = %s", mxlen, m, buf);
			    if(mxlen == m){
				    break;
			     }
            }
            buf[m+1] = '\0';
//            printf("\n BUF1:%s",buf);
            write( fd, buf, strlen(buf)+4); 
            /*set strH with nulls*/
            memset(strHex,0,sizeof(strHex));
            if (logenb==1)
            {
                /*converting str character into Hex and adding into strH*/
                for(i=0,j=0;i<strlen(buf);i++,j+=3)
                { 
                    sprintf((char*)strHex+j,"%02x ",buf[i]);
                }
                printf("%s", strHex);    
                sprintf(logbuf + strlen(logbuf), "PUT %s length %lu\n",fname,strlen(buf));
                sprintf(logbuf + strlen(logbuf), "%s[was not in cache]\n",strHex); //prints out ist of hex
                sprintf(logbuf + strlen(logbuf), "========\n");
            }
//            printf("\n BUF1r:%s", buf);
            strcpy(buf, "");
//            close(fd);
//            close(*new_socket_thread);
           return 0;
        }
        
//        printf("\n ref_f1:%d", ref_f1); 
//        printf("\n ref_f2:%d", ref_f2);    

        if ((ref_f1 == 1) && (ref_f2 == 0))
        {
//            printf("SECOND LOOP");
            counter++;
            fnary1 = strdup(fname);
            fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            ref_f2 = 1;
            while((m = recv(*new_socket_thread, buf, BUF_SIZE,0)) > 0){
                mxlen += m;
//                printf("\n mxlen = %d , m = %d, buf = %s", mxlen, m, buf);
			    if(mxlen == m){
				    break;
			     }
            }
            //m = recv(*new_socket_thread, buf, 65536, 0);
            buf[m+1] = '\0';
//            printf("\n BUF2:%s",buf);
            write( fd, buf, sizeof(buf)+1); 
            /*set strH with nulls*/
            memset(strHex,0,sizeof(strHex));
            if (logenb==1)
            {
                /*converting str character into Hex and adding into strH*/
                for(i=0,j=0;i<strlen(buf);i++,j+=3)
                { 
                    sprintf((char*)strHex+j,"%02x ",buf[i]);
                }
                printf("%s", strHex);    
                sprintf(logbuf + strlen(logbuf), "PUT %s length %lu\n",fname,strlen(buf));
                sprintf(logbuf + strlen(logbuf), "%s[was not in cache]\n",strHex); //prints out ist of hex
                sprintf(logbuf + strlen(logbuf), "========\n");
           }
//            printf("\n BUF2r:%s", buf);
            strcpy(buf, "");
//            close(fd);
//            close(*new_socket_thread);
            return 0;
        }

        if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 0))
        {
//            printf("THIRD LOOP");
            counter++;
            fnary2 = strdup(fname);
            fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            ref_f3 = 1;
            while((m = recv(*new_socket_thread, buf, BUF_SIZE,0)) > 0){
                mxlen += m;
//                printf("\n mxlen = %d , m = %d, buf = %s", mxlen, m, buf);
			    if(mxlen == m){
				    break;
			     }
            }
            //m = recv(*new_socket_thread, buf, 65536, 0);
            buf[m+1] = '\0';
//            printf("\n BUF3:%s",buf);
            write( fd, buf, sizeof(buf)+1); 
            /*set strH with nulls*/
            memset(strHex,0,sizeof(strHex));
            if (logenb==1)
            {
                /*converting str character into Hex and adding into strH*/
                for(i=0,j=0;i<strlen(buf);i++,j+=3)
                { 
                    sprintf((char*)strHex+j,"%02x ",buf[i]);
                }
                printf("%s", strHex);    
                sprintf(logbuf + strlen(logbuf), "PUT %s length %lu\n",fname,strlen(buf));
                sprintf(logbuf + strlen(logbuf), "%s[was not in cache]\n",strHex); //prints out ist of hex
                sprintf(logbuf + strlen(logbuf), "========\n");
            }
//            printf("\n BUF3r:%s", buf);    
            strcpy(buf, "");
//            close(fd);
//            close(*new_socket_thread);
            return 0;
        } 

        if ((ref_f1 == 1) && (ref_f2 == 1) && (ref_f3 == 1) && (ref_f4 == 0))
        {
 //           printf("FOURTH LOOP");
            counter++;
            fnary3 = strdup(fname);
            fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            ref_f4 = 1;
            while((m = recv(*new_socket_thread, buf, BUF_SIZE,0)) > 0){
                mxlen += m;
//                printf("\n mxlen = %d , m = %d, buf = %s", mxlen, m, buf);
			    if(mxlen == m){
				    break;
			     }
            }
            //m = recv(*new_socket_thread, buf, 65536, 0);
            buf[m+1] = '\0';
//            printf("\n BUF4:%s",buf);
            write( fd, buf, sizeof(buf)+1); 
            /*set strH with nulls*/
            memset(strHex,0,sizeof(strHex));
            if (logenb==1)
            {
                /*converting str character into Hex and adding into strH*/
                for(i=0,j=0;i<strlen(buf);i++,j+=3)
                { 
                    sprintf((char*)strHex+j,"%02x ",buf[i]);
                }
                printf("%s", strHex);    
                sprintf(logbuf + strlen(logbuf), "PUT %s length %lu\n",fname,strlen(buf));
                sprintf(logbuf + strlen(logbuf), "%s[was not in cache]\n",strHex); //prints out ist of hex
                sprintf(logbuf + strlen(logbuf), "========\n");
            }
//          printf("\n BUF4r:%s", buf); 
            strcpy(buf, "");
//            close(fd);
//            close(*new_socket_thread);
            return 0;
        } 
    }
    
    if (logenb==1)
    {
        fprintf(fp,"%s",logbuf);
        fclose(fp);
    }   //if (error) {printf("fail to unlock_conn = %d\n", error);}
    close(fd);
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
    int new_socket[4];    
    int opt, err = 0;
    char* nf;
    int numfile = 1;
    char* st;
    static char usage[] ="Usage: httpserver -N 8 -l my_logfile.txt -c localhost 8080";
 
    if (argc == 1){
        printf("\n Usage: httpserver -N 8 -l my_logfile.txt -c localhost 8080");
        return EXIT_FAILURE;
    }
    for (opt = 0; opt<argc; ++opt) {
        if (strcmp((char *) argv[opt], "-l")==0){
            logenb = 1;
            filename = (char *) argv[opt + 1];
        }
        if (strcmp((char *) argv[opt], "-c")==0){
            cacheenb = 1;
        }
        if (strcmp((char *) argv[opt], "-N")==0){
            nf = (char *) argv[opt + 1];
            numfile = atoi(nf);
        }
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
    
    //looping through all the threads
    //new_socket = 0; // allows for multiple sockets
    
    for (int i=0; i < numfile ; i++)
    {   
        if (listen(server_fd, numfile) < 0) 
	   { 
		  perror("listen"); 
		  exit(EXIT_FAILURE); 
	   } 
        addrlen = sizeof(serverAddr);
        puts("Waiting for connection ...");
    

        if ((new_socket[i] = accept(server_fd, (struct sockaddr *)&serverAddr, 
					(socklen_t*)&addrlen))<0) 
	   { 
		  perror("accept"); 
		  exit(EXIT_FAILURE); 
	   } 
        connection(&new_socket[i]);
        close(new_socket[i]);
    }
 
    if (logenb==1)
    {    
        fprintf(fp,"%s",logbuf);
        fclose(fp);
    }
    
    close(server_fd);
    return 0;
}


    
    