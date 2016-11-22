enum FU {INTEGER, MULT1, MULT2, ADD, DIVIDE};
enum OP {TEST};

struct Instruction_Status {
    int Issue;
    int Read_Op;
    int Exec_Complete;
    int Write_Result;
};

struct Instruction_Status Instruction_Statuses[];

struct FU_Status {
    int busy;
    OP op_code;
    int Fi;
    int Fj;
    int Fk;
    FU Qj;
    FU Qk;
    int Rj;
    int Rk;
};

struct FU_Status FU_Statuses[];

FU Register_Result_Status[];