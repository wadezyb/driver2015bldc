#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_
void calibrateRotorZeroPos( void );
void EncCalibrate( void );
void calibrationTask( void *pvParameters );
void CurrentOffsetCalibration( void );
#endif
