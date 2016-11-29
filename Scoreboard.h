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
    int immediate_or_offset;
};

// struct FU_Status FU_Statuses[];

// struct FU Register_Result_Status[];


struct ScoreBoard{
    struct FU_Status FU_Statuses[5];
    struct Instruction_Status Instruction_Statuses[10];
    enum FU Register_Result_Status[32];
};

#endif