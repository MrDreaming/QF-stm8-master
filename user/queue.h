#ifndef __QUEUE_H
#define __QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "stm8s.h"
#define QUEUE_TYPE      uint8_t 
#define	MASSAGE_MAX     16
#define	MASSAGE_MASK	(MASSAGE_MAX - 1)

bool InsertMsgQueue(QUEUE_TYPE value);
uint16_t GetMsgQueue(QUEUE_TYPE *value);
#endif