#include "display.h"
#include "includes.h"
#include "sci.h"
#include "cli.h"
#include "current.h"
#include "dma.h"
#include "hall.h"
#include "foc.h"
void ReportMotion(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,
					int16_t hx,int16_t hy,int16_t hz)
{
 	unsigned int temp=0xaF+9;
	char ctemp;
	sciSendChar(0xa5,bMessage);
	sciSendChar(0x5a,bMessage);
	sciSendChar(14+8,bMessage);
	sciSendChar(0xA2,bMessage);

	if(ax<0)ax=32768-ax;
	ctemp=ax>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=ax;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(ay<0)ay=32768-ay;
	ctemp=ay>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=ay;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(az<0)az=32768-az;
	ctemp=az>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=az;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(gx<0)gx=32768-gx;
	ctemp=gx>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=gx;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(gy<0)gy=32768-gy;
	ctemp=gy>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=gy;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
//-------------------------
	if(gz<0)gz=32768-gz;
	ctemp=gz>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=gz;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(hx<0)hx=32768-hx;
	ctemp=hx>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=hx;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(hy<0)hy=32768-hy;
	ctemp=hy>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=hy;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	if(hz<0)hz=32768-hz;
	ctemp=hz>>8;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;
	ctemp=hz;
	sciSendChar(ctemp,bMessage);
	temp+=ctemp;

	sciSendChar(temp%256,bMessage);
	sciSendChar(0xaa,bMessage);
}

void displayTask( void *pvParameters )
{
	int t;
	for(;;)
	{
		vTaskDelay(20);
		t=t+1;
		ReportMotion(
		pwm,
		0,//hallValue*100,
		0,
		
		adc2,
		adc1,
		0,
		
#ifdef USE_FOC
		FOC.Id,
		FOC.Iq,
#else
		0,
		0,
#endif
		0
		);
	}
}

