#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

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