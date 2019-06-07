#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

/* Worked by Hernest Serani (MATRICOLA: 877028) Copyright ©
   Group Name: P1G83 */

int *STACK;
unsigned int SP; /* Stack pointer */
unsigned int IP; /* Instruction pointer */
int REG[NR_REGISTERS];     /* REGISTERS */
FILE* log_file;  /* Log File to save executed instructions*/

unsigned int branch_instruction = 0;
unsigned int print_instruction = 0;
unsigned int data_transfer_instruction = 0;
unsigned int arithmetic_instruction = 0;
unsigned int IC = 0;

void esegui () {
  int i;
  fclose(log_file = fopen("logs.txt", "w")); /* empty the logs file */
  STACK = (int *) malloc (STACK_MAX_SIZE); /* allocates memory in Dynamic Memory for stack */
  if (!STACK) {
    printf("Cannot allocate memory for STACK. Exiting...\n");
    free (CODE);
    free (STACK);
    exit(7);
  }
  if (length == 0) { /* Machine Code file is blank */
    printf("Machine code file is blank.\n");
    free(CODE);
    free(STACK);
    exit (6);
  }
  for (i=0;i<NR_REGISTERS;i++)
    REG[i] = 0; /* initialize all Registers with 0 */
  SP = 0; /* initialize Stack Pointer with 0 (Points to the first place of stack) */
  IP = 0; /* initialize Instruction Pointer with 0 */
  printf("+========================================================+\n");
  printf("|                 EXECUTED MACHINE CODE                  |\n");
  printf("+========================================================+\n");
  while (1) {
    IC++; /* Number of instructions executing */
    log_line (IP); /* Logs every instruction to file */
    switch (CODE[IP]) {
      case 0: /* HALT */
        branch_instruction++;
        HALT();
        break;
      case 1: /* DISPLAY */
        print_instruction++;
        DISPLAY (CODE[IP+1]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 2: /* PRINT_STACK */
        print_instruction++;
        PRINT_STACK (CODE[IP+1]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 10: /* PUSH */
        data_transfer_instruction++;
        PUSH (CODE[IP+1]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 11: /* POP */
        data_transfer_instruction++;
        POP (CODE[IP+1]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 12: /* MOV */
        data_transfer_instruction++;
        MOV (CODE[IP+1], CODE[IP+2]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 20: /* CALL */
        branch_instruction++;
        CALL (CODE[IP+1]);
        break;
      case 21: /* RET */
        branch_instruction++;
        RET ();
        break;
      case 22: /* JMP */
        branch_instruction++;
        JMP (CODE[IP+1]);
        break;
      case 23: /* JZ */
        branch_instruction++;
        JZ (CODE[IP+1]);
        break;
      case 24: /* JPOS */
        branch_instruction++;
        JPOS (CODE[IP+1]);
        break;
      case 25: /* JNEG */
        branch_instruction++;
        JNEG (CODE[IP+1]);
        break;
      case 30: /* ADD */
        arithmetic_instruction++;
        ADD (CODE[IP+1], CODE[IP+2]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 31: /* SUB */
        arithmetic_instruction++;
        SUB (CODE[IP+1], CODE[IP+2]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 32: /* MUL */
        arithmetic_instruction++;
        MUL (CODE[IP+1], CODE[IP+2]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      case 33: /* DIV */
        arithmetic_instruction++;
        DIV (CODE[IP+1], CODE[IP+2]);
        IP = IP + length_from_op (CODE[IP]);
        break;
      default: /* UNKNOWN INSTRUCTION */
        printf ("Unknown Instruction. Exiting...\n");
        HALT ();
        break;
    }
  }
}
