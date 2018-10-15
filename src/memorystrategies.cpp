#include "memorystrategies.h"
#include "types.h"
#include "constants.h"
#include "memory.h"

int selectedMemoryStrategy = 0;

// using the first chunk with sufficient size
uint32_t* firstFit(int blockSize) {
    byte* start = (byte*)HEAP_START;
    while (start <= (byte*)MEMORY_TABLE_END) {
        if (!isMemoryUsed(start)) {
            if (blockSize == 1) {
                return getMemoryPointerFromMemoryTablePointer(start);
            }

            byte* end = start + 1;
            do {
                if (isMemoryUsed(end) || end > (byte*)MEMORY_TABLE_END) {
                    break;
                }
                end++;
            } while ((end - start) < blockSize);
            // start position is available
            if (end - start == blockSize) {
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

MemoryStrategy memoryStrategies[1] = {firstFit}; 

MemoryStrategy getSelectedMemoryStrategy() {
    return memoryStrategies[selectedMemoryStrategy];
}