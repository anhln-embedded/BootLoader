#include "parsing_srec.h"
#include <stdio.h>
#define HEX_INVALID 16

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * Enumeration to represent the various states of S-record parsing.
 */
enum State
{
    SREC_READ_STYPE,     /** Reading the S-record type */
    SREC_READ_BYTECOUNT, /** Reading the byte count */
    SREC_READ_ADDRESS,   /** Reading the address */
    SREC_READ_DATA,      /** Reading the data */
    SREC_READ_CHECKSUM,  /** Reading the checksum */
    SREC_READ_ERROR,     /** Parsing error occurred */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/



/** Global state for S-record parsing */
enum State state = SREC_READ_STYPE;

/** Global error code */
ErrorCode_t errorCode = SREC_ERROR_NOERR;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint8_t ChartoHex(char c);
static uint8_t StringtoHex(const char *str, int length, uint32_t *result);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Parse a single S-record line.
 */
    ErrorCode_t SrecReadLine(const char *line, SrecLine_t *Output)
{
    uint32_t idx = 0;
    uint32_t sum = 0;
    uint8_t lenAddress;
    char *offset = line;
    state = SREC_READ_STYPE;
    errorCode = SREC_ERROR_NOERR;

    switch (state)
    {
    case SREC_READ_STYPE:
        /** Check if the first character is 'S' and followed by a valid digit */
        if (offset[0] != 'S' || !('0' <= offset[1] && offset[1] <= '9'))
        {
            errorCode = SREC_ERROR_SYNTAX;
            break;
        }
        else if (offset[1] == '4')
        {
            errorCode = SREC_ERROR_NOT_AVAILABLE_S4;
            break;
        }
        else
        {
            Output->u8SrecType = ChartoHex(offset[1]);
            offset += 2;
            state = SREC_READ_BYTECOUNT;
        }
    case SREC_READ_BYTECOUNT:
        /** Read byte count */
        if (!StringtoHex(offset, 2, &Output->u8ByteCount))
        {
            errorCode = SREC_ERROR_SYNTAX;
            break;
        }
        offset += 2;
        state = SREC_READ_ADDRESS;
    case SREC_READ_ADDRESS:
        /** Determine address length based on S-record type */
        switch (Output->u8SrecType)
        {
        case 0:
            lenAddress = 4;
            break;
        case 1:
        case 2:
        case 3:
            lenAddress = (Output->u8SrecType + 1) * 2;
            break;
        case 7:
            lenAddress = 8;
            break;
        case 8:
            lenAddress = 6;
            break;
        case 9:
            lenAddress = 4;
            break;
        default:
            break;
        }
        /** Read the address field */
        if (!StringtoHex(offset, lenAddress, &Output->u32Address))
        {
            errorCode = SREC_ERROR_SYNTAX;
            break;
        }
        else
        {
            offset += lenAddress;
            state = SREC_READ_DATA;
        }
    case SREC_READ_DATA:
        /** Calculate data length and allocate memory */
        Output->u8DataLen = Output->u8ByteCount - lenAddress / 2 - 1;
        free(Output->pData);
        Output->pData = (uint8_t *)malloc(Output->u8DataLen);
        for (idx = 0; idx < Output->u8DataLen; idx++)
        {
            if (!StringtoHex(offset, 2, &Output->pData[idx]))
            {
                errorCode = SREC_ERROR_SYNTAX;
                break;
            }
            else
            {
                offset += 2;
                sum += Output->pData[idx];
            }
        }
        state = SREC_READ_CHECKSUM;
    case SREC_READ_CHECKSUM:
        /** Read checksum and validate */
        if (!StringtoHex(offset, 2, &Output->u8Checksum))
        {
            errorCode = SREC_ERROR_SYNTAX;
            break;
        }
        else
        {
            sum += Output->u8ByteCount;
            sum += Output->u32Address & 0xFF;
            sum += (Output->u32Address >> 8) & 0xFF;
            sum += (Output->u32Address >> 16) & 0xFF;
            sum += (Output->u32Address >> 24) & 0xFF;
            if (0xff - (sum & 0xff) != Output->u8Checksum)
            {
                errorCode = SREC_ERROR_CHECKSUM;
            }
            offset += 2;
            if (offset[0] != '\0' && offset[0] != '\r' && offset[0] != '\n' && offset[0] != EOF)
            {
                errorCode = SREC_ERROR_REDUNDANT;
            }
            break;
        }
    default:
        break;
    }

    return errorCode;
}

/**
 * Convert a single character to its hexadecimal value.
 */
static uint8_t ChartoHex(char c)
{
    uint8_t hexOutput = HEX_INVALID;
    if ('0' <= c && c <= '9')
    {
        hexOutput = c - '0';
    }
    else if ('A' <= c && c <= 'F')
    {
        hexOutput = c - 'A' + 10;
    }
    if ('a' <= c && c <= 'f')
    {
        hexOutput = c - 'a' + 10;
    }
    return hexOutput;
}

/**
 * Convert a string to a hexadecimal value.
 */
static uint8_t StringtoHex(const char *str, int length, uint32_t *result)
{
    uint8_t status = 1;
    int i;
    *result = 0;
    for (i = 0; i < length; i++)
    {
        uint8_t hex = ChartoHex(str[i]);
        if (HEX_INVALID == hex)
        {
            status = 0;
            break;
        }
        else
        {
            *result = (*result << 4) | hex;
        }
    }
    return status;
}
