#ifndef _CAN_H_
#define _CAN_H_

extern void CAN_Configuration(void);
extern void CAN_ErrRecConfig(void);
void CAN_send(int id,int index,int subindex, int param);
#endif


