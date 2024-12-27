/*
 ******************************************************************************
 * @file		parsing_srec.h                                                     *
 * @author	    Luu Ngoc Anh                                                       *
 * @date		14/10/2024                                                         *
 ******************************************************************************
 */

#ifndef __PARSING_SREC_H__
#define __PARSING_SREC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_LINE_LENGTH 256
#define HEX_INVALID 16

    /**
     * Structure to hold the information of a parsed S-record line.
     */
    typedef struct
    {
        uint8_t u8SrecType;  /** S-record type */
        uint8_t u8ByteCount; /** Byte count in the line */
        uint32_t u32Address; /** Address field */
        uint8_t *pData;      /** Pointer to data */
        uint8_t u8DataLen;   /** Length of the data */
        uint8_t u8Checksum;  /** Checksum value */
    } SrecLine_t;

    /**
     * Enumeration to represent different types of errors during S-record parsing.
     */
    typedef enum
    {
        SREC_ERROR_NOERR,            /** No error */
        SREC_ERROR_OPENFILE,         /** File opening error */
        SREC_ERROR_CHECKSUM,         /** Checksum error */
        SREC_ERROR_REDUNDANT,        /** Redundant data error */
        SREC_ERROR_SYNTAX,           /** Syntax error */
        SREC_ERROR_NOT_AVAILABLE_S4, /** S4 type is not available */
    } ErrorCode_t;

    /*******************************************************************************
     * API
     ******************************************************************************/

    ErrorCode_t SrecReadLine(char *line, SrecLine_t *Data);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
