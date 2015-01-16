#ifndef _PID_H_
#define _PID_H_

/********位置式PID********/
typedef struct _PID
{
  int  SetPoint;                                                         //  设定目标 Desired Value
  int  Proportional;                                                       //  比例常数 Proportional Const
  int  Integral;                                                         //  积分常数 Integral Const
  int  Derivative;                                                       //  微分常数 Derivative Const
  int  LastError;                                                        //  Error[-1]
  int SumError;                                                    //  Sums of Errors
	int MaxSumError;// Should be a positive number
	int Output;
	int MaxOutput;// Should be a positive number
}iPidObj;
/********增量式PID********/
typedef struct _QPID
{
	int  Proportional;                                                       //  比例常数 Proportional Const
  int  Integral;                                                         //  积分常数 Integral Const
  int  Derivative;                                                       //  微分常数 Derivative Const
  int  LastError;                                                        //  Error[-1]
  int  PrevError;                                                        //  Error[-2]
}iQPidObj;



/********位置环PD********/
typedef struct _pPD
{
  int  SetPoint;                                                         //  设定目标 Desired Value
  int  Proportional;                                                       //  比例常数 Proportional Const
  int  Derivative;                                                       //  微分常数 Derivative Const
  int  LastError;                                                        //  Error[-1]
	int Output;
	int MaxOutput;// Should be a positive number
}pPDObj;






/***********************************************声明外部函数*********************************************/

void iPID_Init (iPidObj *pid, int SetPoint,int Proportion,int Integral,int Derivative);
void iQPID_Init (iQPidObj *pid,int Proportion,int Integral,int Derivative);
int iPID_Calc(iPidObj *pid, int NextPoint );
int iQPID_Calc(iQPidObj *pid,int Error);
int pPD_Calc(pPDObj *pid, int feedback);
#endif
