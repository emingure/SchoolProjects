/*
environ - skeleton program displaying environment
usage:
environ
displays environment with each name, value pair on a separate line
*/
#include <stdio.h>
#include <stdlib.h>

extern char **environ; // environment array

int main(int argc, char **argv)
{
  char ** env = environ;
  while (*env) printf("%s\n",*env++); // step through environment
  exit(0);
}
