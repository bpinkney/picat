//gcc -Wall -o test servo_feed.c -lwiringPi -lpthread

//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <softPwm.h>


#define LED_BLUE_PIN    (17)
#define LED_RED_PIN     (22)
#define LED_GREEN_PIN   (27)

// continuous rotation
#define FEED_POS_FW4  (140)
#define FEED_POS_FW3  (141)
#define FEED_POS_FW2  (142)
#define FEED_POS_FW1  (143)
#define FEED_POS_STOP (150)
#define FEED_POS_BK1  (157)
#define FEED_POS_BK2  (158)
#define FEED_POS_BK3  (159)
#define FEED_POS_BK4  (160)

void init_leds()
{
   softPwmCreate(LED_BLUE_PIN,   100, 100);
   softPwmCreate(LED_RED_PIN,    100, 100);
   softPwmCreate(LED_GREEN_PIN,  100, 100);
}

void led_enable(int enable, int pin)
{
   if(enable)
      softPwmWrite(pin, 99);
   else
      softPwmWrite(pin, 100);
}

void init_pwm()
{
   pinMode(18, PWM_OUTPUT);
   pwmSetMode(PWM_MODE_MS);
   pwmSetRange(2000);
   pwmSetClock(192);
   //192 and 2000 results in 50Hz servo freq (valid for the DS04-nfc)
}

void disable_pwm()
{
   pinMode(18, INPUT);
}

void busy_status(int delay_time)
{
   int delay_count = 0;
   int delay_cycle = 0;
   const int delay_step  = 5;
   int switch_bool = 0;
   int switch_time = 200;//ms
   while(delay_count < delay_time)
   {
      if(switch_bool)
      {
         led_enable(1, LED_RED_PIN);
         led_enable(0, LED_GREEN_PIN);
         led_enable(0, LED_BLUE_PIN);
      }  
      else
      {
         led_enable(0, LED_RED_PIN);
         led_enable(0, LED_GREEN_PIN);
         led_enable(1, LED_BLUE_PIN);
      } 

      delay_cycle += delay_step;
      delay_count += delay_step;

      if(delay_cycle >= switch_time)
      {
         switch_bool = !switch_bool;
         delay_cycle = 0;
      }

      delay(delay_step);
   }
   led_enable(0, LED_RED_PIN);
   led_enable(1, LED_GREEN_PIN);
   led_enable(0, LED_BLUE_PIN);
}

int main(int argc, char **argv) 
{
 //char user_in[20];
 int portions = atoi(argv[1]);

 //printf ("Raspberry Pi wiringPi test program\n");
 wiringPiSetupGpio();

 init_leds();

 printf("Feed Portions [%d]\n", portions);

 //init_pwm();

 /*while(1)
 {
    printf("Enter PWM CMD then time: ");
    fgets(user_in,20,stdin);
    pwm_cmd = atoi(user_in);
    fgets(user_in,20,stdin);
    int pwm_time = atoi(user_in);

    init_pwm();
    delay(500);
    pwmWrite(18,pwm_cmd);
    delay(pwm_time); 
    pwmWrite(18,FEED_POS_STOP);   
    //delay(2000);  
    //disable_pwm();
    busy_status(1000);
 }*/

  init_pwm();
  delay(2000);

  int i;
  for (i = 0; i < portions*2; i++)
  {

    pwmWrite(18,140); //back the auger up a bit to load/clear
    delay(500);
    pwmWrite(18,FEED_POS_STOP);
    delay(500);
    pwmWrite(18,160); //dispense one portion
    delay(1000);
    pwmWrite(18,FEED_POS_STOP);
    busy_status(1000);
  }

/*   int feed_portions = atoi(argv[1]);
   printf("Feed Portions [%d]\n", feed_portions);

   int i = 0;
   int j = 0;
   for (i=0; i < feed_portions; i++)
   {
      //init_pwm();
      delay(500);
      int stagger = 10;
      for (j=0; j < stagger; j++)
      {
         pwmWrite(18,FEED_POS_FW4);
         delay(FEED_FW4_SINGLE_MS/stagger); 
         pwmWrite(18,FEED_POS_STOP);
         delay(50);
         pwmWrite(18,FEED_POS_BK4);
         delay(FEED_BK4_SINGLE_MS*2/stagger); 
         pwmWrite(18,FEED_POS_STOP); 
      } 
      busy_status(1000);
      // don't need to disable for continous rotation servo?

   }*/
   
  return 0;
}
