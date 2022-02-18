#include "cbuf.h"
#include <string.h>
#include <pthread.h>

/**
 * @struct CircularBuffer that is internal structure for circular buffer definition
 */
typedef struct {
  //! data of circular buffer
  int data[CBUF_DEFAULT_BUF_SIZE];

  //! head index
  int head;

  //! tail index
  int tail;

} CircularBuffer;

//! private instance of circular buffer
static CircularBuffer cBuf;

//! mutex to prevent parallel enqueue and dequeue operations
static pthread_mutex_t cbMutex;

int cbuf_init(void) {
  int err = 0;
  
  if (0 != (err = pthread_mutex_init(&cbMutex, NULL))) {
    perror("circular buffer mutex");
    return err;
  }
  
  /*
   * set all fields of circular buffer, even data list
   * if dequeuing is done more than enqueuing in initial data list,
   * user sees default element of the list, meaning 0
   */
  memset(&cBuf, 0x00, sizeof(CircularBuffer));

  return 0;
}

int cbuf_deinit(void) {
  memset(&cBuf, 0x00, sizeof(CircularBuffer));
  
  return pthread_mutex_destroy(&cbMutex);
}

int cbuf_enqueue(int elem) {
  int err = 0;
  
  if (0 != (err = pthread_mutex_lock(&cbMutex))) {
    perror("enqueue lock");
    return err;
  }

  // add location is tail
  cBuf.data[cBuf.tail] = elem;
  cBuf.tail++;
  // circulate tail if exceed
  cBuf.tail %= CBUF_DEFAULT_BUF_SIZE;

  if (0 != (err = pthread_mutex_unlock(&cbMutex))) {
    perror("enqueue unlock");
    return err;
  }
  
  return 0;
}

int cbuf_dequeue(int *elem) {
  int err = 0;

  if (NULL == elem) {
    return -1;
  }
  
  if (0 != (err = pthread_mutex_lock(&cbMutex))) {
    perror("dequeue lock");
    return err;
  }

  // get element from head index
  *elem = cBuf.data[cBuf.head];
  cBuf.head++;
  // circulate head if exceed
  cBuf.head %= CBUF_DEFAULT_BUF_SIZE;
  
  if (0 != (err = pthread_mutex_unlock(&cbMutex))) {
    perror("dequeue lock");
    return err;
  }

  return 0;
}

void cbuf_print(void) {
  int i = 0;
        
  for (i = 0; i < CBUF_DEFAULT_BUF_SIZE; ++i) {
    if (i == cBuf.head - 1 && i == cBuf.tail - 1) {
      printf("HT>%d ", cBuf.data[i]);
    }
    else if (i == cBuf.head - 1) {
      printf("H>%d ", cBuf.data[i]);
    }
    else if (i == cBuf.tail - 1) {
      printf("T>%d ", cBuf.data[i]);
    }
    else {
      printf("%d ", cBuf.data[i]);
    }
  }

  printf("\n");
    
  return;
}
