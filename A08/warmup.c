#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int child_status;
  pid_t pid;
  printf("%d] A0\n", getpid());
  pid = fork();
  if (pid){
    printf("%d] B0\n", getpid());
    pid = wait(&child_status);
    if (pid){
      pid = fork();
      if (pid){
	printf("%d] C0\n", getpid());
      }
      else{
	printf("%d] C1\n", getpid());
      }
    }
  }
  else{
    printf("%d] B1\n", getpid());
  }
 printf("%d] Bye\n", getpid()); 
 return 0;
}
