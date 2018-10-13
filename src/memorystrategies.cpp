#include "memorystrategies.h"
#include "types.h"
#include "constants.h"
#include "memory.h"

int selected = 0;

uint32_t* findFirst(int size) {
    byte* start = (byte*)HEAP_START;
    while (start <= (byte*)MEMORY_TABLE_END) {
        if (!isMemoryUsed(start)) {
            if (size == 1) {
                return getMemoryPointerFromMemoryTablePointer(start);
            }

            byte* end = start + 1;
            do {
                if (isMemoryUsed(end) || end > (byte*)MEMORY_TABLE_END) {
                    break;
                }
                end++;
            } while ((end - start) < size);
            // start position is available
            if (end - start == size) {
                return getMemoryPointerFromMemoryTablePointer(start);
            }
            start = end;
        } else {
            start++;
        }
    }
    // nothing found
    return 0;
}

MemoryStrategy memoryStrategies[1] = {findFirst}; 

MemoryStrategy getSelectedMemoryStrategy() {
    return memoryStrategies[selected];
}