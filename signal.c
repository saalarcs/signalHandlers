/*------------------------------------------------------------------------
# Signal Processing
#
# Name: signal.c
#
# Written By: Saalar Faisal - 2021-03-20 
#
# Purpose: Create 3 signal handler:
#			1. SIGINT (CTRL-C) - INCREMENT A COUNTER (MAX OF 5)
#				AND OUTPUT COUNT.
#			2. SIGQUIT (CTRL-\) - CREATE A FORK AND TERMINATE CHILD.
#				THEN, SEND A MESSAGE, AND SIGNAL SIGUSR1
#			3. SIGUSR1 - PRINT MESSAGE OF PROGRAM TERMINATION.
#
# To compile: gcc -o signal signal.c
#
#-------------------------------------------------------------------------*/

#include <stdlib.h> // STD LIB - standard library
#include <stdio.h> // IO LIB - used for input/output
#include <signal.h> //SIGNAL LIB - used for signal
#include <unistd.h> // FORK LIB - used for pause

// COUNTERS
int max = 5; 
int count = 0; 

// CHILD PROCESSES
int myChild; 

// THREE SIGNAL HANDLER FUNCTION HEADERS
void sigInt(int signo);
void quitProgram(int signo);
void userSignal(int signo);


int main() {
	// loop counter
	int done = 0;
  
    // SIGINT 
    if(signal(SIGINT, sigInt) == SIG_ERR)
		printf( "Cannot catch '^C' SIGINT\n");
	
	// SIGQUIT
	if(signal(SIGQUIT, quitProgram) == SIG_ERR)
		printf( "Cannot catch '^\' SIGQUIT\n");
	
	// SIGUSR1 
	if(signal(SIGUSR1, userSignal) == SIG_ERR)
		printf( "Cannot catch SIGUSR1\n");

    // Loop Until Termination
    while (!done) {
      printf(" Waiting for another signal ... \n");	
	  
	  //pause until signal hander has processed signal
      pause(); 
	  
    } 
	return 0;
}

// SIGINT
void sigInt(int signo){

  // handle reset problem (optional)
  signal(SIGINT, sigInt);
  
  // increase count on each call
  count++;

  // message if we reached the max
  if(count >= max){
    printf(" You have reached the Max.\n");

	// diable function
	// next time its called we will exit
	// using default signal handler
    signal(SIGINT, SIG_DFL); 
  }
  else {
    printf(" You have pressed ctrl-c %d time.\n", count);
  }
}

// SIGQUIT
void quitProgram(int signo){
	
	// handle reset problem (optional)
	signal(SIGQUIT, quitProgram);
	
	// fork the processes
	myChild = fork();
	
	// if successful
	if(myChild == 0)
	{
		// send message
		printf(" I am the child and I am sending a signal.\n");	
		// kill the child process and signal SIGUSR1
		kill(getppid(), SIGUSR1); 
		// exit without error
		exit(0);
	}
	
	// error message
	else if (myChild < 0) 
	{
		printf("Error forking the process for the child\n");
		// return with error
        exit(1);
	}
	
	else {
		// wait for child to finish
		pause();
	}

}

// SIGUSR1
void userSignal(int signo){	
  // handle reset problem (optional)
  signal(SIGUSR1, userSignal);
  // get the total
  printf("You pressed ctrl-c a total of %d times. \n", count);
  // termination message
  printf("Child sent a signal. The program will now terminate.\n");
  exit(0); 
}