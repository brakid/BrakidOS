#include "program.h"
#include "types.h"
#include "list.h"
#include "memory.h"
#include "programs.h"
#include "io.h"

Node<Program>* programs;

Node<Program>* initProgram(uint32_t programId, ProgramType programType, ProgramFunction programFunction) {
    Program* program = (Program*)malloc(sizeof(Program)/4);
    program->programId = programId;
    program->programType = programType;
    program->programFunction = programFunction;

    Node<Program>* programNode = (Node<Program>*)malloc(sizeof(Node<Program>)/4);
    programNode->value = program;
    programNode->next = 0;

    return programNode;
}

void initPrograms() {
    Node<Program>* program1Node = initProgram(1, AUTOSTART, program1Function);
    Node<Program>* program2Node = initProgram(2, AUTOSTART, program2Function);
    Node<Program>* program3Node = initProgram(2, AUTOSTART, program3Function);
    program1Node->next = program2Node;
    program2Node->next = program3Node;
    program3Node->next = 0;

    programs = program1Node;
}

Node<Program>* getPrograms() {
    return programs;
}