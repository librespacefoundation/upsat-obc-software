/* 
 * File:   circular_buffer.h
 * Author: Apostolos D. Masiakos
 *
 * Created on March 12, 2016, 4:48 PM
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#define CIRCULAR_QUEUE 512

typedef struct{
    uint8_t pRD, pWR;
    uint8_t circqueue[CIRCULAR_QUEUE];
}CircQueue;


#endif /* CIRCULAR_BUFFER_H */

