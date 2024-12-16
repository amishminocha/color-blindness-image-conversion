#include "io.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct buffer {
    int fd;
    int offset;
    int num_remaining;
    uint8_t a[BUFFER_SIZE];
};

Buffer *read_open(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    Buffer *buf = calloc(1, sizeof(Buffer));
    buf->fd = fd;
    buf->offset = 0;
    buf->num_remaining = 0;
    return buf;
}

void read_close(Buffer **pbuf) {
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

Buffer *write_open(const char *filename) {
    int fd = creat(filename, 0664);
    if (fd < 0) {
        return NULL;
    }
    Buffer *buf = calloc(1, sizeof(Buffer));
    buf->fd = fd;
    buf->offset = 0;
    buf->num_remaining = 0;
    return buf;
}

void write_close(Buffer **pbuf) {
    if ((*pbuf)->offset == BUFFER_SIZE) {
        uint8_t *start = (*pbuf)->a;
        int num_bytes = (*pbuf)->offset;
        do {
            ssize_t rc = write((*pbuf)->fd, start, num_bytes);
            start += rc;
            num_bytes -= rc;
        } while (num_bytes > 0);
        (*pbuf)->offset = 0;
    }
    write((*pbuf)->fd, (*pbuf)->a, (*pbuf)->offset);
    close((*pbuf)->fd);
    free(*pbuf);
    *pbuf = NULL;
}

bool read_uint8(Buffer *buf, uint8_t *x) {
    if (buf->num_remaining == 0) {
        ssize_t rc = read(buf->fd, buf->a, sizeof(buf->a));
        if (rc < 0) {
            printf("Error reading!\n");
            exit(1);
        }
        if (rc == 0)
            return false;
        buf->num_remaining = rc;
        buf->offset = 0;
    }
    *x = buf->a[buf->offset];
    (buf->num_remaining) -= 1;
    (buf->offset) += 1;
    return true;
}

bool read_uint16(Buffer *buf, uint16_t *x) {
    uint8_t a;
    uint8_t b;
    uint16_t d;
    if (read_uint8(buf, &a) == false) {
        return false;
    }
    if (read_uint8(buf, &b) == false) {
        return false;
    }
    d = b;
    d = d << 8;
    d = d | a;
    *x = d;
    return true;
}

bool read_uint32(Buffer *buf, uint32_t *x) {
    uint16_t a;
    uint16_t b;
    uint32_t d;
    if (read_uint16(buf, &a) == false) {
        return false;
    }
    if (read_uint16(buf, &b) == false) {
        return false;
    }
    d = b;
    d = d << 16;
    d = d | a;
    *x = d;
    return true;
}

void write_uint8(Buffer *buf, uint8_t x) {
    if (buf->offset == BUFFER_SIZE) {
        uint8_t *start = buf->a;
        int num_bytes = buf->offset;

        do {
            ssize_t rc = write(buf->fd, start, num_bytes);
            if (rc < 0) {
                printf("Error writing!\n");
                exit(1);
            }
            start += rc;
            num_bytes -= rc;
        } while (num_bytes > 0);

        buf->offset = 0;
    }
    buf->a[buf->offset] = x;
    buf->offset += 1;
}

void write_uint16(Buffer *buf, uint16_t x) {
    write_uint8(buf, x);
    write_uint8(buf, x >> 8);
}

void write_uint32(Buffer *buf, uint32_t x) {
    write_uint16(buf, x);
    write_uint16(buf, x >> 16);
}
