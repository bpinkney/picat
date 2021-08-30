#!/bin/bash


last_time=`date +%H:%M`

while true; do


  #wait for minute to roll over
  current_time=`date +%H:%M`
  current_time_full=`date +"%I:%M:%S %p %d/%b/%Y"`
  echo "Current Time: $current_time"

  if  [ "$last_time" != "$current_time" ]
  then
    last_time=$current_time

    string=`cat /home/pi/automation/save_files/feed_schedule.txt`


    TIMES=()
    PORTIONS=()

    IFS='.' read -r -a array <<< "$string"

    for a in "${array[@]}"; do

      IFS=',' read -r -a line <<< "$a" 

      entries="${#line[@]}"

      if [ $entries -gt 1 ]
      then
        TIMES+=("${line[0]}")
        PORTIONS+=("${line[1]}")
      fi  
    done

    i=0
    for time_i in "${TIMES[@]}"; do
    #for i in `seq 0 ${#TIMES[@]}`; do  
      #echo "${TIMES[$i]}"
      #time_i="${TIMES[$i]}"
      portion_i="${PORTIONS[$i]}"
      if [ "$current_time" = "$time_i" ]; then
        echo "Feed time is now! $time_i"

        /home/pi/automation/bin/servo_test $portion_i

        log_string="""$current_time_full - AUTOMATIC feed dispensed $portion_i portions"""

        /home/pi/automation/bin/send_test_email "$log_string"

        echo "$log_string" >> /home/pi/automation/save_files/event_log.txt
      fi
      i=$((i + 1))
    done

    echo "Times and Portions:"
    echo "${TIMES[@]}"
    echo "${PORTIONS[@]}"
    echo ""
  fi

  # sleep for a bit and check again (at least once a minute)
  sleep 16

done