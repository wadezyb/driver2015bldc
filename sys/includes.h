//#define DEBUG_MODE
#define CAN_NODE1 (0X01)
#define CAN_NODE2 (0X02)
#define CAN_NODE3 (0X03)
#define CAN_NODE4 (0X04)
#define CAN_NODE_MASTER (0X05)
#define CAN_NODE CAN_NODE2

/*standard lib*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


