#include "frixia_fins_message.h"

#include "frixia_fins.h"

int parse_fins_message(char *s, size_t buffer_length, fins_message_t *message)
{
    printf("Parsing FINS message: %d bytes\n",buffer_length);
    if ( buffer_length % 2 == 1)
    {
        fprintf(stderr, "Error: Buffer odd length for FINS message\n");
        return -1;
    }
    
    const uint8_t *buffer = (const uint8_t *)s;
    if (buffer_length < FINS_HEADER_SIZE)
    {
        fprintf(stderr, "Error: Buffer too small for FINS header\n");
        return -1;
    }

    message->ICF = buffer[0];
    message->RSV = buffer[1];
    message->GCT = buffer[2];
    message->DNA = buffer[3];
    message->DA1 = buffer[4];
    message->DA2 = buffer[5];
    message->SNA = buffer[6];
    message->SA1 = buffer[7];
    message->SA2 = buffer[8];
    message->SID = buffer[9];

    message->payload_length = buffer_length - FINS_HEADER_SIZE;
    if (message->payload_length > MAX_PAYLOAD_SIZE)
    {
        fprintf(stderr, "Error: Payload too large, increment MAX_PAYLOAD_SIZE variable\n");
        return -1;
    }

    // Copy the payload
    memcpy(message->payload, buffer + FINS_HEADER_SIZE, message->payload_length);

    return 0;
}
