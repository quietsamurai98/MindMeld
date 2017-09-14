/*
 *  This is an interpreter for MindMeld, a BrainFuck derivative that uses two data pointers
 *  Syntax:
 *  Pointer movement:
 *      >A  Move data pointer A forward
 *      >B  Move data pointer B forward
 *      <A  Move data pointer A backward
 *      <A  Move data pointer B backward
 *  Data modification:
 *      +A  Increment the byte that A points at by 1
 *      +B  Increment the byte that B points at by 1
 *      -A  Decrement the byte that A points at by 1
 *      -B  Decrement the byte that B points at by 1
 *  Console IO:
 *      .A  Output the byte that A points at to the console as an ASCII encoded char
 *      .B  Output the byte that B points at to the console as an ASCII encoded char
 *      ,A  Get a char from the console, and set the byte that A points at to the char's ASCII value
 *      ,B  Get a char from the console, and set the byte that B points at to the char's ASCII value
 *  Control Flow:
 *      [A  If the byte that A points to equals 0b00000000,
 *              move the instruction pointer forward to the matching close bracket (Either ]A or ]B is acceptable)
 *              Otherwise, move the instruction pointer to the next instruction
 *      [B  If the byte that B points to equals 0b00000000,
 *              move the instruction pointer forward to the matching close bracket (Either ]A or ]B is acceptable)
 *              Otherwise, move the instruction pointer forward to the next instruction
 *      ]A  If the byte that A points to does NOT equal 0b00000000,
 *              move the instruction pointer backward to the matching open bracket (Either [A or [B is acceptable)
 *              Otherwise, move the instruction pointer forward to the next instruction
 *      ]B  If the byte that A points to does NOT equal 0b00000000,
 *              move the instruction pointer backward to the matching open bracket (Either [A or [B is acceptable)
 *              Otherwise, move the instruction pointer forward to the next instruction
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <conio.h>

//Pre-processing functions
char* source_read(const char* filename);
char* source_sanitize(char *source, bool is_hex);

//Execution function
void execute(char *instructions);

int main()
{
    std::cout << "Enter a path to a MindMeld source file: ";
    std::string path;
    getline(std::cin, path);
    char *raw_source = source_read(path.c_str());
    if(raw_source==nullptr){
        std::cout << std::endl << "Could not read the file \""<<path<<"\". Are you sure it exists?" << std::endl;
        std::cout << "Press any key to continue..." << std::endl;
        getch();
        return 1;
    }
    bool is_hex = path.substr(path.rfind(".") + 1)=="hmm";
    char *instructions=source_sanitize(raw_source, is_hex);
    printf("%s\n", instructions);
    execute(instructions);
    std::cout << std::endl << "Press any key to continue..." << std::endl;
    getch();
    return 0;
}

//Reads the given file, sanitizes it, and returns a cstring of instruction characters
char* source_read(const char* filename)
{
    std::fstream source;
    source.open(filename, source.in);
    if(source){
        //Get the number of chars in the file
        source.seekg (0, source.end);
        long long int length = source.tellg();
        source.seekg (0, source.beg);

        //Read the file into a char array
        char *contents = new char[length+1];
        source.read(contents, length);
        contents[strlen(contents)]='\0';
        return contents;
    } else {
        return nullptr;
    }
}

//Remove invalid characters from the raw source
//TODO: Add in removing comments marked with /*example comment*/, so you can have comments with MM command characters.
char* source_sanitize(char *source, bool is_hex)
{
    //replace hex commands with MM commands
    if(is_hex){
        std::string source_str(source);
        std::string hex[16] = {"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};                    //Hex codes
        std::string mmc[16] = {">A",">B","<A","<B","+A","+B","-A","-B",".A",".B",",A",",B","[A","[B","]A","]B"};    //MindMeld commands
        for(int i=0; i<16;i++){
            std::string::size_type n = 0;
            std::string f = hex[i]; //find
            std::string r = mmc[i]; //replace
            while ( ( n = source_str.find( f, n ) ) != std::string::npos )
            {
                source_str.replace( n, f.size(), r );
                n += r.size();
            }
        }
        source_str+="\0";
        delete source;
        source = new char[strlen(source_str.c_str())+1];
        strcpy(source, source_str.c_str());
    }
    long long int valid_count = 0; //Interpreter can hypothetically handle up to 16 exbibytes worth of executable code.

    //Count the number of valid characters in the source file
    char *source_i = source; //Iterator pointer for raw source code
    while(*source_i){
        switch(*source_i){
            case 'A':
            case 'B':
            case '<':
            case '>':
            case '-':
            case '+':
            case '.':
            case ',':
            case '[':
            case ']':
                valid_count++;
        }
        source_i++;
    }

    char *instructions = new char[valid_count+1](); //Create a char array to hold the instructions, plus a null terminator
    char *instructions_i = instructions; //Iterator pointer for sanitized instructions
    source_i = source; //Point the iterator back at the start

    while(*source_i){
        switch(*source_i){ //Add acceptable characters to the instruction string
            case 'A':
            case 'B':
            case '<':
            case '>':
            case '-':
            case '+':
            case '.':
            case ',':
            case '[':
            case ']':
                *(instructions_i++) = *source_i;
        }
        source_i++;
    }

    *(instructions_i++) = '\0'; //Add a null terminator

    return instructions;
}

//Interpret and execute the MM code.
void execute(char *instructions)
{
    char *instruction_pointer;      //Keeps track of the interpreter's position in the program.
    char *data_tape;                //Stores the data used by the program. Basically, it's RAM.
    char *data_pointer_A;           //Used to modify/read cells on the data_tape. Controllable.
    char *data_pointer_B;           //Used to modify/read cells on the data_tape. Controllable.


    instruction_pointer = instructions;
    data_tape = new char[30000](); //BrainFuck interpreters conventionally have a 30000 byte memory block.
    data_pointer_A = data_tape;
    data_pointer_B = data_tape;


    while(*instruction_pointer) //Until the instruction pointer reaches the end of the instructions
    {
        char **data_ptr; //Holds the address of the pointer specified by the command.
        if(instruction_pointer[1]=='A'){
            data_ptr = &data_pointer_A;
        }
        if(instruction_pointer[1]=='B'){
            data_ptr = &data_pointer_B;
        }

        //Execute the appropriate instruction, using the appropriate data_pointer.
        if(*instruction_pointer=='+'){
            ++**data_ptr;               //Increment value at data_ptr
        } else if(*instruction_pointer=='-'){
            --**data_ptr;               //Decrement value at data_ptr
        } else if(*instruction_pointer=='>'){
            ++*data_ptr;                //Move data_ptr forward by 1
        } else if(*instruction_pointer=='<'){
            --*data_ptr;                //Move data_ptr back by 1
        } else if(*instruction_pointer=='.'){
            printf("%c",**data_ptr);    //Print data_ptr's value
        } else if(*instruction_pointer==','){
            **data_ptr=getch();         //Set value of data_ptr to character from console
            printf("%c",**data_ptr);    //Display the char the user typed, as getch hides it.
        } else if(*instruction_pointer=='['){
            if(**data_ptr=='\0'){       //If the data_ptr is zero, skip to the matching close bracket
                int level = 1;
                while(level){
                    instruction_pointer+=2;
                    if (*instruction_pointer == '[') {
                        level++;
                    } else
                    if (*instruction_pointer == ']') {
                        level--;
                    }
                }
            }                           //If the data_ptr isn't zero, let the interpreter enter the loop.
        } else if(*instruction_pointer==']'){
            if(**data_ptr!='\0'){       //If the data_ptr is not zero, jump back to the matching open bracket
                int level = 1;
                while(level){
                    instruction_pointer-=2;
                    if (*instruction_pointer == '[') {
                        level--;
                    } else
                    if (*instruction_pointer == ']') {
                        level++;
                    }
                }
                instruction_pointer-=2;
            }                           //If the data_ptr isn't zero, let the interpreter exit the loop.

        }
        instruction_pointer+=2;         //Move to the next instruction

        long long int n = instruction_pointer-instructions;
        n/=2;    //Debug.
    }
}
