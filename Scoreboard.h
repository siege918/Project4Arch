#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>


typedef int mem_addr; //32 bits to store memory addresses
typedef int mem_word; //32 bit storage unit
typedef uint8_t byte; //Standard size of memory unit
typedef mem_word reg; //32 bit type for our registers

enum instruction { nope = 255, syscall = 0, addi = 1, subi = 2, beqz = 3, bge = 4, bne = 5, la = 6, lb = 7, li = 8, b = 9, add = 10, fadd = 11, fmul = 12, fsub = 13, ld = 14, sd = 15 };

//Struct used to store variables
struct variable {
    mem_addr variable_addr; //Address of where the data is stored
    char variable_name[64]; //Name of the variable
    int is_text; //This is used as a bool to determine if it was a string
};

struct label_reference {
    char label[64];
    mem_addr code_location;
};

struct label_definition {
    char label[64];
    mem_addr pc_addr;
};

struct ex_wr {
    enum instruction op_code;
    mem_word ALU_out;
	float f_ALU_out;
    mem_word operand_b;
    mem_word rd;
    int init;
};


enum FU {INTEGER, MULT1, MULT2, ADD, DIVIDE, NOPE = 255};
enum OP {TEST};

struct Instruction_Status {
    int Issue;
    int Read_Op;
    int Exec_Complete;
    int Write_Result;
};

//struct Instruction_Status Instruction_Statuses[];

struct FU_Status {
    int busy;
    enum OP op_code;
    int Fi;
    int Fj;
    int Fk;
    enum FU Qj;
    enum FU Qk;
    int Rj;
    int Rk;
};

// struct FU_Status FU_Statuses[];

// struct FU Register_Result_Status[];


struct ScoreBoard{
    struct FU_Status FU_Statuses[5];
    struct Instruction_Status Instruction_Statuses[10];
    enum FU Register_Result_Status[32];
};


struct is_ro{
    enum instruction op_code;
    int ir;
};

struct ro_ex{
    enum instruction op_code;
    //Register names/not values
    mem_word rs;
    mem_word rt;
    mem_word rd;
    mem_word operand_a;
    mem_word operand_b;
    float f_operand_a;
    float f_operand_b;
    mem_word ALU_out;
    mem_word immediate_or_offset;
};


#endif