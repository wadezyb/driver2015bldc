#ifndef _TEST_H_
#define _TEST_H_
void testLEDTask( void *pvParameters );
void testHALLTask( void *pvParameters );
void testPWMTask( void *pvParameters );
void testCANTask( void *pvParameters );
void testTEMPTask( void *pvParameters );
void testPWM1Task( void *pvParameters );
void testPWM2Task( void *pvParameters );
void testPWM3Task( void *pvParameters );
int FindMost(int number[20]);
int FindSecondMost(int number[20]);
void testENCODERTask(void * pvParameters);
void testSVPWMTask(void * pvParameters);
void testSVPWMTask2(void * pvParameters);
extern float w;
#endif
