#include "cpu.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>

#define DATA_LEN 12

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *program)
{

  FILE *fp;

  char line[1024];

  fp = fopen(program, "r");

  if( fp == NULL) {
    fprintf(stderr, "Error opening file.\n");
    exit(1);
  }

  int i = 0x00;
  while( fgets(line, sizeof(line), fp) != NULL ) {
    char *endPtr;
    unsigned char byte = strtol(line, &endPtr, 2);

    if(endPtr == line) {
      continue;
    }

    cpu_ram_write(cpu, i++, byte);
  
  }

  fclose(fp);

}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, int address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  //int stack[256];
  //int sp = cpu->registers[7];

  //int PC = cpu->PC;
  //unsigned char value;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch(instruction) {

      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case MUL:
        cpu->registers[operandA] = (cpu->registers[operandA] * cpu->registers[operandB]);
        //test that MUL completed successfully
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;
/*
      case PUSH:
        sp--;
        //cpu->registers[sp] = cpu->registers[operandA];
        cpu_ram_write(cpu, sp, cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case POP:
        cpu->registers[operandA] = cpu_ram_read(cpu, sp);
        sp++;
        cpu->PC += 2;
        break;

      case CALL:
        sp--;
        cpu->registers[sp] = cpu->registers[operandA];
        cpu_ram_write(cpu, cpu->PC, operandA);
        cpu->PC = cpu->registers[sp];
        break;

      case RET:
        cpu->registers[operandA] = cpu_ram_read(cpu, sp);
        cpu->PC = cpu->registers[operandA];
        sp++;
        break;
        */
        
      case JMP:
        //printf("jumping to instruction\n");
        cpu->PC = cpu->registers[operandA];
        break;

      case CMP:
        if(cpu->registers[operandA] == cpu->registers[operandB]) {
          //printf("setting FL to 1\n");
          cpu->FL = 1;
          cpu->PC += 3;
          break;

        } else {
          //printf("setting FL to 0\n");
          cpu->FL = 0;
          cpu->PC += 3;
          break;
        }
        //break;
      
      case JEQ:
        if(cpu->FL == 1) {
          cpu->PC = cpu->registers[operandA];
          break;
        } else {
          cpu->PC += 2;
          break;
        }
        //break;

      case JNE:
        if(cpu->FL == 0) {
          cpu->PC = cpu->registers[operandA];
          break;
        } else {
          cpu->PC += 2;
          break;
        }
        //break;

      default:
       fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, instruction);
       exit(3);

    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->FL = 0;

  memset(cpu->registers, 0, 8*sizeof(unsigned char));

  memset(cpu->ram, 0, 8*sizeof(unsigned char));

  //cpu->registers[7] = 0xF4;

}
