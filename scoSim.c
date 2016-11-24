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


int main()
{
	mem_addr program_top = MEM_SIZE / 2; //Where the program's dedicated area stops
	mem_addr memory_top = MEM_SIZE; //Where the dedicated data area stops
	int user_mode = 1;

	mem_addr load_counter;
	mem_addr var_counter;

	int is_loading_code = 0;
	char * fileName = argv[1]; //Argument for what file to run

							   //This area is what loads the file
	FILE *file;
	file = fopen(fileName, "r");
	//file = fopen("C:\\lab3c.s", "r");


	//Makes an array of variables that can be filled out
	struct variable variables[512];
	struct label_reference labelReferences[1024];
	struct label_definition labelDefinitions[512];

	if (file) {
		char file_line[255]; //Char array that will hold lines of code
		char * fgets_return;

		fgets_return = fgets(file_line, 255, file); //Gets first line

													//declare counters
		var_counter = 0;
		load_counter = 0;
		num_of_vars = 0;

		//Loop that gets each line of code from the file
		while (fgets_return)
		{
			mem_addr var_addr;
			char *var_name;

			char *trim_string = trimwhitespace(file_line); //Gets rid of spaces

														   //This is to take care of all empty lines
			if (trim_string[0] == 0)
			{
			}

			//if text section, change is_loading_code to true
			else if (strcmp(trim_string, ".text") == 0)
			{
				is_loading_code = 1;
			}
			else if (strcmp(trim_string, ".data") == 0)
			{
			}

			else if (is_loading_code == 1)
			{
				if (stringContains(trim_string, ':') != -1)
				{
					loadLabel(trim_string, labelReferences, labelDefinitions, memory, load_counter);
				}
				else
				{
					load_counter = parseInstructionString(trim_string, labelReferences, labelDefinitions, memory, load_counter, num_of_vars, variables);
				}

				if (load_counter > program_top)
				{
					printf("Out of program memory!");
				}

				if (var_counter + program_top > memory_top)
				{
					printf("Out of main memory!");
				}
			}

			//Loading data
			//This area will store the name, and the data in memory
			else if (is_loading_code == 0)
			{
				var_counter = parseDataString(trim_string, variables, var_counter, program_top, memory);
			}
			else
			{
				//todo IF be broke
			}
			fgets_return = fgets(file_line, 255, file); //Gets next line and repeat
		}

		end_of_program = load_counter;
		fclose(file);
	}
	else
	{
		//todo
		printf("Invalid file name.");
	}
}












////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int parseDataString(char * trim_string, struct variable * variables, int var_counter, int program_top, byte * memory)
{
	int string_counter = 0;
	char variable_name[512];
	do
	{
		variable_name[string_counter] = trim_string[string_counter];
		string_counter++;
	} while (trim_string[string_counter] != ':');
	variable_name[string_counter] = 0;
	strcpy(variables[num_of_vars].variable_name, variable_name);

	//increments to type
	do
	{
		string_counter++;
	} while (trim_string[string_counter] != '.');
	string_counter++;

	switch (trim_string[string_counter])
	{
		//handles words
	case 'w':
		if (trim_string[1 + string_counter] == 'o' &&
			trim_string[2 + string_counter] == 'r' &&
			trim_string[3 + string_counter] == 'd' &&
			isspace(trim_string[4 + string_counter]))
		{
			string_counter = string_counter + 4;
		}
		else
		{
			//printf("Instructions are invalid.");
			//return 0;
		}
		variables[num_of_vars].is_text = 0;
		variables[num_of_vars].variable_addr = var_counter + program_top;
		num_of_vars++;

		char *value_text = trimwhitespace(trim_string + string_counter);
		//trim_string = trimwhitespace(trim_string + string_counter);

		mem_word value = atoi(value_text);
		//mem_word value = atoi(trim_string); 

		insertWordIntoByteArray(memory, var_counter + program_top, value);
		var_counter = var_counter + 4;
		break;
		//Handles text
	case 'a':
		if (trim_string[1 + string_counter] == 's' &&
			trim_string[2 + string_counter] == 'c' &&
			trim_string[3 + string_counter] == 'i' &&
			trim_string[4 + string_counter] == 'i' &&
			trim_string[5 + string_counter] == 'z' &&
			isspace(trim_string[6 + string_counter]))
		{
			string_counter = string_counter + 6;
		}
		else
		{
			//printf("Instructions are invalid.");
			//return 0;
		}
		char *value_text_sys = trimwhitespace(trim_string + string_counter); //gets value of syscall

		variables[num_of_vars].is_text = 1;
		variables[num_of_vars].variable_addr = var_counter + program_top;
		num_of_vars++;

		int i = 1; //Starts at 1 to skip quotes
		while (value_text_sys[i] != '"')
		{
			// handles \n since it could not be handles the normal way
			if (value_text_sys[i] == '\\' && value_text_sys[i + 1] == 'n')
			{
				memory[program_top + var_counter] = '\n';
				i = i + 2;
				var_counter++;
			}
			else
			{
				memory[program_top + var_counter] = value_text_sys[i];
				i++;
				var_counter++;
			}
		}
		memory[program_top + var_counter] = 0; //Null terminates string
		var_counter++;
		break;
	case 's':
		if (trim_string[1 + string_counter] == 'p' &&
			trim_string[2 + string_counter] == 'a' &&
			trim_string[3 + string_counter] == 'c' &&
			trim_string[4 + string_counter] == 'e' &&
			isspace(trim_string[6 + string_counter]))
		{
			string_counter = string_counter + 5;
		}
		else
		{
			//printf("Instructions are invalid.");
			//return 0;
		}
		char *value_text_space = trimwhitespace(trim_string + string_counter); //gets value of syscall

		variables[num_of_vars].is_text = 1;
		variables[num_of_vars].variable_addr = var_counter + program_top;
		num_of_vars++;

		int length = atoi(value_text_space);

		var_counter += length;
		break;
	default:
		printf("One or more data types is invalid.");
		break;
	}

	return var_counter;
}

//Saves an address to memory and returns the location
mem_addr saveLabelAddressToMemory(char * label_name, struct label_reference * label_references, struct label_definition * label_definitions, byte * memory, mem_addr location)
{
	mem_word valueToSave = 0;
	int foundLabel = 0;
	int i = 0;
	while (i < labelCount)
	{
		if (stringEquals(label_name, label_definitions[i].label))
		{
			valueToSave = label_definitions[i].pc_addr;
			foundLabel = 1;
			break;
		}
		i++;
	}

	insertWordIntoByteArray(memory, location, valueToSave);

	if (!foundLabel) {
		strcpy(label_references[labelReferenceCount].label, label_name);
		label_references[labelReferenceCount].code_location = location;
		labelReferenceCount++;
	}
	return location + 4;
}

//Load label takes in a string and memory and finds the string adress in memory
void loadLabel(char * trim_string, struct label_reference label_references[], struct label_definition label_definitions[], byte * memory, int load_counter)
{
	char labelName[64];
	//strcpy(trim_string, labelName);
	strcpy(labelName, trim_string);

	int i = 0;
	while (labelName[i] != ':')
	{
		i++;
	}
	labelName[i] = 0;

	strcpy(label_definitions[labelCount].label, labelName);
	label_definitions[labelCount].pc_addr = load_counter;

	i = 0;
	while (i < labelReferenceCount)
	{
		if (stringEquals(label_references[i].label, label_definitions[labelCount].label))
		{
			insertWordIntoByteArray(memory, label_references[i].code_location, label_definitions[labelCount].pc_addr);
		}
		i++;
	}
	labelCount++;
}

int parseInstructionString(char * trim_string, struct label_reference label_references[], struct label_definition label_definitions[], byte * memory, int load_counter, int num_of_vars, struct variable * variables)
{
	char * var_name;
	mem_addr var_addr;

	if (stringBeginsWith(trim_string, "syscall"))
	{
		//Loads syscall as op code and increments. This is done in ever else if
		memory[load_counter] = syscall;
		load_counter++;
	}
	else if (stringBeginsWith(trim_string, "nop"))
	{
		//Loads syscall as op code and increments. This is done in ever else if
		memory[load_counter] = nope;
		load_counter++;
	}
	else if (stringBeginsWith(trim_string, "addi"))
	{
		memory[load_counter] = addi;
		load_counter++;

		char * operands = trimwhitespace(trim_string + 4);

		//Loads Registers
		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;

		int sourceRegister = readInRegister(&operands);
		memory[load_counter] = sourceRegister;
		load_counter++;

		int immediateValue = (int)operands[0] - '0';
		int i = 1;
		while (operands[i] != 0)
		{
			immediateValue = immediateValue * 10 + ((int)operands[i] - '0');
			i++;
		}
		//Immediate Value loaded to memory
		insertWordIntoByteArray(memory, load_counter, immediateValue);
		load_counter = load_counter + 4;
	}
	else if (stringBeginsWith(trim_string, "add"))
	{
		memory[load_counter] = add;
		load_counter++;

		char * operands = trimwhitespace(trim_string + 3);

		//Loads Registers
		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;

		int sourceRegister = readInRegister(&operands);
		memory[load_counter] = sourceRegister;
		load_counter++;

		int targetRegister = readInRegister(&operands);
		memory[load_counter] = targetRegister;
		load_counter++;
	}
	
	else if (stringBeginsWith(trim_string, "subi"))
	{
		memory[load_counter] = subi;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 4);

		//Registers
		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;

		int sourceRegister = readInRegister(&operands);
		memory[load_counter] = sourceRegister;
		load_counter++;

		int immediateValue = (int)operands[0] - '0';
		int i = 1;
		while (operands[i] != 0)
		{
			immediateValue = immediateValue * 10 + ((int)operands[i] - '0');
			i++;
		}
		//Loads immediate value
		insertWordIntoByteArray(memory, load_counter, immediateValue);
		load_counter += 4;

	}
	else if (stringBeginsWith(trim_string, "beqz"))
	{
		memory[load_counter] = beqz;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 4);
		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;
		//operands = trimwhitespace(operands + 1);

		//loads label address to memory
		saveLabelAddressToMemory(operands, label_references, label_definitions, memory, load_counter);
		load_counter += 4;
	}
	else if (stringBeginsWith(trim_string, "bge"))
	{
		memory[load_counter] = bge;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 3);

		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;
		int sourceRegister = readInRegister(&operands);
		memory[load_counter] = sourceRegister;
		load_counter++;

		//loads label address to memory
		saveLabelAddressToMemory(operands, label_references, label_definitions, memory, load_counter);
		load_counter += 4;
	}
	else if (stringBeginsWith(trim_string, "bne"))
	{
		memory[load_counter] = bne;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 3);
		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;

		int sourceRegister = readInRegister(&operands);
		memory[load_counter] = sourceRegister;
		load_counter++;

		//loads label address to memory
		saveLabelAddressToMemory(operands, label_references, label_definitions, memory, load_counter);
		load_counter += 4;
	}
	else if (stringBeginsWith(trim_string, "la"))
	{
		memory[load_counter] = la;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 2);
		int destinationRegister = readInRegister(&operands);

		memory[load_counter] = destinationRegister;
		load_counter++;

		int i = 0;
		mem_addr var_addr;
		while (i < num_of_vars)
		{
			if (strcmp(operands, variables[i].variable_name) == 0)
			{
				var_addr = variables[i].variable_addr;
				break;
			}
			i++;
		}
		if (i == num_of_vars)
		{
			printf("This variable has not been declared.");
		}

		//Loads address into memory
		insertWordIntoByteArray(memory, load_counter, var_addr);
		load_counter += 4;

	}
	else if (stringBeginsWith(trim_string, "lb"))
	{
		memory[load_counter] = lb;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 2);

		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;
		//operands = trimwhitespace(operands + 1);

		//Detects numbers for immediate and loads to memory
		int i = 0;
		if (isdigit(operands[0]))
		{
			int offset = (int)operands[0] - '0';
			while (isdigit(operands[i]))
			{
				offset = offset * 10 + ((int)operands[i] - '0');
				i++;
			}
			insertWordIntoByteArray(memory, load_counter, offset);
		}
		else
		{
			insertWordIntoByteArray(memory, load_counter, 0);
		}
		load_counter += 4;
		operands = operands + i + 1;
		int sourceRegister = readInRegister(&operands);
		memory[load_counter] = sourceRegister;
		load_counter++;

	}
	else if (stringBeginsWith(trim_string, "li"))
	{
		memory[load_counter] = li;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 2);


		int destinationRegister = readInRegister(&operands);
		memory[load_counter] = destinationRegister;
		load_counter++;

		int immediateValue = (int)operands[0] - '0';
		int i = 1;
		while (operands[i] != 0)
		{
			immediateValue = immediateValue * 10 + ((int)operands[i] - '0');
			i++;
		}
		insertWordIntoByteArray(memory, load_counter, immediateValue);
		load_counter += 4;

	}
	else if (stringBeginsWith(trim_string, "b"))
	{
		memory[load_counter] = b;
		load_counter++;
		char * operands = trimwhitespace(trim_string + 1);


		saveLabelAddressToMemory(operands, label_references, label_definitions, memory, load_counter);
		load_counter += 4;
		//loadLabel(operands, label_references, label_definitions, memory, load_counter);
	}

	return load_counter;
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




//////////////////
//STRING METHODS//
//////////////////

//This code was found on stack overflow posted by Adam Rosenfield
//It gets rid of white space at beggining and end of string
//http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while (isspace(*str)) str++;

	if (*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end + 1) = 0;

	return str;
}

//Checks that two strings are equal
int stringEquals(char * string1, char * string2)
{
	int i = 0;
	do {
		if (string1[i] != string2[i])
		{
			return 0;
		}
		i++;
	} while (!(string1[i] == 0 || string2[i] == 0));
	return 1;
}

//Checks if a string begins with another string
int stringBeginsWith(char * trim_string, char * string_to_compare)
{
	int compare_string_length = strlen(string_to_compare);
	int i = 0;
	while (i < compare_string_length)
	{
		if (trim_string[i] == 0 || string_to_compare[i] != trim_string[i])
		{
			return 0;
		}
		i++;
	}
	return 1;
}


//Sees if a character is contained within a string
int stringContains(char * string, char query)
{
	char thisChar = string[0];
	int i = 0;
	while (thisChar != 0)
	{
		if (thisChar == query)
		{
			return i;
		}
		i++;
		thisChar = string[i];
	}
	return -1;
}

