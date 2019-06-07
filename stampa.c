#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

/* Worked by Hernest Serani (MATRICOLA: 877028) Copyright ©
   Group Name: P1G83 */

void stampa () { /* prints the Machine Code */
  int opcode;
  unsigned int ip;
  printf("+-------------------------------+\n");
  printf("|   ID   | OPCODE     |P1  |P2  |\n");
  printf("+-------------------------------+\n");
  if (length == 0) {
    printf("Machine code file is blank.\n");
    free (CODE);
    exit(6);
  }
  for (ip=0;ip<length;) { /* goes through every line of CODE array */
    opcode = CODE[ip];
    printf("|->%6d|", ip); /* ID of machine code array */
    print_opcode (opcode); /* Prints the OPCODE Name */
    ip++;
    /* Parameter NR1
       Check if it is a register or constant
       R -> Register | N -> Number | - -> Nothing   */
    if (p1 (opcode) == 'R') {
      printf("R%-4d", CODE [ip]); /*Align to 4 digits for registers*/
      ip++;
    }
    else if (p1 (opcode) == 'N') {
      printf("%-4d", CODE [ip]); /*Align to 4 digits for constants*/
      ip++;
    }
    /* Parameter NR2
       Check if it is a register or constant
       R -> Register | N -> Number | - -> Nothing   */
    if (p2 (opcode) == 'R') {
      printf("R%d", CODE [ip]); /*Align to 4 digits for registers*/
      ip++;
    }
    else if (p2 (opcode) == 'N') {
      printf("%d", CODE [ip]); /*Align to 4 digits for constants*/
      ip++;
    }
    printf("\n");
  }
  printf("+-------------------------------+\n");
  free(CODE);
}
