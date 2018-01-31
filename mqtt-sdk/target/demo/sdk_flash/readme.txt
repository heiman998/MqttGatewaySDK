Readme for adding HTML pages
============================

Requirements:
------------
Linux System

Steps:
-----
* Copy the HTML files which needed to be added, to the same directory where all the scripts are present.
* Rename the firmware binary to "flashotp.bin" and copy "flashotp.bin" to the same directory
* Create a file named "filelist" in the same directory
* Add the htmlfile names to the file "filelist" one by one with each filename in one line.
  Note: "filelist" should not contain any blank lines.
* Run the script "htmltoflash.sh" which will generate the flash image named "flashotp.bin.new"
* Use the new image "flashotp.bin.new" to flash using host.exe

Note:
----
host.exe use the default image "flashotp.bin" to flash.
So use the complete path of new flash image to flash the new image "flashotp.bin.new"
