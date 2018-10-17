#ifndef __IO__
#define __IO__
#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define BROWN 0x6
#define GREY 0x7
#define DARK_GREY 0x8
#define LIGHT_BLUE 0x9
#define LIGHT_GREEN 0xA
#define LIGHT_CYAN 0xB
#define LIGHT_RED 0xC
#define LIGHT_MAGENTA 0xD
#define LIGHT_BROWN 0xE
#define WHITE 0xF

#define VIDEO_POSITION 0xB8000
#define VGA_INDEX_REGISTER 0x03D4
#define VGA_DATA_REGISTER 0x03D5
#define VGA_CURSOR_LOW 0x0F
#define VGA_CURSOR_HIGH 0x0E

#define ROWS 25
#define COLUMNS 80

#define TAB_WIDTH 4

#include "types.h"
#include "string.h"

extern int row;
extern int column;

void clearScreen();
void print(
        const char* text,
        byte inTextColor = WHITE);
void print(
        char character,
        byte inTextColor = WHITE);
void print(
        byte number,
        byte inTextColor = WHITE);
void print(
        int number,
        byte inTextColor = WHITE);
void print(
        uint32_t number,
        byte inTextColor = WHITE);
void println(
        const char* text,
        byte inTextColor = WHITE);
void println(
        char character,
        byte inTextColor = WHITE);
void println(
        byte number,
        byte inTextColor = WHITE);
void println(
        int number,
        byte inTextColor = WHITE);
void println(
        uint32_t number,
        byte inTextColor = WHITE);
#endif