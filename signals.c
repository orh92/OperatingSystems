#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

int arr[5], child = 0;

//sighandler function
void sighandler(int signum){
	
    printf("PID %d caught  \n", getpid()); //recursion to all proccessses
    if (child > 0)
        kill(arr[child - 1], SIGINT); //next proccess

    exit(0);
}



int main() {

    signal(SIGINT, sighandler);                  //signal
    int now=0;

    for (int i = 0; i < 5; i++)  {
        int childpid= fork();                       // child pid
        
	if (childpid > 0)	//father
           arr[i] = childpid;
        
        else  {                      //child                  	
            printf("PID %d is ready \n", getpid());
            break;
        }
        child++;
    }

    
    if (child != 4 && child != 5)   //if its not the last proccess or the father
        pause();
     
    if (child == 4)                 //sending msg
        kill(getpid(), SIGINT);
    

    if (child == 5) {               //the father is waiting for the son
 
       waitpid(arr[0], &now, 0);
       waitpid(arr[1], &now, 0);
       waitpid(arr[2], &now, 0);
       waitpid(arr[3], &now, 0);
       waitpid(arr[4], &now, 0);
    }
  
    for (int i = 0; i < child; i++){     //the end
        printf("pid %d proccess is dead \n", arr[i]);
}

    return 0;

}
