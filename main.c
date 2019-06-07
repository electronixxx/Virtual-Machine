#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"

/*
  Worked by Hernest Serani (MATRICOLA: 877028) Copyright ©
  Group Name: P1G83
  Ca' Foscari, Academic Year 2018 - 2019
  Course: PROGRAMMAZIONE E LABORATORIO [CT0442]

  View the documentation file for more info. (README.txt)
*/

struct op_codes op[NR_INSTRUCTIONS]; /* Struct to hold all opcodes information of VM */
FILE* machine_code_file; /* pointer to the file of  machine code */
int *CODE; /* machine code array (pointer) */

int main (int argc,char* argv[]) {
  if (argc == 3) { /* if there are three parameters when executing */
    if (!strcmp (argv[1], "stampa")) {
      if ((machine_code_file = fopen(argv[2], "r"))) { /* file with machine code exists */
        upload_instructions (); /* uploads all opcodes from instructions file to struct */
        upload_array (); /* uploads machine code to array (ignoring the comments) */
        stampa ();
      }
      else { /* file with machine code DOES NOT exist */
        printf("File \"%s\" does not exist in the current directory.\n", argv[2]);
        exit(1);
      }
    }
    else if (!strcmp (argv[1], "esegui")) {
      if ((machine_code_file = fopen(argv[2], "r"))) { /* file with machine code exists */
        upload_instructions (); /* uploads all opcodes from file to struct */
        upload_array (); /* uploads machine code to array (ignoring the comments) */
        esegui ();
      }
      else { /* file with machine code DOES NOT exist */
        printf("File \"%s\" does not exist in the current directory.\n", argv[2]);
        exit(1);
      }
    }
    else { /* wrong command */
      printf("Command \"%s\" does not exist in program.\n", argv[1]);
      exit(2);
    }
  }
  else {
    /* NOT ENOUGH PARAMETERS OR TOO MANY */
    if (argc > 3) {
      printf("Too many parameters. Program supports only 2 parameters.\n");
      exit(3);
    }
    else {
      printf("Few parameters. Program supports 2 parameters.\n");
      exit(3);
    }
  }
  return 0;
}
