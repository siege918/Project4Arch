#include <stdint.h>
#include "Scoreboard.h"


typedef int mem_addr; //32 bits to store memory addresses
typedef int mem_word; //32 bit storage unit
typedef uint8_t byte; //Standard size of memory unit

enum instruction { nope = 255, syscall = 0, addi = 1, subi = 2, beqz = 3, bge = 4, bne = 5, la = 6, lb = 7, li = 8, b = 9, add = 10, fadd = 11, fmul = 12, fsub = 13, ld = 14, sd = 15 };

int labelReferenceCount = 0;
int labelCount = 0;

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

int issue();
int detectStructuralHazard();
int checkForWAW();
int setFetchBuffer();

int main() 
{
    int num_of_vars;
    float f_registers[35];
    mem_word r_registers[35];

    // struct FU_Status FU_Statuses[5];
    enum FU Register_Result_Status[35];
    // struct Instruction_Status Instruction_Statuses[35];

    while(1) {
        Issue();
    }

    return 0;
}


issue(int *PC)
{
    int ir = *pc;
    int returnValue = 255;

    if(DetectIssueHazard()){
        return returnValue;
    }
    if(CheckForWAW())
    {
        return returnValue;
    }

    if (instruction == la || instruction == lb || instruction == li || instruction == ld || instruction == sd)
    {
        Scoreboard.FU_Statuses[0].busy = 1;
        Scoreboard.FU_Statuses[0].op = memory[ir];
        Scoreboard.FU_Statuses[0].Fi = memory[ir + 1];

        //Scoreboard.FU_Statuses[0].Fk = memory[sourcelocation];
        if (instruction == la)
        {
            Scoreboard.FU_Statuses[0].Fk = memory[ir + 2];
        }
        // else if (instruction == lb)
        // {
        // }
        // else if (instruction == li)
        // {
        // }
        // else if (instruction == ld)
        // {
        // }
        else if (instruction == sd)
        {
            loadFloatIntoMemory(memory[memory[offsetLocation] + registers[sourcelocation]], memory[sourcelocation]);     
        }
        else
        {
        }

        Scoreboard.FU_Statuses[0].Qk = Register_Result_Status[memory[ir + 2]];
        if(Scoreboard.FU_Statuses[0].Qk == null)
        {
            Scoreboard.FU_Statuses[0].Rk = 1;
        }
        else
        {
            Scoreboard.FU_Statuses[0].Rk = 0;
        }


        Register_Result_Status[memory[ir + 2]] = INTEGER;
        returnValue = 0;

    }
    else if (instruction == fmul)
    {
        //If mul1 is busy, try mul2
        if (Scoreboard.FU_Statuses[1].busy != 1)
        {
            Scoreboard.FU_Statuses[1].busy = 1;
            Scoreboard.FU_Statuses[1].op = memory[ir];
            Scoreboard.FU_Statuses[1].Fi = memory[ir + 1];
            Scoreboard.FU_Statuses[1].Fj = memory[ir + 2];
            Scoreboard.FU_Statuses[1].Fk = f_registers[memory[sourcelocation]];
            Scoreboard.FU_Statuses[1].Qj = Register_Result_Status[destinationLocation];
            Scoreboard.FU_Statuses[1].Qk = Register_Result_Status[destinationLocation];

            if(Scoreboard.FU_Statuses[1].Qj == null)
            {
                Scoreboard.FU_Statuses[1].Rj = 1;
            }
            else
            {
                Scoreboard.FU_Statuses[1].Rj = 0;
            }
            if(Scoreboard.FU_Statuses[1].Qk == null)
            {
                Scoreboard.FU_Statuses[1].Rk = 1;
            }
            else
            {
                Scoreboard.FU_Statuses[1].Rk = 0;
            }

            Register_Result_Status[destinationLocation] = MULT1;
            returnValue = 1;
        }
        else 
        {
            Scoreboard.FU_Statuses[2].busy = 1;
            Scoreboard.FU_Statuses[2].op = memory[opcodeLocation];
            Scoreboard.FU_Statuses[2].Fi = memory[destinationLocation];
            Scoreboard.FU_Statuses[2].Fj = memory[targetLocation];
            Scoreboard.FU_Statuses[2].Fk = memory[sourcelocation];
            Scoreboard.FU_Statuses[2].Qj = Register_Result_Status[destinationLocation];
            Scoreboard.FU_Statuses[2].Qk = Register_Result_Status[destinationLocation];

            if(Scoreboard.FU_Statuses[2].Qj == null)
            {
                Scoreboard.FU_Statuses[2].Rj = 1;
            }
            else
            {
                Scoreboard.FU_Statuses[2].Rj = 0;
            }
            if(Scoreboard.FU_Statuses[2].Qk == null)
            {
                Scoreboard.FU_Statuses[2].Rk = 1;
            }
            else
            {
                Scoreboard.FU_Statuses[2].Rk = 0;
            }

            Register_Result_Status[destinationLocation] = MULT2;
            returnValue = 2;
        }
    }
    else if (instruction == addi || instruction == add || instruction == fadd || instruction == fsub)
    {
        Scoreboard.FU_Statuses[3].busy = 1;
        Scoreboard.FU_Statuses[3].op = memory[opcodeLocation];
        Scoreboard.FU_Statuses[3].Fi = memory[destinationLocation];
        Scoreboard.FU_Statuses[3].Fj = memory[targetLocation];
        if (instruction == addi)
        {
            Scoreboard.FU_Statuses[3].Fk = memory[sourcelocation];
        }
        else if (instruction == add)
        {
            Scoreboard.FU_Statuses[3].Fk = registers[memory[sourcelocation]];
        }
        else
        {
            Scoreboard.FU_Statuses[3].Fk = f_registers[memory[sourcelocation]];
        }

        Scoreboard.FU_Statuses[3].Qj = Register_Result_Status[destinationLocation];
        Scoreboard.FU_Statuses[3].Qk = Register_Result_Status[destinationLocation];

        if(Scoreboard.FU_Statuses[3].Qj == null)
        {
            Scoreboard.FU_Statuses[3].Rj = 1;
        }
        else
        {
            Scoreboard.FU_Statuses[3].Rj = 0;
        }
        if(Scoreboard.FU_Statuses[3].Qk == null)
        {
            Scoreboard.FU_Statuses[3].Rk = 1;
        }
        else
        {
            Scoreboard.FU_Statuses[3].Rk = 0;
        }

        Register_Result_Status[destinationLocation] = ADD;
        returnValue = 3;
    }
    PC++;
    return returnValue;


}
int detectStructuralHazard()
{

}
int checkForWAW()
{

}
int setFetchBuffer()
{

}