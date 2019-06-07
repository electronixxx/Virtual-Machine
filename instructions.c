#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "settings.h"

/* Worked by Hernest Serani (MATRICOLA: 877028) Copyright ©
   Group Name: P1G83 */

void HALT () {
  printf("END OF EXECUTION.\n");
  printf("+========================================================+\n");
  performance_log(); /*prints virtual machine details */
  free(STACK);
  free(CODE);
  exit (0);
}

void DISPLAY (int id) {
  if (!(id>=0 && id<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id);
    HALT ();
  }
  printf("REG[%d]= %d\n",id, REG[id]);
}

void PRINT_STACK (int n) {
  int i;
  int int_SP = SP;
  printf("\n+--------------------------------------------------------+\n");
  printf("|       PRINTING STACK FOR THE LAST %-5d VALUES         |\n", n);
  printf("+--------------------------------------------------------+\n");
  if (int_SP>=1) {
    for (i=int_SP-1;i>=int_SP-n && i>=0;i--)
      printf("|STACK[%d] = %-45d|\n", i, STACK[i]);
  }
  else
    printf("|EMPTY STACK. %-43c|\n", ' ');
  printf("+--------------------------------------------------------+\n\n");
}

void PUSH (int id) {
  if (!(id>=0 && id<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id);
    HALT ();
  }
  if (SP <= STACK_MAX_SIZE/4) {
    STACK[SP] = REG[id];
    SP++;
  }
  else {
    printf("STACK OVERFLOW! Exiting...\n");
    HALT();
  }
}

void POP (int id) {
  if (!(id>=0 && id<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id);
    HALT ();
  }
  if (SP > 0) {
    SP--;
    REG[id] = STACK[SP];
  }
  else {
    printf("STACK UNDERFLOW! Exiting...\n");
    HALT();
  }
}

void MOV (int id, int value) {
  if (!(id>=0 && id<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id);
    HALT ();
  }
  REG[id] = value;
}

void CALL (int position) {
  IP = IP + length_from_op (CODE[IP]);
  if (SP <= STACK_MAX_SIZE/4) {
    STACK[SP] = IP;
    SP++;
  }
  else {
    printf("STACK OVERFLOW! Exiting...\n");
    HALT();
  }
  JMP (position);
}

void RET () {
  if (SP > 0) {
    SP--;
    IP = STACK[SP];
  }
  else {
    printf("STACK UNDERFLOW! Exiting...\n");
    HALT();
  }
}

void JMP (int value) {
  IP = value;
}

void JZ (int value) {
  SP--;
  if (STACK[SP] == 0)
    IP = value;
  else
    IP = IP + length_from_op (CODE[IP]);
}

void JPOS (int value) {
  SP--;
  if (STACK[SP] > 0)
    IP = value;
  else
    IP = IP + length_from_op (CODE[IP]);
}

void JNEG (int value) {
  SP--;
  if (STACK[SP] < 0)
    IP = value;
  else
    IP = IP + length_from_op (CODE[IP]);
}

void ADD (int id_1, int id_2) {
  if (!(id_1>=0 && id_1<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_1);
    HALT ();
  }
  if (!(id_2>=0 && id_2<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_2);
    HALT ();
  }
  if (SP <= STACK_MAX_SIZE/4) {
    int a = REG [id_1];
    int b = REG [id_2];
    int sum;
    if ((b > 0) && (a > INT_MAX - b)) { /* a + b would overflow */
      printf("ADDITION OVERFLOW! Exiting...\n");
      HALT();
    }
    else if ((b < 0) && (a < INT_MIN - b)) {  /* a + b would underflow */
      printf("ADDITION UNDERFLOW! Exiting...\n");
      HALT();
    }
    else {
      sum = a + b;
      STACK[SP] = sum;
      SP++;
    }
  }
  else {
    printf("STACK OVERFLOW! Exiting...\n");
    HALT();
  }
}

void SUB (int id_1, int id_2) {
  if (!(id_1>=0 && id_1<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_1);
    HALT ();
  }
  if (!(id_2>=0 && id_2<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_2);
    HALT ();
  }
  if (SP <= STACK_MAX_SIZE/4) {
    int a = REG [id_1];
    int b = REG [id_2];
    int dif;
    if ((b < 0) && (a > INT_MAX + b)) { /* a - b would overflow */
      printf("SUBTRACTION OVERFLOW! Exiting...\n");
      HALT();
    }
    if ((b > 0) && (a < INT_MIN + b)) { /* a - b would underflow */
      printf("SUBTRACTION UNDERFLOW! Exiting...\n");
      HALT();
    }
    else {
      dif = a - b;
      STACK[SP] = dif;
      SP++;
    }
  }
  else {
    printf("STACK OVERFLOW! Exiting...\n");
    HALT();
  }
}

void MUL (int id_1, int id_2) {
  if (!(id_1>=0 && id_1<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_1);
    HALT ();
  }
  if (!(id_2>=0 && id_2<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_2);
    HALT ();
  }
  if (SP <= STACK_MAX_SIZE/4) {
    int a = REG [id_1];
    int b = REG [id_2];
    int product;
    if ((a == -1 && b == INT_MIN) || (b == -1 && a == INT_MIN) || (a > INT_MAX / b) || (a < INT_MIN / b)) {
        printf("MULTIPLICATION OVERFLOW! Exiting...\n");
        HALT();
    }
    else {
      product = a * b;
      STACK[SP] = product;
      SP++;
    }
  }
  else {
    printf("STACK OVERFLOW! Exiting...\n");
    HALT();
  }
}

void DIV (int id_1, int id_2) {
  if (!(id_1>=0 && id_1<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_1);
    HALT ();
  }
  if (!(id_2>=0 && id_2<NR_REGISTERS)) { /* If register does not exist, temrinates the program */
    printf("Register R%d does not exist in program. Exiting...\n", id_2);
    HALT ();
  }
  if (SP <= STACK_MAX_SIZE/4) {
    int a = REG [id_1];
    int b = REG [id_2];
    int div;
    if (b == 0) {
      printf("DIVISION BY ZERO! Exiting...\n");
      HALT();
    }
    else {
      div = a / b;
      STACK[SP] = div;
      SP++;
    }
  }
  else {
    printf("STACK OVERFLOW! Exiting...\n");
    HALT();
  }
}
