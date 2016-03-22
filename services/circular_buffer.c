#include "circular_buffer.h"

/**
  * @brief  
  * @param  
  * @param  
  * @retval
  */
static uint8_t cirq_buffer_full( CircQueue* theq  )
{
    return ( ((theq->pWR + 1) % CIRCULAR_QUEUE) == theq->pRD  );
}

/**
  * @brief  
  * @param  
  * @param  
  * @retval
  */
static uint8_t cirq_buffer_empty( CircQueue* theq )
{
    return theq->pWR == theq->pRD;
}

/**
  * @brief  
  * @param  
  * @param  
  * @retval
  */
static uint8_t Enqueue( CircQueue* theq, uint8_t data)
{
    if ( cirq_buffer_full(theq) ){
        return 0; //no space availible.
    }
    else{
        theq->circqueue[theq->pWR] = data;
        theq->pWR = ((theq->pWR+1 )== CIRCULAR_QUEUE) ? 0 : theq->pWR+1;
    }
}

/**
  * @brief  
  * @param  
  * @param  
  * @retval
  */
static uint8_t Dequeue( CircQueue* theq, uint8_t* data)
{
    if (cirq_buffer_empty(theq)){
        return 0; //empty circbuffer
    }
    else{
        *data = theq->circqueue[theq->pRD];
        theq->pRD = ((theq->pRD+1)== CIRCULAR_QUEUE) ? 0 : theq->pRD+1;
    }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}