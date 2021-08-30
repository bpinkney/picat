



<!-- html markup -->
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<link rel="stylesheet" type="text/css" href="webserver.css">
</head>

<!-- redirect any 'submit' actions to the same page to pick up the php -->
<form action="index.php" method="post">

  <!-- load php variables from the schedule file prior to markup -->
  <?php 
    // add ob_start to make sure any php 'echo' commands before markup don't ruin refreshes
    ob_start();
    load_feed_schedule();
  ?>

  <font style="font-size:180%;"><?php date_default_timezone_set("America/New York"); echo date("h:i:s A  d/M/Y\r\n"); ?></font>
  <br>
  <br>
  <input 
    class="BasicButton" type="submit"  
    value="Manual Feed" 
    name="manaulFeedAction" 
    width="80%"/>

  <br>
  <br>
  <div>
    <font style="font-size:250%;">Manual Portions:</font>
    <input type="number" name="manualPortion" value="<?= $manualPortion ?>">
  </div>

  <!-- <input 
    class="BasicButton" type="submit"  
    value="Test Servo From PHP" 
    name="servoAction" 
    display="none"/>
  <br>
  <br>
  <input 
    class="BasicButton" type="submit"  
    value="Test LEDSs From PHP" 
    name="ledAction" />
  <br>
  <br>
  <input 
    class="BasicButton" type="submit"  
    value="Test Email From PHP" 
    name="emailAction" />
  <br> -->
  
  <br>
  <table id="feedTable" class="blueTable" name="feedTable">
  <th>Automatic Feeding Time</th>
  <th>Feeding Portions</th>
  <tr><td><font><input type="time" name="time1" value="<?= $time1 ?>"></font></td><td><font color="green"><input type="number" name="portion1" value="<?= $portion1 ?>"></font></td></tr>
  <tr><td><font><input type="time" name="time2" value="<?= $time2 ?>"></font></td><td><font color="green"><input type="number" name="portion2" value="<?= $portion2 ?>"></font></td></tr>
  <tr><td><font><input type="time" name="time3" value="<?= $time3 ?>"></font></td><td><font color="green"><input type="number" name="portion3" value="<?= $portion3 ?>"></font></td></tr>
  </table>
  <br>
  
  <input type="checkbox" name="emailAlerts[]" value="1" <?php echo ($emailAlerts==1 ? 'checked' : '');?>>
  <font style="font-size:250%;">Email Alerts</font>

  <br>
  <br>
  <br>

  <font style="font-size:150%;">Alert Song:</font>
  <select id="midiSelect" name="midiSelect" style="font-size:150%;" action="">
     <option value="None">------------</option>
      <?php
      foreach($midiFiles as $key => $value):      
        echo '<option value="'.$value.'" '.(($midiFilesSelect==$value ? 'selected' : '')).'>'.$value.'</option>'; 
      endforeach;
      ?>
  </select>
  <br>
  <input 
    class="BasicButton" type="submit"  
    value="Test Alert Song" 
    name="testMidi"/>

  <br>
  <br>
  <br>

  <input class="BasicButton" type="submit" 
    value="Save Settings" 
    name="saveAction"/>

  <br>

<!-- 
  <input type="button" id="txtButton" value="Save" onclick="tableToTxt();"/> -->
</form>
</html>


<!-- javascript code -->
<script type="text/javascript">

function tableToTxt() 
{
  /*document.getElementById("demo").textContent="newtext";
  //document.getElementById("demo").innerHTML = "Hello World!";
  var feedTable = document.getElementById('feedTable');
  var feedTableContents = feedTable.innerHTML;

  var text = "jim";
  var filename = "/home/pi/automation/save_files/feedTable.txt";
  var blob = new Blob([feedTableContents], {type: "text/plain;charset=utf-8"});
  saveAs(blob, filename);
*/

  //feedTable.innerHTML=feedTableContents;
  //feedTable.setAttribute('href', 'data:text/csv;charset=utf-8,' + encodeURIComponent(VALUE));
  //feedTable.setAttribute('feedTable', 'filename.csv');

  //file_put_contents($filename, $content)
}

</script>


<!-- php code -->
<!-- add 'www-data ALL=NOPASSWD: /home/pi/automation/bin/*' -->
<!-- to your 'visudo' file for each executable dir-->
<!-- might need to add the user to the gpio group as well? -->
<!-- Also chmod any dirs you wnat to generate files in from php -->
<?php
    
    date_default_timezone_set("America/New York");

    //load last 5 events
    load_event_log();

    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['servoAction']))
    {
      test_servo();
    }
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['ledAction']))
    {
      test_led();
    }
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['emailAction']))
    {
      test_email();
    }
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['saveAction']))
    {
      save_feed_schedule();  
      //header('Location: index.php'); //clears POST to avoid double submit
    }
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['testMidi']))
    {
      test_midi();      
    }
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['manaulFeedAction']))
    {
      manual_feed();      
    }

    function test_servo()
    {
      //echo "The function has been called";
      $old_path = getcwd();
      chdir('/home/pi/automation/bin/');
      $output = shell_exec('/usr/bin/sudo -u root ./servo_test 1');
      chdir($old_path);  
      echo "The SERVO function has been called";
    }

    function test_led()
    {
      //echo "The function has been called";
      $old_path = getcwd();
      chdir('/home/pi/automation/bin/');
      $output = shell_exec('/usr/bin/sudo -u root ./simple_signal 0');
      $output = shell_exec('/usr/bin/sudo -u root ./simple_signal 1');
      $output = shell_exec('/usr/bin/sudo -u root ./simple_signal 2');
      chdir($old_path);  
      echo "The LED function has been called";
    }
    function test_email()
    {
      //echo "The function has been called";
      $old_path = getcwd();
      chdir('/home/pi/automation/bin/');
      $output = shell_exec('./send_test_email "Hello from PHP-Land"');
      chdir($old_path);  
      echo "The EMAIL function has been called";
    }

    function test_midi()
    {
      $midiFilesSelect = $_POST['midiSelect'];
      echo $midiFilesSelect;
      if ($midiFilesSelect != "None")
        $output = shell_exec("/home/pi/automation/bin/play_buzzer_song /home/pi/automation/midi_files/publish/$midiFilesSelect.midicsv");
    }

    function manual_feed()
    {      
      $manualPortion = $_POST['manualPortion'];

      $old_path = getcwd();
      chdir('/home/pi/automation/bin/');
      $output = shell_exec("/usr/bin/sudo -u root ./servo_test $manualPortion");
      chdir($old_path);  

      log_event(date("h:i:s A  d/M/Y")." - MANUAL feed dispensed $manualPortion portions\n");

      load_event_log();

      //header("Refresh:0");
    }

    function proper_page_refresh()
    {
      $page = $_SERVER['PHP_SELF'];
      $sec = "10";
      header("Refresh: $sec; url=$page");
    }

    function load_feed_schedule()
    {
      global $time1, $time2, $time3, $portion1, $portion2, $portion3, $manualPortion, $emailAlerts, $midiFiles, $midiFilesSelect;

      // read back
      $file_contents = file_get_contents("/home/pi/automation/save_files/feed_schedule.txt");

      $rows = explode('.', $file_contents);

      // Parse by letter key
      foreach ($rows as &$row) 
      {
        //echo "$row";
        $er = explode(',', $row);
        switch ($er[0]) 
        {
          case "T1":
            $time1    = $er[1];
            $portion1 = $er[2];  

            break;
          case "T2":
            $time2    = $er[1];
            $portion2 = $er[2];
           
            break;
          case "T3":
            $time3    = $er[1];
            $portion3 = $er[2];
           
            break;
          case "M":
            $manualPortion = $er[1];
          
            break;
          case "E":
            $emailAlerts = $er[1];
          
            break;
          case "S":
            $midiFilesSelect = $er[1];
            
            break;
        }    
      }

      // read in midi filenames and trim
      $output = shell_exec("ls /home/pi/automation/midi_files/publish/ | sed 's/.midicsv//g'");
      $midiFiles=explode("\n",$output);
    }

    function save_feed_schedule()
    {
      // update value of table vars from POST
      $time1 = $_POST['time1'];
      $portion1 = $_POST['portion1'];
      $time2 = $_POST['time2'];
      $portion2 = $_POST['portion2'];
      $time3 = $_POST['time3'];
      $portion3 = $_POST['portion3'];

      $manualPortion = $_POST['manualPortion'];

      $emailAlerts = isset($_POST['emailAlerts']) ? 1 : 0;

      $midiFilesSelect = $_POST['midiSelect'];      

      // make sure to chmod the save dir and files first!
      file_put_contents("/home/pi/automation/save_files/feed_schedule.txt","T1,$time1,$portion1.T2,$time2,$portion2.T3,$time3,$portion3.M,$manualPortion.E,$emailAlerts.S,$midiFilesSelect");
      
      log_event(date("h:i:s A  d/M/Y")." - Saved the feeding schedule\n");

      // sync file system
      shell_exec("sync");

      //load_feed_schedule();
     
      header("Refresh:0");
      

      //Header('Location: '.$_SERVER['PHP_SELF']);
      //Exit(); //optional
    }

    function log_event($event_string)
    {
      file_put_contents("/home/pi/automation/save_files/event_log.txt", $event_string, FILE_APPEND);
    }

    function load_event_log()
    {
      // print last 5 log items
      $file_contents = file_get_contents("/home/pi/automation/save_files/event_log.txt");
      $lines = explode( "\n", $file_contents );
      $line_count = count($lines);
      for ($x = $line_count - 1; $x >= $line_count - 6; $x--) 
      {
        $line = "$lines[$x]\r\n\r\n";
        echo nl2br($line);
      }
    }
?>

