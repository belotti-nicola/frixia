#ifndef FRIXIA_FINS_MESSAGE_H
#define FRIXIA_FINS_MESSAGE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_PAYLOAD_SIZE 1024
#define FINS_HEADER_SIZE 10

typedef struct
{
    uint8_t ICF;                       // Information Control Field
    uint8_t RSV;                       // Reserved
    uint8_t GCT;                       // Gateway Count
    uint8_t DNA;                       // Destination Network Address
    uint8_t DA1;                       // Destination Node Address
    uint8_t DA2;                       // Destination Unit Address
    uint8_t SNA;                       // Source Network Address
    uint8_t SA1;                       // Source Node Address
    uint8_t SA2;                       // Source Unit Address
    uint8_t SID;                       // Service ID
    uint8_t payload[MAX_PAYLOAD_SIZE]; // Message payload
    size_t payload_length;             // Length of the payload
} fins_message_t;

#endif