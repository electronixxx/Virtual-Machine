#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "settings.h"

/* Worked by Hernest Serani (MATRICOLA: 877028) Copyright ©
   Group Name: P1G83 */

unsigned int length = 0; /* length of machine code array */

void upload_array () { /* Copy machine code from file to the machine code array */
  size_t buffer_size =  0; /* getline will increase the size of buffer, so size is not important */
  char *s = NULL; /* Buffer to write the read line */
  int dim_found = 0; /* controlls if has read the first int from file */
  int i = 0, elem_read;
  while (getline (&s, &buffer_size, machine_code_file) != -1) {
    if (sscanf( s, "%d ; %*[^\n]", &elem_read) == 1) { /* ignore comments (Get the first number, ignore everything) */
      if (dim_found == 0) { /* reading firt line */
        /* allocates memory for machine code array*/
        length = elem_read;
        CODE = (int *) malloc (sizeof (int) * length); /* Allocates memory for machine code array */
        dim_found = 1; /* Got the first line */
        if (!CODE) { /* If can't allocate memory for instructions array */
          printf("Cannot allocate memory to hold the instructions. Exiting...\n");
          free(s);
          fclose (machine_code_file);
          exit(7);
        }
      }
      else {
        CODE[i] = elem_read; /* copy every machine code to the machine code array */
        i++;
      }
    }
  }
  free(s);
  fclose (machine_code_file);
}

void upload_instructions () { /* uploads all opcodes from file to struct */
  FILE *instructions;
  int i = 0;
  if ((instructions = fopen("instructions.txt", "r"))) { /* Read the file with opcodes */
    while (!feof(instructions)) { /* Copy machine opcodes to struct */
      fscanf(instructions, "%s %d %d %c %c\n", op[i].name, &op[i].length, &op[i].code, &op[i].parameter_1, &op[i].parameter_2);
      i++;
    }
    fclose(instructions);
  }
  else {
    printf("Instruction File is mising!\n"); /* File instructions.txt does not exist in the current directory */
    fclose (machine_code_file);
    exit(4);
  }
}

int length_from_op (int n) { /* Find the length of opcode from opcode ID*/
  int i, len = 0;
  for (i=0;i<NR_INSTRUCTIONS;i++) {
    if (n == op[i].code)
      len = op[i].length;
  }
  return len;
}

void log_line (int ip) { /* Save every executed instruction to the file logs.txt */
  int i, opcode = CODE[ip];
  log_file = fopen("logs.txt", "a");
  if (ip == 0) { /* Not executed anything yet */
    fprintf(log_file, "+========================================================+\n");
    fprintf(log_file, "|                 EXECUTED MACHINE CODE                  |\n");
    fprintf(log_file, "+========================================================+\n\n");
  }
  for (i=0;i<NR_INSTRUCTIONS;i++) {
    if (opcode == op[i].code)
      fprintf(log_file, "%s\t", op[i].name); /* Prints the name of current opcode */
  }
  ip++; /* Increments the IP to go to the next parameter */
  /* Parameter NR1
     Check if it is a register or constant */
  if (p1 (opcode) == 'R') { /* If it is a Register, prints the "R" */
    fprintf(log_file, "R%d\t", CODE[ip]);
    ip++; /* Increments the IP to go to the next parameter */
  }
  else if (p1 (opcode) == 'N') { /* If it is a Number, just prints the number */
    fprintf(log_file, "%d\t", CODE[ip]);
    ip++; /* Increments the IP to go to the next parameter */
  }
  /* Parameter NR2
     Check if it is a register or constant */
  if (p2 (opcode) == 'R') { /* If it is a Register, prints the "R" */
    fprintf(log_file, "R%d\t", CODE[ip]);
    ip++; /* Increments the IP to go to the next parameter */
  }
  else if (p2 (opcode) == 'N') { /* If it is a Number, just prints the number */
    fprintf(log_file, "%d\t", CODE[ip]);
    ip++; /* Increments the IP to go to the next parameter */
  }
  fprintf(log_file, "\n");
  fclose(log_file);
}

void print_opcode (int n) { /* prints the name of OPCODE from OPCODE ID */
  int i, found = 0;
  for (i=0;i<NR_INSTRUCTIONS;i++) {
    if (n == op[i].code) {
      found = 1;
      printf(" %-12s", op[i].name);
    }
  }
  if (!found) {
    printf (" Unknown Instruction. Exiting...\n");
    free (STACK); /* frees the stack */
    free(CODE); /* frees the machine code array */
    exit(5);
  }
}

char p1 (int opcode) { /* Checks if first parameter is a Register or Number */
  int i;
  char status = '-';
  for (i=0;i<NR_INSTRUCTIONS;i++) {
    if (opcode == op[i].code) {
      if (op[i].parameter_1 == 'R')
        status = 'R';
      else if (op[i].parameter_1 == 'N')
        status = 'N';
    }
  }
  return status;
}

char p2 (int opcode) { /* Checks if second parameter is a Register or Number */
  int i;
  char status = '-';
  for (i=0;i<NR_INSTRUCTIONS;i++) {
    if (opcode == op[i].code) {
      if (op[i].parameter_2 == 'R')
        status = 'R';
      else if (op[i].parameter_2 == 'N')
        status = 'N';
    }
  }
  return status;
}

void performance_log () { /* Prints the log of VM performance and executed details */
  time_t mytime = time (NULL); /* returns current time in seconds */
  char *time_str = ctime (&mytime); /* converts seconds to human readable time*/
  float CPI_AVERAGE = ((float) branch_instruction / (float) IC * (float) CPI_BRANCH +
                       (float) print_instruction / (float) IC * (float) CPI_PRINT +
                       (float) data_transfer_instruction / (float) IC * (float) CPI_DATA_TRANSFER +
                       (float) arithmetic_instruction / (float) IC * (float) CPI_ARITHMETIC);
  float EXE_TIME = IC * CPI_AVERAGE * 1.0 / FREQUENCY * 1000000000.0 ; /* Exe Time in nanoseconds */

  printf("\n\n\n+========================================================+\n");
  printf("|                 VIRTUAL MACHINE LOGS                   |\n");
  printf("+========================================================+\n");
  printf("|Executed on: %.19s                        |\n", time_str);
  printf("|INSTRUCTION COUNT (IC): %-32d|\n", IC); /* Total number of executed Instructions */
  printf("|EXE_TIME: %10.2f nanoseconds %23s|\n", EXE_TIME, " ");
  printf("|CPU FREQUENCY: %10.5f Ghz                           |\n", FREQUENCY / 1000000000.0);
  printf("|CPI BRANCH: %d ns                                        |\n", CPI_BRANCH);
  printf("|CPI PRINT: %d ns                                         |\n", CPI_PRINT);
  printf("|CPI DATA TRANSFER: %d ns                                 |\n", CPI_DATA_TRANSFER);
  printf("|CPI ARITHMETIC: %d ns                                    |\n", CPI_ARITHMETIC);
  printf("|CPI AVERAGE: %.3f ns                                   |\n", CPI_AVERAGE);
  printf("+========================================================+\n");
  printf("|Branch Instructions Executed: %-5d (%05.2f %%) %11s\n", branch_instruction, branch_instruction * 100.0 / IC, "|");
  printf("+--------------------------------------------------------+\n");
  printf("|Print Instructions Executed: %-5d (%05.2f %%) %12s\n", print_instruction, print_instruction * 100.0 / IC, "|");
  printf("+--------------------------------------------------------+\n");
  printf("|Data Transfer Instructions Executed: %-5d (%05.2f %%) %4s\n", data_transfer_instruction, data_transfer_instruction * 100.0 / IC, "|");
  printf("+--------------------------------------------------------+\n");
  printf("|Arithmetic Instructions Executed: %-5d (%05.2f %%) %7s\n", arithmetic_instruction, arithmetic_instruction * 100.0 / IC, "|");
  printf("+========================================================+\n");

  log_file = fopen("logs.txt", "a"); /* appends to the logs file */

  fprintf(log_file, "\n\n\n+========================================================+\n");
  fprintf(log_file, "|                 VIRTUAL MACHINE LOGS                   |\n");
  fprintf(log_file, "+========================================================+\n");
  fprintf(log_file, "|Executed on: %.19s                        |\n", time_str);
  fprintf(log_file, "|INSTRUCTION COUNT (IC): %-32d|\n", IC);
  fprintf(log_file, "|EXE_TIME: %-10.2f nanoseconds %23s|\n", EXE_TIME, " ");
  fprintf(log_file, "|CPU FREQUENCY: %10.5f Ghz                           |\n", FREQUENCY / 1000000000.0);
  fprintf(log_file, "|CPI BRANCH: %d ns                                        |\n", CPI_BRANCH);
  fprintf(log_file, "|CPI PRINT: %d ns                                         |\n", CPI_PRINT);
  fprintf(log_file, "|CPI DATA TRANSFER: %d ns                                 |\n", CPI_DATA_TRANSFER);
  fprintf(log_file, "|CPI ARITHMETIC: %d ns                                    |\n", CPI_ARITHMETIC);
  fprintf(log_file, "|CPI AVERAGE: %.3f ns                                   |\n", CPI_AVERAGE);
  fprintf(log_file, "+========================================================+\n");
  fprintf(log_file, "|Branch Instructions Executed: %-5d (%05.2f %%) %11s\n", branch_instruction, branch_instruction * 100.0 / IC, "|");
  fprintf(log_file, "+--------------------------------------------------------+\n");
  fprintf(log_file, "|Print Instructions Executed: %-5d (%05.2f %%) %12s\n", print_instruction, print_instruction * 100.0 / IC, "|");
  fprintf(log_file, "+--------------------------------------------------------+\n");
  fprintf(log_file, "|Data Transfer Instructions Executed: %-5d (%05.2f %%) %4s\n", data_transfer_instruction, data_transfer_instruction * 100.0 / IC, "|");
  fprintf(log_file, "+--------------------------------------------------------+\n");
  fprintf(log_file, "|Arithmetic Instructions Executed: %-5d (%05.2f %%) %7s\n", arithmetic_instruction, arithmetic_instruction * 100.0 / IC, "|");
  fprintf(log_file, "+========================================================+\n");

  fclose(log_file);
}
