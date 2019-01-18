/*
strtokeg - skeleton shell using strtok to parse command line
Muhammed Emin Gure
usage:
./1a
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define
SEPARATORS).
can use redirected input
if the first token is a recognized internal command, then that
command is executed. otherwise the tokens are printed on the
display.
internal commands:
clr - clears the screen
dir <directory> - equals with "ls -al <directory>" command
environ - show the environment variables
quit - exits from the program
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token sparators

int environ_cmd(){
  extern char **environ; // environment array
  char ** env = environ;
  while (*env) printf("%s\n",*env++); // step through environment
}

int main (int argc, char ** argv)
{
  char buf[MAX_BUFFER]; // line buffer
  char * args[MAX_ARGS]; // pointers to arg strings
  char ** arg; // working pointer thru args
  char * prompt = "==>" ; // shell prompt
  /* keep reading input until "quit" command or eof of redirected input */
  while (!feof(stdin)) {
    /* get command line from input */
    fputs (prompt, stdout); // write prompt
    if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
      /* tokenize the input into args array */
      arg = args;
      *arg++ = strtok(buf,SEPARATORS); // tokenize input
      while ((*arg++ = strtok(NULL,SEPARATORS)));
        // last entry will be NULL
      if (args[0]) { // if there's anything there
        /* check for internal/external command */
        if (!strcmp(args[0],"clr")) { // "clr" internal command
          system("clear");
          continue;
        }
        if (!strcmp(args[0],"dir")) {  // dir <directory> command
          char *cmd;
          if(args[1]){  // if there is a directory indicated
            cmd = malloc(7 + strlen(args[1]));  // allocate memory
            strcpy(cmd, "ls -al "); // copy the beginning of command to the cmd string
            strcat(cmd, args[1]); // concetenate the indicated directory to the cmd string
          }else{
            cmd = strdup("ls -al"); // assign default command
          }
          system(cmd);
          continue;
        }
        if (!strcmp(args[0],"environ")) { // "environ" command
          environ_cmd();
          continue;
        }
        if (!strcmp(args[0],"quit")) // "quit" command
          break; // break out of 'while' loop

        /* else pass command onto OS (or in this instance, print them out) */
        arg = args;

        char *command = strdup(*arg++); // duplicate the first word of command by newly allocating command string
        while (*arg) strcat(strcat(command," "),*arg++);  // continue to concetenation of command
        system(command);
      }
    }
  }
  return 0;
}
