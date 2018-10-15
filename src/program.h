#ifndef __PROGRAM__
#define __PROGRAM__

#include "types.h"
#include "list.h"

enum ProgramType {
    AUTOSTART, MANUAL
};

typedef void (*ProgramFunction)();

struct Program {
    uint32_t programId;
    ProgramType programType;
    ProgramFunction programFunction;
};

extern List* programList;

void initPrograms();

List* getProgramList();

#endif