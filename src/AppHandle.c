#include "AppHandle.h"

uint8_t BLE_Checksum(uint8_t *data, uint8_t size)
{
    uint16_t sum = 0U;
    uint8_t *end = data + size - 1U;
    uint8_t *ptr;

    for (ptr = data; ptr < end; ptr++)
    {
        sum += *ptr;
    }

    return (uint8_t)(sum & 0xFFU);
}
