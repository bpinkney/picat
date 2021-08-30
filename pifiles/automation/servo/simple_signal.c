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
#define LED_BLUE_INT    (99)
#define LED_RED_INT     (99)
#define LED_GREEN_INT   (90)

void init_leds()
{
   softPwmCreate(LED_BLUE_PIN, 100, 100);
   softPwmCreate(LED_RED_PIN, 100, 100);
   softPwmCreate(LED_GREEN_PIN, 100, 100);
}

void led_enable(int enable, int pin, int pin_int)
{
   if(enable)
      softPwmWrite(pin, pin_int);
   else
      softPwmWrite(pin, 100);
}

int main(int argc, char **argv) 
{
  
   wiringPiSetupGpio();

   init_leds();

   int rgb = 0;

   if(argc < 2)
   {
      printf("DEFAULT Simple Signal [%d]\n", rgb);
   }
   else
   {
      rgb = atoi(argv[1]);
      printf("Simple Signal [%d]\n", rgb);
   }      

   switch(rgb)
   {
      case 0:
         led_enable(1, LED_RED_PIN, LED_RED_INT);
         delay(2000);
         led_enable(0, LED_RED_PIN, LED_RED_INT);
      break;      
      case 1:
         led_enable(1, LED_GREEN_PIN, LED_GREEN_INT);
         delay(2000);
         led_enable(0, LED_GREEN_PIN, LED_GREEN_INT);
      break;
      case 2:
         led_enable(1, LED_BLUE_PIN, LED_BLUE_INT);
         delay(2000);
         led_enable(0, LED_BLUE_PIN, LED_BLUE_INT);
      break;
      delay(2000);
   }
   
   return 0;
}
