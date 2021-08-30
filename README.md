
#Steps for PiFeeder

Local Machine = Linux Laptop
Substitude putty and winscp if you only have Windows

##in sudo raspi-config:
- set hostname as pifeeder
- enable wifi and set up
- expand file system
- enable ssh
- set timezone

##From local machine, before SSH:
`ssh-copy-id pi@pifeeder`
(or `ssh-keygen -R 192.168.X.XXX` for your pi IP)

##ssh in then:
`sudo apt-get update`
`sudo apt-get install vim ssmtp mailutils mpack dnsutils gawk apache2 cron -y`

##From local machine, cd to the 'pifiles' folder, and copy all the files onto the pi home:
scp -r * pi@pifeeder:~/

##Add your alert email to:
the `automation/servo/send_test_email` script in place of `EMAILHERE`

##set up cron job for feeder_daemon
`sudo crontab -e -u root`
add line:
`@reboot /home/pi/automation/bin/feed_daemon.sh`

##grab wiringPi and install

## add some permissions
`sudo visudo`
add line:
`www-data ALL=NOPASSWD: /home/pi/automation/bin/*`

##add www-data to gpio group
`sudo adduser www-data gpio`
`sudo chown -R pi:www-data /var/www/html/`
`sudo chmod -R 770 /var/www/html/`
`sudo apt install php php-mbstring -y`

##From local machine, navigate to the webserver directory and copy files up:
`scp * pi@pifeeder:/var/www/html/`

##I forget these steps... need to double check
move index.php file?
move remaining file to bin or src as required?

##chmod 777 any files which you'll be writing to
`sudo chmod 777 event_log.txt`
`sudo chmod 777 feed_schedule.txt` 

##From local machine, navigate to the cfg_files dir and:
`scp cfg_files/ssmtp.conf pi@pifeeder:/etc/ssmtp/ssmtp.conf`
change `EMAILHERE` to your alert email again
change AUTHPASSHERE to your auth token from gmail

##Back on the pi, chmod some files
`sudo chmod 640 /etc/ssmtp/ssmtp.conf`
`sudo chmod 640 /etc/ssmtp/ssmtp.conf`
`sudo chmod 777 /etc/ssmtp /etc/ssmtp/*`

Midi files converted with MIDITONES, great project!


