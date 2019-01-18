/*
Project2 for C S 3113 Fall 2017
Author: Muhammed Emin Gure
This program basically simulates four memory allocation algorithms(best fit, first fit, next fit, buddy system) that is used widely. 
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define MAX_BUFFER 1024 // max line buffer
#define MAX_ARGS 64 // max # args
#define SEPARATORS " \t\n" // token sparators

unsigned long N;
char *processes[16];
int process_count = 0;
char* algo;
unsigned long cursor = 0;

struct node {
  unsigned long size,start;
  int isLeaf;
  char **process;
  struct node *parent, *left, *right;
};
struct node* newNode(struct node* parent, unsigned long size, unsigned long start) {
  struct node* cursor = (struct node*)malloc(sizeof(struct node));

  cursor->size = size;
  cursor->start = start;
  cursor->isLeaf = 1;
  cursor->process = NULL;
  cursor->parent = parent;
  cursor->left = NULL;
  cursor->right = NULL;
  return(cursor);
}
void assignNode(struct node* cursor, char* arr[], char* process, unsigned long byte) {
  processes[process_count] = malloc(16);
  strcpy(processes[process_count],process);
  cursor->process = &processes[process_count];
  for(unsigned long i=cursor->start; i < cursor->start+cursor->size; i++) {
    arr[i] = processes[process_count];
  }
  process_count++;
}
struct node *root;


void first_fit(char* arr[], char process[16], unsigned long byte);
void best_fit(char* arr[], unsigned long **aval_list, char* process, unsigned long byte);
void next_fit(char* arr[], char* process, unsigned long byte);
int buddy(char* arr[], struct node* cursor, char* process, unsigned long byte, int U);
int buddy_release(char* arr[], struct node* cursor, char* process);

void request(char* arr[], char* process, unsigned long byte);
void release(char* arr[], char* process);
int list_avaliable(char* arr[], unsigned long **list);
int buddy_list_avaliable(char* arr[], struct node* cursor, unsigned long **list, int index);
void list_assigned(char* arr[]);
void find(char* arr[], char* process, int type);

void construct_array();




int main (int argc, char **argv)
{

  if (argc < 4) {
    printf("Missing parameter\n");
    exit(0);
  }

  N = strtol(argv[2], NULL, 10);
  if ( (N < 16) || (N > 1048576) ) {
    printf("%lu is not in between 2^4 and 2^20\n", N);
  }

  algo = argv[1];
  root = newNode(NULL,N,0);

  char* arr[N];

  construct_array(arr);

  unsigned long (**aval_list) = malloc(sizeof(unsigned long) * N);

  for(unsigned long i =0; i < N; i++) {
    aval_list[i] = malloc(sizeof(unsigned long) * 2);
    aval_list[i][0] = 0;
  }

  char line[MAX_BUFFER];
  FILE *fptr;

  if ((fptr = fopen(argv[3], "r")) == NULL)
  {
      printf("Error! opening file");
      exit(1);
  }




  while(fgets(line, sizeof(line), fptr)){

    char* s[3];
    char* token = strtok(line, SEPARATORS);
    for(int i=0;i<3;i++) {
      if(token != NULL) {
        s[i] = malloc(strlen(token)+1);
        strcpy(s[i],token);
        token = strtok(NULL, SEPARATORS);
      }
    }

    if( !strcmp(s[0], "REQUEST") ) {
      char process1[16];
      strcpy(process1,s[1]);
      unsigned long size = strtoul(s[2],NULL,10);

      if ( !strcmp(algo,"FIRSTFIT") ) {
        first_fit(arr,process1,size);
      }else if ( !strcmp(algo,"BESTFIT") ) {
        best_fit(arr,aval_list,process1,size);
      }else if ( !strcmp(algo,"NEXTFIT") ) {
        next_fit(arr,process1,size);
      }else if ( !strcmp(algo,"BUDDY") ) {
        int U = ceil(log(N)/log(2));
        if (buddy(arr,root,process1,size,U) == 0) {
          printf("FAIL REQUEST %s %lu\n",process1,size );
        }
      }
    } else if( !strcmp(s[0], "RELEASE") ) {
      char* process = s[1];
      if ( !strcmp(algo,"BUDDY") ) {
        if (buddy_release(arr,root,process) == 0) {
          printf("FAIL RELEASE %s\n",process );
        }
      } else {
        release(arr,process);
      }
    } else if( !strcmp(s[0], "LIST") ) {
      if( !strcmp(s[1], "AVAILABLE") ) {
        if ( !strcmp(algo,"BUDDY") ) {
          buddy_list_avaliable(arr,root,aval_list,0);
        } else {
          list_avaliable(arr,aval_list);
        }
        int index = list_avaliable(arr,aval_list);
        for(int i = 0; i < index; i++) {
          printf("(%lu,%lu) ",aval_list[i][0],aval_list[i][1]);
        }
        if (index == 0) {
          printf("FULL");
        }
        printf("\n");
      } else if( !strcmp(s[1], "ASSIGNED") ) {
        list_assigned(arr);
      }

    } else if( !strcmp(s[0], "FIND") ) {
      char process1[16];
      strcpy(process1,s[1]);
      find(arr,process1,1);
    }

  }


  free(aval_list);
  return 0;
}

void construct_array(char* arr[]){
  for(unsigned long i=0; i < N; i++){
    arr[i] = NULL;
  }
}


int buddy(char* arr[], struct node* cursor, char* process, unsigned long byte, int U) {
  int k = ceil(log(byte)/log(2));
  unsigned long need = pow(2,k);
  if(k < U && cursor->process == NULL) {
    if (cursor->left == NULL ) {
      cursor->left = newNode(cursor,cursor->size/2,cursor->start);
      cursor->right = newNode(cursor,cursor->size/2,(cursor->start+cursor->size/2));
      cursor->isLeaf = 0;
    }
    if (buddy(arr,cursor->left,process,byte,U-1) == 1) {
      return 1;
    }
    if (buddy(arr,cursor->right,process,byte,U-1) == 1) {
      return 1;
    }

  }
  if (k == U) {
    if (cursor->process == NULL && cursor->isLeaf == 1) {
      assignNode(cursor, arr, process, byte);
      printf("ALLOCATED %s %lu %lu\n",process,cursor->size,cursor->start );
      return 1;
    }
  }
  return 0;
}

void concat(struct node* cursor) {
  if (cursor->left->process == NULL && cursor->right->process == NULL && cursor->left->isLeaf == 1 && cursor->right->isLeaf == 1 ) {
    cursor->left = NULL;
    cursor->right = NULL;
  }
}

int buddy_release(char* arr[], struct node* cursor, char* process) {
  if (cursor == NULL) {
    return 0;
  }
  if( cursor->process != NULL && !strcmp(process, *cursor->process) ) {
    cursor->process = NULL;
    for(unsigned long i=cursor->start; i<cursor->start+cursor->size; i++) {
      arr[i] = NULL;
    }
    for(int i=0; i<process_count; i++) {
      if( processes[i] != NULL && !strcmp(processes[i],process) ){
        processes[i] = NULL;
      }
    }
    printf("FREE %s %lu %lu\n",process,cursor->size,cursor->start );
    concat(cursor->parent);
    return 1;
  }
  if (buddy_release(arr,cursor->left,process) == 1) {
    return 1;
  }
  if (buddy_release(arr,cursor->right,process) == 1) {
    return 1;
  }
  return 0;
}



void first_fit(char* arr[], char process[16], unsigned long byte) {
  processes[process_count] = malloc(16);
  strcpy(processes[process_count],process);
  int found = 0;
  unsigned long start = -1;
  unsigned long aval_byte = 0;
  unsigned long end = -1;
  for(unsigned long i=0; i < N; i++){
    if (arr[i] == NULL && i != N-1) {
      if (found ==0) {
        start = i;
        found = 1;
      }
      aval_byte++;
    } else {
      if (aval_byte >= byte) {
        end = start + byte;
        break;
      } else {
        start = -1;
        found = 0;
      }
    }
  }
  if (found == 1) {
    for(unsigned long i = start; i < end; i++) {
      arr[i] = malloc(strlen(processes[process_count])+1);
      strcpy(arr[i],processes[process_count]);
    }
    process_count++;
    cursor = end;
    printf("ALLOCATED %s %lu\n",process,start );
  }else {
    printf("FAIL REQUEST %s %lu\n",process,byte );
  }
}

void best_fit(char* arr[], unsigned long **aval_list, char* process, unsigned long byte){

  list_avaliable(arr,aval_list);

  processes[process_count] = malloc(16);
  strcpy(processes[process_count],process);
  unsigned long min = N+1;
  unsigned long min_index = -1;
  for(unsigned long i=0; i < N; i++) {
    if (aval_list[i] == NULL) {
      break;
    }
    if (aval_list[i][0] >= byte && aval_list[i][0] < min) {
      min = aval_list[i][0];
      min_index = i;
    }
  }

  if (min_index != -1) {
    unsigned long start = aval_list[min_index][1];
    unsigned long end = start + byte;
    for(unsigned long i=start; i<end; i++) {
      arr[i] = processes[process_count];
    }
    process_count++;
    cursor = end;
    printf("ALLOCATED %s %lu\n",process, start );
  } else {
    printf("FAIL REQUEST %s %lu\n",process,byte );
  }
}

void next_fit(char* arr[], char* process, unsigned long byte) {
  processes[process_count] = malloc(16);
  strcpy(processes[process_count],process);
  int found = 0;
  unsigned long start = -1;
  unsigned long aval_byte = 0;
  unsigned long end = -1;
  for(unsigned long i=cursor; i < N; i++){
    if (arr[i] == NULL && i!=N-1) {
      if (found ==0) {
        start = i;
        found = 1;
      }
      aval_byte++;
    } else {
      if (i == N-1){
        if (start == -1){
          start = i;
          found = 1;
        }
        aval_byte++;
      }
      if (aval_byte >= byte) {
        end = start + byte;
        break;
      } else {
        start = -1;
        found = 0;
      }
    }
  }
  if (found == 0){
    for(unsigned long i=0; i < cursor; i++){
      if (arr[i] == NULL) {
        if (found ==0) {
          start = i;
          found = 1;
        }
        aval_byte++;
      } else {
        if (aval_byte >= byte) {
          end = i;
          break;
        } else {
          start = -1;
          found = 0;
        }
      }
    }
  }
  if (found == 1) {
    for(unsigned long i = start; i < end; i++) {
      arr[i] = processes[process_count];
    }
    process_count++;
    cursor = end;
    printf("ALLOCATED %s %lu\n",process,start );
  }else {
    printf("FAIL REQUEST %s %lu\n",process,byte );
  }
}

void release(char* arr[], char* process) {

  unsigned long info[2];
  int found = 0;
  unsigned long start = 0;
  for(unsigned long i = 0; i < N; i++) {
    if ( arr[i] != NULL && !strcmp(process, arr[i]) ) {
      if (found == 0) {
        start = i;
        found = 1;
      }
      if ( found == 1 && i == (N-1) ) {
        info[0] = i-start+1;
        info[1] = start;
      }
      arr[i] = NULL;
    } else {
      if ( found == 1 ) {
        info[0] = i-start;
        info[1] = start;
        break;
      }
    }
  }
  if(found == 1){
    for (int i=0; i<process_count; i++){
      if ( processes[i] != NULL && !strcmp(processes[i],process) ){
        processes[i] = NULL;
        break;
      }
    }
    printf("FREE %s %lu %lu\n",process,info[0],info[1] );
  }else {
    printf("FAIL RELEASE %s\n",process );
  }

}
int list_avaliable(char* arr[], unsigned long **list) {
  int index = 0;
  unsigned long aval = 0;
  unsigned long start = 0;
  for(unsigned long i = 0; i < N; i++) {
    if ( arr[i] == NULL ) {
      if (aval == 0) {
        start = i;
        aval = 1;
      }
      if ( aval ==1 && i == (N-1) ) {
        list[index][0] = i-start+1;
        list[index++][1] = start;
      }
    } else {
      if ( aval == 1 ) {
        list[index][0] = i-start;
        list[index++][1] = start;
        aval = 0;
      }
    }
  }
  return index;
}
int buddy_list_avaliable(char* arr[], struct node* cursor, unsigned long **list, int index) {

  if (cursor->isLeaf == 1 && cursor->process == NULL) {
      list[index][0] = cursor->size;
      list[index++][1] = cursor->start;
  } else {
      if (cursor->left == NULL) {
        return index;
      }
      index = buddy_list_avaliable(arr,cursor->left,list,index);
      index = buddy_list_avaliable(arr,cursor->right,list,index);
  }
  return index;


}
void list_assigned(char* arr[]) {
  int found = 0;
  for(int i = 0; i < process_count; i++){
    if (processes[i] != NULL ) {
      found = 1;
      find(arr, processes[i],0);
    }
  }
  if (found == 0) {
    printf("NONE" );
  }
  printf("\n");

}
void find(char* arr[], char* process, int type) {

  unsigned long info[2];
  int found = 0;
  unsigned long start = 0;
  for(unsigned long i = 0; i < N; i++) {
    if ( arr[i] != NULL && !strcmp(process, arr[i]) ) {
      if (found == 0) {
        start = i;
        found = 1;
      }
      if ( found == 1 && i == (N-1) ) {
        info[0] = i-start+1;
        info[1] = start;
      }
    } else {
      if ( found == 1 ) {
        info[0] = i-start;
        info[1] = start;
        break;
      }
    }
  }
  if(found == 1){
    printf("(%s,%lu,%lu) ",process,info[0],info[1] );
    if(type == 1) {
      printf("\n" );
    }
  }else {
    printf("%s is not found\n",process );
  }
}
