#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define PIN 12 



int main()
{

  if (wiringPiSetupPhys() == -1) 
     exit (1) ;
  
  pinMode(PIN, PWM_OUTPUT);

  pwmSetMode(PWM_MODE_MS); 

  pwmSetClock(384);
  pwmSetRange(1000);

  float val;

  while(-1){
  
    printf("(30~75~120)= ");
    scanf("%f", &val);
    
    if ( val == -1 ) break;
    
    pwmWrite(PIN, val );
    sleep(1);
  }
  
  return 0 ;
}
