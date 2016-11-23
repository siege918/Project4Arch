typedef int32 mem_addr; //32 bits to store memory addresses
typedef int32 mem_word; //32 bit storage unit
typedef uint8_t byte; //Standard size of memory unit

enum instruction { nope = 255, syscall = 0, addi = 1, subi = 2, beqz = 3, bge = 4, bne = 5, la = 6, lb = 7, li = 8, b = 9, add = 10 };

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

int num_of_vars;

float f_registers[];
mem_word r_registers[];

void execute_code()
{
	
}

