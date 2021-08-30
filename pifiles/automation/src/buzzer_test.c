//gcc -Wall -o test servo_test.c -lwiringPi -lpthread

//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>


#define BUZZER_PIN    (23)
#define BUZZER_RANGE  (4000)

int get_note_freq(char * note_letter, int note_num)
{
  // mods are '', '#', 'b' for 
  // e.g. 
  // C4 would be ('C', 4)
  // C#4 would be ('C#', 4)
  // Cb4 would be ('Cb', 4)

  float base_freq = 0;

  if (strcmp(note_letter, "C") == 0) 
  {
    base_freq = 16.35;
  } 
  else if(strcmp(note_letter, "C#") == 0 || strcmp(note_letter, "Db") == 0)
  {
    base_freq = 17.32;
  }
  else if(strcmp(note_letter, "D") == 0)
  {
    base_freq = 18.35;
  }
  else if(strcmp(note_letter, "D#") == 0 || strcmp(note_letter, "Eb") == 0)
  {
    base_freq = 19.45;
  }
  else if(strcmp(note_letter, "E") == 0)
  {
    base_freq = 20.60;
  }
  else if(strcmp(note_letter, "F") == 0)
  {
    base_freq = 21.83;
  }
  else if(strcmp(note_letter, "F#") == 0 || strcmp(note_letter, "Gb") == 0)
  {
    base_freq = 23.12;
  }
  else if(strcmp(note_letter, "G") == 0)
  {
    base_freq = 24.50;
  }
  else if(strcmp(note_letter, "G#") == 0 || strcmp(note_letter, "Ab") == 0)
  {
    base_freq = 25.96;
  }
  else if(strcmp(note_letter, "A") == 0)
  {
    base_freq = 27.50;
  }
  else if(strcmp(note_letter, "A#") == 0 || strcmp(note_letter, "Bb") == 0)
  {
    base_freq = 29.14;
  }
  else if(strcmp(note_letter, "B") == 0)
  {
    base_freq = 30.87;
  }
  else
  {
    printf("This is not a note!\n");
    exit(1);
  }  

  float multiplier = pow(2.0, ((float)note_num));

  return (int)(base_freq * multiplier);  
}

int main(int argc, char **argv) 
{
  wiringPiSetupGpio();

  softToneCreate(BUZZER_PIN);

  if(argc < 3)
  {
    printf("Supply a note please\n");
    return 0;
  }

  delay(200);

  int num = atoi(argv[2]);

  int freq = get_note_freq(argv[1], num);

  printf("Note [%s %d] results in freq [%d]\n",
    argv[1], num, freq);


  softToneWrite(BUZZER_PIN, get_note_freq("Bb", 4));
  delay(500);
  softToneWrite(BUZZER_PIN, get_note_freq("C", 5));
  delay(750);
  softToneWrite(BUZZER_PIN, get_note_freq("Ab", 4));
  delay(1000);
  softToneWrite(BUZZER_PIN, get_note_freq("Ab", 3));
  delay(750);
  softToneWrite(BUZZER_PIN, get_note_freq("Eb", 4));
  delay(1750);

  //softToneWrite(BUZZER_PIN, freq);
  //printf("Press enter to stop.\n");
  //getchar();
  /*softToneWrite(BUZZER_PIN, freq*2);
  delay(1000);
  softToneWrite(BUZZER_PIN, freq*4);
  delay(1000);*/ 
/*  int freq = atoi(argv[1]);
  printf("Freq [%d]\n", freq);

  drive_buzzer(freq);*/
  
  softToneWrite(BUZZER_PIN, 0);

  //uninit_buzzer();
  
  return 0;
}
