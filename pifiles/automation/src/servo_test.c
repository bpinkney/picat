//gcc -Wall -o test servo_test.c -lwiringPi -lpthread

//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <softPwm.h>


#define LED_BLUE_PIN    (17)
#define LED_RED_PIN     (22)
#define LED_GREEN_PIN   (27)

#define FEED_POS_0   (100)
#define FEED_POS_1   (150)

void init_leds()
{
   softPwmCreate(LED_BLUE_PIN, 100, 100);
   softPwmCreate(LED_RED_PIN, 100, 100);
   softPwmCreate(LED_GREEN_PIN, 100, 100);
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
}

void disable_pwm()
{
   pinMode(18, INPUT);
}

void busy_status(int delay_time)
{
   //led_enable(0, LED_GREEN_PIN);
   for (int i=0;i<(delay_time/200);i++)
   {
      led_enable(1, LED_BLUE_PIN);
      led_enable(0, LED_RED_PIN);
      delay(200);
      led_enable(0, LED_BLUE_PIN);
      led_enable(1, LED_RED_PIN);
      delay(200);
   }
   led_enable(0, LED_BLUE_PIN);
   led_enable(0, LED_RED_PIN);
   //led_enable(1, LED_GREEN_PIN);
}

int main(int argc, char **argv) 
{
   //char user_in[20];
   //int pwm_cmd = 150;

   //printf ("Raspberry Pi wiringPi test program\n");
   wiringPiSetupGpio();

   init_leds();

   int feed_portions = atoi(argv[1]);
   printf("Feed Portions [%d]\n", feed_portions);

   for (int i=0; i < feed_portions; i++)
   {
      //printf("Enter PWM cmd: ");
      //fgets(user_in,20,stdin);
      //pwm_cmd = atoi(user_in);
      init_pwm();

      delay(2000); // impose some delay here, the pi zero seems to init slower

      pwmWrite(18,FEED_POS_0);      
      printf("PWM cmd [%d]\n", FEED_POS_0);
      busy_status(2000);

      pwmWrite(18,FEED_POS_1);      
      printf("PWM cmd [%d]\n", FEED_POS_1);
      busy_status(2000);

      delay(2000); // impose some delay here, the pi zero seems to init slower

      disable_pwm();

   }
   
   return 0;
}
