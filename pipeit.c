#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
  int status;
  int READ = 0;
  int WRITE = 1;
  int pipe1[2] = {READ, WRITE}; // child to parent
  int pipe2[2] = {READ, WRITE}; // parent to child
  pipe(pipe1);
  pipe(pipe2);
  
  int child = fork();
  if (!child) { // child process
    close(pipe1[READ]);
    close(pipe2[WRITE]);
    
    int i;
    read(pipe2[READ], &i, sizeof(i));
    
    printf("Child subtracting 1 from int %d...\n", i);
    i--;  // quick maths
    
    printf("Child sending int %d back to parent...\n", i);
    write(pipe1[WRITE], &i, sizeof(i));
    
    close(pipe1[WRITE]);
    close(pipe2[READ]);
  }
  else {  // parent process
    close(pipe1[WRITE]);
    close(pipe2[READ]);
    
    int i = getpid();
    printf("Parent sending int %d to child...\n", i);
    write(pipe2[WRITE], &i, sizeof(i));
    
    wait(&status);
    
    read(pipe1[READ], &i, sizeof(i));
    printf("Parent received int %d.\n", i);
    
    close(pipe1[READ]);
    close(pipe2[WRITE]);
  }
  
  return 0;
}
