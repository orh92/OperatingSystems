
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "md5.h"

#define MSGSIZE 20

char dad_message[MSGSIZE]; //message
int p1[2], p2[2], pid, nbytes; //2 pipes, for dad write son read, and son write dad read

void sighandler(int signum)	//signal
{
    if (signum == SIGINT){
        if (strlen(dad_message) == 32)
        {
            printf("%s", dad_message);
            kill(0, SIGKILL);
        }
    }
}

int main()
{ 
    signal(SIGINT, sighandler);			//signal
	
    if (pipe(p1) < 0 || pipe(p2) < 0)		//eror
    {
        exit(1);
    }

	
    if ((pid = fork()) > 0)	//father
    {
	close(p1[0]);	//close unnececery pipes
        close(p2[1]);
        fgets(dad_message, MSGSIZE, stdin);      //enter msg
        write(p1[1], dad_message, MSGSIZE); //writing to the son

        //  read() // to do:
        nbytes = read(p2[0], dad_message, 32); //need to check nbytes
        printf("father see:  %s \n", dad_message);
    }
 
    else		   //son
    {
        close(p1[1]);	//close unnececery pipes
        close(p2[0]);
        nbytes = read(p1[0], dad_message, MSGSIZE);	//need to check nbytes
        printf("father msg:  %s \n", dad_message);
        printf("Hashed message:  %s\n", md5(dad_message).c_str());
        if (nbytes > 0)
        {
            char *hash_msg = (char *)malloc(32);	//hashing the msg
            strcpy(hash_msg, md5(dad_message).c_str());	
            int kill(pid_t getppid(), int sig);
            write(p2[1], hash_msg, MSGSIZE); //writing to the father
        }
    }

    return 0;
}
