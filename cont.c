#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

static int child_fn() {
  printf("Clone internal PID: %ld\n", (long) getpid());
  pid_t child_pid = fork();
  if (child_pid) {
    printf("Clone fork child PID: %ld\n", (long) child_pid);
    //Mount namespace
    system("mount --make-rprivate -o remount /");
    system("mount proc /proc -t proc --make-private");
    //Up new network interface
    printf("\nGuest network namespace:\n");
    system("ifconfig veth1 10.1.1.2/24 up");
    system("ip link");
    printf("\n\n");
    //Loop
    system("dd if=/dev/zero of=image.fs count=10 bs=20M");
    //Using loop30 to write the filesystem
    system("losetup /dev/loop30 image.fs");
    system("mkfs.ext2 image.fs");
    system("mount -t ext2 /dev/loop30 /root/Container/Mnt --make-private");
    system("dd if=/dev/zero of=/root/Container/Mnt/check.txt count=1 bs=10M");
    printf("Files in the new mountspace /root/Container/Mnt:\n");
    system("ls /root/Container/Mnt");
    //Enter bash
    system("bash");
  }
  // Detaching loop device
  system("umount /dev/loop30");
  system("losetup --detach /dev/loop30");
 
  return EXIT_SUCCESS;
}

int main() {
  pid_t child_pid = clone(child_fn, child_stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD | CLONE_NEWNET | CLONE_NEWNS, NULL);
  printf("clone() = %ld\n", (long) child_pid);
  //Configure network interfaces
  char newnt[1024*sizeof(char)];
  sprintf(newnt, "ip link add name veth0 type veth peer name veth1 netns %ld", (long) child_pid);
  system(newnt);
  system("ifconfig veth0 10.1.1.1/24 up");
  printf("\nHost network namespace:\n");
  system("ip link");
  waitpid(child_pid, NULL, 0);
  //Check that check.txt is not in the host
  printf("Files on the local machine:\n");
  system("ls");
  return EXIT_SUCCESS;
}

