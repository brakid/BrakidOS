#ifndef __PROGRAM__
#define __PROGRAM__

#include "types.h"
#include "list.h"

#define AUTOSTART_NAME "Autostart"
#define MANUAL_NAME "Manual"

enum ProgramType {
    AUTOSTART, MANUAL
};

typedef void (*ProgramFunction)();

struct Program {
    uint32_t programId;
    ProgramType programType;
    ProgramFunction programFunction;
};

void initPrograms();

List* getProgramList();
Program* getIdleProgram();

#endif