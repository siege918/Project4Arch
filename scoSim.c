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







//Returns a 32 bit value from the address in the specified array
mem_word getWordFromByteArray(byte* array, mem_addr location) {
	int i = 0;
	int value = 0;
	while (i < 4)
	{
		value = (value << 8) + array[location + i];
		i++;
	}
	return value;
}

//Inserts a word into an array at the selected address as 4 bytes
void insertWordIntoByteArray(byte* array, mem_addr location, mem_word value) {
	int i = 4;
	int workingValue = value;
	while (i > 0) {
		array[location + i - 1] = workingValue & 0xFF;
		workingValue = workingValue >> 8;
		i--;
	}
}

//Returns a float from the address in the specified array
float getFloatFromByteArray(byte* array, mem_addr location)
{
	mem_word val = getWordFromByteArray(array, location);
	
	printf("%d\n", val);
	
	mem_word leftSide = (val >> 16) & 0xFFFF;
	mem_word rightSide = val & 0xFFFF;
	
	printf("Left side: %d\n", leftSide);
	printf("Right side: %d\n", rightSide);
	
	float f_rightSide = (float)rightSide;
	
	
	while (f_rightSide > 1)
	{
	    printf("Right side: %2.2f\n", f_rightSide);
		f_rightSide = f_rightSide * .1f;
	}
	
	return leftSide + f_rightSide;
}

//Inserts a float into an array at the selected array as 4 bytes
void insertFloatIntoByteArray(byte* array, mem_addr location, mem_word leftSide, mem_word rightSide)
{
	mem_word tempLeft = (leftSide << 16) & 0xFFFF0000;
	mem_word tempRight = rightSide & 0xFFFF;
	
	printf("%d\n", tempLeft + tempRight);
	
	insertWordIntoByteArray(array, location, tempLeft + tempRight);
}

