#include "program.h"
#include "types.h"
#include "list.h"
#include "memory.h"
#include "programs.h"
#include "io.h"
#include "utils.h"
#include "constants.h"

List* programList;

Program* initProgram(uint32_t programId, ProgramType programType, ProgramFunction programFunction) {
    enterCritical();
    Program* program = (Program*)malloc(sizeof(Program)/4);
    program->programId = programId;
    program->programType = programType;
    program->programFunction = programFunction;
    leaveCritical();
    return program;
}

void initPrograms() {
    enterCritical();
    programList = createList();
    append(programList, initProgram(1, AUTOSTART, program1Function));
    append(programList, initProgram(2, AUTOSTART, program2Function));
    append(programList, initProgram(3, AUTOSTART, program3Function));
    leaveCritical();
}

List* getProgramList() {
    return programList;
}