#ifndef _CIRCULAR_QUEUE_H_
#define _CIRCULAR_QUEUE_H_
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_SIZE 128
typedef struct {
    uint8_t buffer[QUEUE_SIZE];
    volatile uint16_t head;  
    volatile uint16_t tail;  
} CircularQueue_t;

void CircularQueue_Init(CircularQueue_t *queue);
bool CircularQueue_IsFull(const CircularQueue_t *queue);
bool CircularQueue_IsEmpty(const CircularQueue_t *queue);
bool CircularQueue_Enqueue(CircularQueue_t *queue, uint8_t data);
bool CircularQueue_Dequeue(CircularQueue_t *queue, uint8_t *data);
void CircularQueue_Free(CircularQueue_t *queue);
uint16_t CircularQueue_Size(CircularQueue_t *queue);
void CircularQueue_GetLine(CircularQueue_t *queue, uint8_t *line);

#endif