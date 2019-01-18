/*
Muhammed Emin Gure
usage:
./1c
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define
SEPARATORS).
can use redirected input
if the first token is a recognized internal command, then that
command is executed. otherwise the tokens are printed on the
display.
internal commands:
echo
pause
help
clr - clears the screen
dir <directory> - equals with "ls -al <directory>" command
environ - show the environment variables
quit - exits from the program (aliases: exit)
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token sparators

int BACKGROUND = 0;   // flag for background jobs
int IOCHECK = 1;      // flag for I/O redirection

int environ_cmd(){
  extern char **environ; // environment array
  char ** env = environ;
  while (*env) printf("%s\n",*env++); // step through environment
}

/* parses the command to arrange I/O redirection and set environments as stdin, stdout, stderr */
void parseIO(char **line){
  for(int i=0; line[i]!=NULL; i++){
    if(!strcmp(line[i],"<")){           // input file opens for reading
      if(IOCHECK == 0){
        // if flag is 0, do nothing
      }else if(access(line[i+1],F_OK | R_OK) == 0){   // checks if input file exists and readable
        freopen(line[i+1], "r", stdin);
      }else{
        fprintf(stderr, "%s: %s", line[i+1], "No such file or not allowed to read!\n");
        exit(0);
      }
    }else if(!strcmp(line[i],">")){     // output file is created if it does not exist and truncated if it does.
      if(IOCHECK == 0){
        // if flag is 0, do nothing
      }else if(access(line[i+1],W_OK) == 0 || access(line[i+1],F_OK) == -1){    // checks if output file is writable if exists or doesn't exists
        freopen(line[i+1], "w", stdout);
      }else{
        fprintf(stderr, "%s: %s", line[i+1], "You are not allowed to write!\n");
        exit(0);
      }
    }else if(!strcmp(line[i],">>")){    // output file is created if it does not exist and appended to if it does
      if(IOCHECK == 0){
        // if flag is 0, do nothing
      }else if(access(line[i+1],W_OK) == 0 || access(line[i+1],F_OK) == -1){    // checks if output file is writable if exists or doesn't exists
        freopen(line[i+1], "a+", stdout);
      }else{
        fprintf(stderr, "%s: %s", line[i+1], "You are not allowed to read!\n");
        exit(0);
      }
    }else{                              // if no IO redirection continue to search without removing any string
      continue;
    }
    /* remove parsed strings to prevent confusion */
    line[i] = NULL;
    line[++i] = NULL;
  }
}

void  execute(char **args)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          parseIO(args);
          if (execvp(args[0], args) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed , %s\n",*args);  /* print if error exits */
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          if(BACKGROUND == 0) {                     /* checks if the command will run at background */
            waitpid(pid, &status, WUNTRACED);       /* wait for completion  */
          }
     }

}

int main (int argc, char ** argv)
{
  char buf[MAX_BUFFER]; // line buffer
  char * args[MAX_ARGS]; // pointers to arg strings
  char ** arg; // working pointer thru args
  char * prompt = "==>" ; // shell prompt
  char **lastToken; // last token of input
  lastToken = (char **) malloc(1);  // allocate a small space to bypass the bug that is the case of first command of only one string

  /* reads batchfile if specified and redirects input file to stdin */
  if(argc>1){
    freopen(argv[1], "r", stdin);
  }

  /* keep reading input until "quit" command or eof of redirected input */
  while (!feof(stdin)) {
    /* sets default flag values */
    BACKGROUND = 0;
    IOCHECK = 1;
    /* get command line from input */
    if (argc < 2) fputs (prompt, stdout); // write prompt if no batchfile exists
    if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
      /* tokenize the input into args array */
      arg = args;
      *arg++ = strtok(buf,SEPARATORS); // tokenize input
      while ((*arg = strtok(NULL,SEPARATORS))) {
        lastToken = arg++;                          // sets the last token to check for background jobs
      };

      if(*lastToken != NULL && !strcmp(*lastToken, "&")) {    // checks if the command will run at background
        *lastToken = NULL;                                    // removes the '&' string
        BACKGROUND = 1;                                       // sets the flag to indicate that the command will run at background
      }

      if (args[0]) { // if there's anything there

        if(!strcmp(args[0], "echo")) {   // checks for "echo" command to set the flag
          IOCHECK = 0;
        }

        /* check for internal/external command */
        if (!strcmp(args[0],"pause")) {         // "pause" internal command
          printf("Press Enter to continue...");
          system("stty -echo");                 // turns off the echo of keyboard input
          while( getchar() != '\n' );           // waits until "Enter" pressed
          system("stty echo | echo ");          // turns on the echo of keyboard input and goes to new line
          continue;
        }
        if (!strcmp(args[0],"help")) {          // "help" internal command
          /* creates a char array to send to 'execute' function as parameter */
          char *cmd[3];
          cmd[0] = "man";
          cmd[1] = "-l";
          cmd[2] = "readme";
          execute(cmd);
          continue;
        }
        if (!strcmp(args[0],"clr")) { // "clr" internal command
          strcpy(args[0],"clear");    // change to use as parameter of execute() function
          execute(args);
          continue;
        }
        if (!strcmp(args[0],"dir")) {  // dir <directory> command
          /* Create a command to use as parameter in order as project description */
          char * cmd[3];
          cmd[0] = "ls";
          cmd[1] = "-al";
          cmd[2] =  args[1]; // concetenate the indicated directory to the cmd string
          execute(cmd);
          continue;
        }
        if (!strcmp(args[0],"environ")) { // "environ" command
          environ_cmd();
          continue;
        }
        if (!strcmp(args[0],"cd")) { // "cd" command
          char cwd[1024];                   // to store current directory
          getcwd(cwd, sizeof(cwd));         // gets the current directory
          if(chdir(args[1]) == 0){          // if directory changes successfully
            setenv("OLDPWD",cwd,1);         // set environment for old pwd
            getcwd(cwd, sizeof(cwd));
            setenv("PWD",cwd,1);
          }
          continue;
        }
        if (!strcmp(args[0],"quit") || !strcmp(args[0],"exit")) // "quit" command
          break; // break out of 'while' loop

        /* else pass command to 'execute' function */
        arg = args;
        execute(args);
      }
    }
  }
  return 0;
}
