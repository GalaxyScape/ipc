#include <stdio.h> 
#include <unistd.h> 
#include <signal.h> 
int count = 0; 
  
void alrm_action(int signo) 
{ 
    printf("Write blocked after %d characters\n", count); 
    exit(0); 
} 
int main() 
{ 
    int p[2]; 
    char c = 'x'; 
  
    signal(SIGALRM, alrm_action); 
  
    if (pipe(p) == -1) 
        exit(1); 
  
    while (1) { 
        alarm(2); 
  
        //write a char until blocked
        write(p[1], &c, 1); 
  
        ++count; 
        alarm(0); 
    } 
} 
