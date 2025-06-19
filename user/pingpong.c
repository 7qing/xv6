#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int parent_to_child[2]; // Pipe for parent to child communication
  int child_to_parent[2]; // Pipe for child to parent communication
  char recv_buf[5];       // Buffer to store received data

  pipe(parent_to_child);
  pipe(child_to_parent);

  int pid = fork();

  if (pid > 0) {
    // Parent process
    close(parent_to_child[0]);
    close(child_to_parent[1]);

    write(parent_to_child[1], "ping", 4);

    read(child_to_parent[0], recv_buf, 4);
    recv_buf[4] = '\0';
    printf("%d: received %s\n", getpid(), recv_buf);

    close(parent_to_child[1]);
    close(child_to_parent[0]);

    wait(0);
    exit(0); // Exit parent process
  } else {
    // Child process
    close(parent_to_child[1]);
    close(child_to_parent[0]);

    read(parent_to_child[0], recv_buf, 4);
    recv_buf[4] = '\0'; // Null-terminate the string
    printf("%d: received %s\n", getpid(), recv_buf);

    write(child_to_parent[1], "pong", 4);

    close(parent_to_child[0]);
    close(child_to_parent[1]);

    exit(0); // Exit child process
  }
}