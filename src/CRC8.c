#include "CRC8.h"

uint8_t CRC8(void* p, uint8_t size)
{
    uint8_t crc = 0;
    const uint8_t generator = 0xD5;//Polynomial

    uint8_t* arr = (uint8_t*)p;

    for (int j = 0; j < size; j++)
    {
        crc ^= arr[j];

        //For each bit of the byte check for the MSB bit, if it is 1 then left shift the CRC and XOR with the polynomial
        //otherwise just left shift the variable
        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x80) != 0)
            {
                crc = (uint8_t)((crc << 1 ^ generator));
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}