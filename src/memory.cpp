#include "memory.h"
#include "types.h"
#include "utils.h"
#include "constants.h"
#include "io.h"
#include "processmanager.h"
#include "memorystrategies.h"

void initMemory() {
    // clear all bytes in memory table
    memset((byte*)HEAP_START, 0, MEMORY_TABLE_END - HEAP_START);
}

// will use last 5 bit of processId
byte getMemoryTableEntry(bool used, bool shared, bool isLastByte, byte processId) {
    byte memoryTableEntry = 0;
    memoryTableEntry |= used << 7;
    memoryTableEntry |= shared << 6;
    memoryTableEntry |= isLastByte << 5;
    memoryTableEntry |= processId & 0b00011111;
    return memoryTableEntry;
}

bool isMemoryUsed(byte* memoryTableEntry) {
    return ((*memoryTableEntry & 0b10000000) >> 7);
}

bool isMemoryShared(byte* memoryTableEntry) {
    return ((*memoryTableEntry & 0b01000000) >> 6);
}

bool isLastByte(byte* memoryTableEntry) {
    return ((*memoryTableEntry & 0b00100000) >> 5);
}

byte getProcessId(byte* memoryTableEntry) {
    return (*memoryTableEntry & 0b00011111);
}

uint32_t* getMemoryPointerFromMemoryTablePointer(byte* memoryTableEntry) {
    uint32_t difference = (uint32_t)memoryTableEntry - HEAP_START;
    return (uint32_t*)((MEMORY_TABLE_END + 1) + sizeof(uint32_t) * difference);
}

byte* getMemoryTablePointerForMemoryPointer(uint32_t* memory) {
    uint32_t difference = (uint32_t)memory - (MEMORY_TABLE_END + 1);
    return (byte*)(HEAP_START + difference / sizeof(uint32_t));
}
 
void reserveMemory(uint32_t* pointer, bool shared, byte processId, int size) {
    enterCritical();
    byte* memoryTableEntry = getMemoryTablePointerForMemoryPointer(pointer);
    while (size-- > 0) {
        *memoryTableEntry++ = getMemoryTableEntry(1, shared, size == 0, processId);
    }
    leaveCritical();
}

uint32_t* malloc(int size) {
    enterCritical();
    if (size < 0) {
        // invalid input;
        leaveCritical();
        return 0;
    }

    uint32_t* pointer = getSelectedMemoryStrategy()(size);
    if (pointer == 0) {
        // no memory chunk found
        leaveCritical();
        return 0;
    }
    
    reserveMemory(pointer, false, getCurrentProcessId(), size);
    leaveCritical();
    return pointer;
}

void free(uint32_t* pointer) {
    enterCritical();
    byte* memoryTableEntry = getMemoryTablePointerForMemoryPointer(pointer);
    byte processId = getProcessId(memoryTableEntry);

    if (!(processId == getCurrentProcessId() || processId == KERNEL_PROCESS_ID || isMemoryShared(memoryTableEntry))) {
        println("Not allowed: memory belongs to different user");
        leaveCritical();
        return;
    }
    
    // release
    bool isLast = false;
    while(isMemoryUsed(memoryTableEntry) && !isLast) {
        isLast = isLastByte(memoryTableEntry);
        // check if this is the last item of a memory chunk
        *memoryTableEntry++ = 0;
    }
    leaveCritical();
}