//g++ -Wall -o test buzzer_test.c -lwiringPi -lpthread -lm;

//
//c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//c++
#include <string>
#include <iostream>

//pi stuff
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>


#define BUZZER_PIN    (23)
#define BUZZER_RANGE  (4000)

int get_note_freq(std::string note_letter, int note_num)
{
  // mods are '', '#', 'b' for 
  // e.g. 
  // C4 would be ('C', 4)
  // C#4 would be ('C#', 4)
  // Cb4 would be ('Cb', 4)

  float base_freq = 0;

  if ((note_letter == "C")) 
  {
    base_freq = 16.35;
  } 
  else if((note_letter == "C#") || (note_letter == "Db"))
  {
    base_freq = 17.32;
  }
  else if((note_letter == "D"))
  {
    base_freq = 18.35;
  }
  else if((note_letter == "D#") || (note_letter == "Eb"))
  {
    base_freq = 19.45;
  }
  else if((note_letter == "E"))
  {
    base_freq = 20.60;
  }
  else if((note_letter == "F"))
  {
    base_freq = 21.83;
  }
  else if((note_letter == "F#") || (note_letter == "Gb"))
  {
    base_freq = 23.12;
  }
  else if((note_letter == "G"))
  {
    base_freq = 24.50;
  }
  else if((note_letter == "G#") || (note_letter == "Ab"))
  {
    base_freq = 25.96;
  }
  else if((note_letter == "A"))
  {
    base_freq = 27.50;
  }
  else if((note_letter == "A#") || (note_letter == "Bb"))
  {
    base_freq = 29.14;
  }
  else if((note_letter == "B"))
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

void play_from_csv(void)
{
  while (getline(file, ID, ',')) {
      cout << "ID: " << ID << " " ; 

      getline(file, nome, ',') ;
      cout << "User: " << nome << " " ;

      getline(file, idade, ',') ;
      cout << "Idade: " << idade << " "  ; 

      getline(file, genero);
      cout << "Sexo: " <<  genero<< " "  ;
  }
}

int main(int argc, char **argv) 
{


  play_from_csv();

  /*wiringPiSetupGpio();

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
  
  //softToneWrite(BUZZER_PIN, 0);

  //uninit_buzzer();
  
  return 0;
}
