// Header file for CSE13s Section 1 asgn5
// io.h
// Made by Dr. Kerry Veenstra
// DO NOT modify this file.

#include <inttypes.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096

typedef struct buffer Buffer;

Buffer *read_open(const char *filename);
void read_close(Buffer **pbuf);
bool read_uint8(Buffer *buf, uint8_t *x);
bool read_uint16(Buffer *buf, uint16_t *x);
bool read_uint32(Buffer *buf, uint32_t *x);

Buffer *write_open(const char *filename);
void write_close(Buffer **pbuf);
void write_uint8(Buffer *buf, uint8_t x);
void write_uint16(Buffer *buf, uint16_t x);
void write_uint32(Buffer *buf, uint32_t x);
