/* Worked by Hernest Serani (MATRICOLA: 877028) Copyright ©
   Group Name: P1G83 */

#define MAX_LENGTH_OF_INSTRUCTION 20 /* Maximum length of an instruction name */
#define STACK_MAX_SIZE 64000 /*64 KB */
#define NR_INSTRUCTIONS 16  /*Number of available instructions */
#define NR_REGISTERS 32 /*Number of register of the Virtual Machine */

/* Cycle per Instruction  in nanoseconds*/
#define CPI_BRANCH 2
#define CPI_PRINT 3
#define CPI_DATA_TRANSFER 3
#define CPI_ARITHMETIC 4
#define FREQUENCY 2500000000 /* 2.5 GHz -- FREQUENCY of the CPU (Used to calculate EXE_TIME) */

struct op_codes { /*Structure which will hold the list of instructions */
  char name[MAX_LENGTH_OF_INSTRUCTION]; /* Name of the opcode */
  int length; /* length of an instruction */
  int code; /* Code of opcode */
  char parameter_1;
  char parameter_2;
}op_codes;

/* Global variables available among functions */
extern unsigned int length; /* length of machine code array */
extern unsigned int SP; /* Stack pointer */
extern unsigned int IP; /* Instruction pointer */
extern unsigned int branch_instruction; /* Nr of branch instructions */
extern unsigned int print_instruction; /* Nr of print instructions */
extern unsigned int data_transfer_instruction; /* Nr of data transfer instructions */
extern unsigned int arithmetic_instruction; /* Nr of arithmetic instructions */
extern unsigned int IC; /* Total number of executed instructions */
extern struct op_codes op[NR_INSTRUCTIONS]; /*Structure which will hold the list of opcodes */
extern FILE* machine_code_file; /* Pointer to file with machine code */
extern FILE* log_file; /* Pointer to logs file */
extern int *CODE; /* Machine code */
extern int *STACK; /* Pointer to the STACK */
extern int REG[32]; /* VM registers */

/* Function Prototypes */
void stampa ();
void esegui ();
void upload_array ();
void upload_instructions ();
void log_line (int actual);
void print_opcode (int n);
void performance_log ();
char p1 (int opcode);
char p2 (int opcode);
int length_from_op (int n);

/* Instruction Function Prototypes */
void HALT ();
void DISPLAY (int id);
void PRINT_STACK (int n);
void PUSH (int value);
void POP (int id);
void MOV (int id, int value);
void CALL (int position);
void RET ();
void JMP (int value);
void JZ (int value);
void JPOS (int value);
void JNEG (int value);
void ADD (int id_1, int id_2);
void SUB (int id_1, int id_2);
void MUL (int id_1, int id_2);
void DIV (int id_1, int id_2);
