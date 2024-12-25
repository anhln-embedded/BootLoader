/*
 * my_status.h
 *
 *  Created on: Dec 25, 2024
 *      Author: nvd
 */

#ifndef MY_STATUS_H_
#define MY_STATUS_H_
typedef enum {
    STATUS_ERROR = 0,
    STATUS_OK = 1,
    STATUS_BUSY,
    STATUS_TIMEOUT,
	STATUS_NULL
} status_t;



#endif /* MY_STATUS_H_ */
