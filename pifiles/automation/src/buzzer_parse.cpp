//g++ -Wall -o test buzzer_parse.cpp -lwiringPi -lpthread -lm;

//
//c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

//c++
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

//pi stuff
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>


#define BUZZER_PIN    (23)
#define BUZZER_RANGE  (4000)

static int max_notes = 6;
static int octave_offset = 0;
static float note_dt_max_ms = 20.0;
static int max_play_time = 20;
// tuning param 2nd order poly (no c0 term)
// where freq_true = freq ^2 * c2 + freq * c1
static float freq_poly_c2 = 0.0002; 
static float freq_poly_c1 = 1.0001; 

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

  float raw_freq = base_freq * multiplier;

  // runtime correction factor (2nd order poly) for mfg imperfections in the buzzer
  float corrected_freq = pow(raw_freq, 2) * freq_poly_c2 + raw_freq * freq_poly_c1;

  return (int)(corrected_freq);  
}

void play_notes(
  std::string delay_s, 
  std::string note0, 
  std::string note1, 
  std::string note2, 
  std::string note3, 
  std::string note4, 
  std::string note5)
{

  //int octave_offset = 1;
  static float total_play_time = 0;

  //array of notes and modifiers
  std::vector<std::string> letters;
  std::vector<int> nums;  

 // can we just limit this like this to clean up the sound?
  //int max_notes = 3; // /6

  int i;
  for(i=0; i<max_notes; i++)
  {
    std::string note_raw;
    switch(i)
    {
      case 0:
      note_raw = note0;
      break;
      case 1:
      note_raw = note1;
      break;
      case 2:
      note_raw = note2;
      break;
      case 3:
      note_raw = note3;
      break;
      case 4:
      note_raw = note4;
      break;
      case 5:
      note_raw = note5;
      break;
    }

    // parse raw note
    if(note_raw.length() > 0)
    {
      int num = std::stoi(note_raw.substr(0, 1));
      std::string letter = note_raw.substr(1, note_raw.size() - 1);
      printf("Note %d: [%s %d]\n", i, letter.c_str(), num);

      // add to valid note array
      letters.push_back(letter);
      nums.push_back(num);

      //softToneWrite(BUZZER_PIN, get_note_freq(letter, num));
    }
    else
    {
      // stop playing instrument i
      //softToneWrite(BUZZER_PIN, 0);
    }
  }

  // for the delay duration, we cycle through the valid notes
  // durations can be as low as 0.02s, so we need a short enough interval to
  // ensure proper polyphony for up to 6 notes
  float note_dt_min_ms = 2.0;
  //float note_dt_max_ms = 20.0;

  float delay_ms = (stof(delay_s)*1000.0);

  float delay_counter_ms = 0;

  while (delay_counter_ms < delay_ms)
  {
    // cycle all valid notes
    int num_size = nums.size();
    // try to split the note durations up to be a bit longer
    float note_dt_ms = std::min(note_dt_max_ms, std::max(note_dt_min_ms, (float)delay_ms/std::max((float)num_size, (float)0.001)));
    if(num_size > 0)
    {
      for (i=0;i<num_size;i++)
      {
        softToneWrite(BUZZER_PIN, get_note_freq(letters[i], nums[i] + octave_offset));

        usleep((int)(note_dt_ms*1000));
        delay_counter_ms += note_dt_ms;
        // leav early to avoid imposing delay
        if(delay_counter_ms >= delay_ms)
        {
          break;
        }
      }
    }
    else
    {
      // rest
      softToneWrite(BUZZER_PIN, 0);
      delay_counter_ms = delay_ms;
      usleep((int)(delay_ms*1000));
    }    
  }

  total_play_time += (float)delay_ms / 1000.0;

  if(total_play_time >= max_play_time)
  {
    // we are past our play time limit, disable notes and exit
    printf("Exiting early due to max time param!\n");
    softToneWrite(BUZZER_PIN, 0);
    exit(0);
  }
}

void play_from_csv(std::string filename)
{
  std::ifstream song_file (filename);

  if (song_file)
  {
    std::string time_s = "", note0 = "", note1 = "", note2 = "", note3 = "", note4 = "", note5 = "", delay = "", eol = "";
    while (song_file.good())
    {
      getline(song_file, time_s, ',');      
      getline(song_file, note0, ',');
      getline(song_file, note1, ',');
      getline(song_file, note2, ',');
      getline(song_file, note3, ',');
      getline(song_file, note4, ',');
      getline(song_file, note5, ',');
      getline(song_file, delay);

      /*printf("Line: [%s: %s, %s, %s, %s, %s, %s - %s]\n", 
        time_s.c_str(), 
        note0.c_str(), 
        note1.c_str(), 
        note2.c_str(), 
        note3.c_str(), 
        note4.c_str(), 
        note5.c_str(),
        delay.c_str());*/

      play_notes(delay, note0, note1, note2, note3, note4, note5);
    }
    song_file.close();
  }
}

int main(int argc, char **argv) 
{
  wiringPiSetupGpio();

  softToneCreate(BUZZER_PIN);

  if(argc < 2)
  {
    printf("Supply a file please\n");
    return 0;
  }

  if(argc > 2)
  {
    max_notes = atoi(argv[2]);
  }
  if(argc > 3)
  {
    octave_offset = atoi(argv[3]);
  }
  if(argc > 4)
  {
    note_dt_max_ms = atof(argv[4]);
  }
  if(argc > 6)
  {
    freq_poly_c2 = atof(argv[5]);
    freq_poly_c1 = atof(argv[6]);
  }
  if(argc > 7)
  {
    max_play_time = atoi(argv[7]);
  } 

  delay(200);

  std::string firstarg(argv[1]);
  if(firstarg == "tuning")
  {
    // run quick tuning routine (helps to find tuning mult)
    softToneWrite(BUZZER_PIN, get_note_freq("C", 0));
    delay(5000);
    softToneWrite(BUZZER_PIN, get_note_freq("C", 1));
    delay(5000);
    softToneWrite(BUZZER_PIN, get_note_freq("C", 2));
    delay(5000);
    softToneWrite(BUZZER_PIN, get_note_freq("C", 3));
    delay(5000);
    softToneWrite(BUZZER_PIN, get_note_freq("C", 4));
    delay(5000);
    softToneWrite(BUZZER_PIN, get_note_freq("C", 5));
    delay(5000);
  }
  else
  {
    play_from_csv(argv[1]);
  }  

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
  softToneWrite(BUZZER_PIN, freq*2);
  delay(1000);
  softToneWrite(BUZZER_PIN, freq*4);
  delay(1000);
  int freq = atoi(argv[1]);
  printf("Freq [%d]\n", freq);

  drive_buzzer(freq);
  
  softToneWrite(BUZZER_PIN, 0);

  //uninit_buzzer();
  */
  softToneWrite(BUZZER_PIN, 0);
  return 0;
}
