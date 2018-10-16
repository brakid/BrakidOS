#include "program.h"
#include "types.h"
#include "list.h"
#include "memory.h"
#include "programs.h"
#include "io.h"
#include "utils.h"
#include "constants.h"
#include "timer.h"

List* programList = 0;
Program* idleProgram = 0;

void idleFunction() {
    while(1);
}

Program* initProgram(uint32_t programId, ProgramType programType, ProgramFunction programFunction) {
    enterCritical();
    Program* program = (Program*)malloc(sizeof(Program));
    program->programId = programId;
    program->programType = programType;
    program->programFunction = programFunction;
    leaveCritical();
    return program;
}

void initPrograms() {
    enterCritical();
    idleProgram = initProgram(0, AUTOSTART, idleFunction);

    programList = createList();
    append(programList, initProgram(1, AUTOSTART, program1Function));
    append(programList, initProgram(2, AUTOSTART, program2Function));
    append(programList, initProgram(3, AUTOSTART, program3Function));
    leaveCritical();
}

List* getProgramList() {
    return programList;
}

Program* getIdleProgram() {
    return idleProgram;
}