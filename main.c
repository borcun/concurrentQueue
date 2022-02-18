#include <stdlib.h>
#include <pthread.h>
#include "cbuf.h"

#define DEFAULT_NUMBERING_OFF (0)
#define DEFAULT_ENQUEUE_COUNT (50)
#define DEFAULT_DEQUEUE_COUNT (50)

void test(void);
void usage(void);
void *enqueue(void *arg);
void *dequeue(void *arg);

int main(int argc, const char * argv[]) {
  pthread_t enqThread;
  pthread_t deqThread;
  int enqCount = DEFAULT_ENQUEUE_COUNT;
  int deqCount = DEFAULT_DEQUEUE_COUNT;

  // argument checking block
  if (1 != argc) {
    if (3 == argc) {
      enqCount = atoi(argv[1]);
      deqCount = atoi(argv[2]);

      if (enqCount < 0 || deqCount < 0) {
	printf("The count values must be positive\n");
	return -1;
      }
    }
    else {
      usage();
      return -1;
    }
  }

  cbuf_init();

  pthread_create(&enqThread, NULL, enqueue, &enqCount);
  pthread_create(&deqThread, NULL, dequeue, &deqCount);

  pthread_join(enqThread, NULL);
  pthread_join(deqThread, NULL);

  cbuf_print();
  cbuf_deinit();

  return 0;
}

void usage(void) {
  printf("./<executable> <enqueue count> <dequeue count>\n");
  printf("note: default counts are %d %d respectively\n",
	 DEFAULT_ENQUEUE_COUNT,
	 DEFAULT_DEQUEUE_COUNT);
  
  return;
}

void *enqueue(void *arg) {
  int enqCount = *((int *) arg);

  while (0 != enqCount) {
    cbuf_enqueue(DEFAULT_NUMBERING_OFF + enqCount);
    // printf is intentionally added since it makes a bit latency
    printf("+ENQUEUED (%d)\n", enqCount);
    --enqCount;
  }

  return NULL;
}

void *dequeue(void *arg) {
  int deqCount = *((int *) arg);
  int elem;

  while (0 != deqCount) {
    cbuf_dequeue(&elem);
    // printf is intentionally added since it makes a bit latency
    printf("-DEQUEUED (%d)\n", elem);
    --deqCount;
  }

  return NULL;
}

void test(void) {
  int i = 0;
  int data = 0;
    
  cbuf_init();

  printf("\nInitial queue content\n");
  cbuf_print();
  printf("\n");
    
  for (i = 0; i < DEFAULT_ENQUEUE_COUNT; ++i) {
    // 50 is arbitrary value, has no special meaning
    cbuf_enqueue(i + 50);
    printf("%02d enqueue (%02d): ", i + 50, i + 1);
    cbuf_print();
  }
    
  printf("\nQueue content\n");
  cbuf_print();
    
  printf("\nDequeueing...\n");
  for (i = 0; i < DEFAULT_DEQUEUE_COUNT; ++i) {
    cbuf_dequeue(&data);
    printf("%02d dequeue (%02d): ", i + 1, data);
    cbuf_print();
  }

  printf("\n\nQueue content\n");
  cbuf_print();
    
  return;
}
