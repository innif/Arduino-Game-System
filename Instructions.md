# Mini Arcade – Building Instructions
## You will Need
### Material
- ESP8266 D1 Mini
- I2C 0.96 inch OLED Display
- 4x Push Button 6x6x5mm
- Perforated Grid Board / Prototype PCB
- 2x short M3 countersunk head screw (5mm-10mm)
- Some thin Wire 
- Some Solder 
- USB-Cable
### Tools
- Soldering Iron
- Cutter Knife or Saw (to cut PCB)
- Cutting Pliers
- Wire stripper
- File / Sandpaper (if PCB is a bit to big)
- Hotglue Gun (optional)

## Part 1 – Electronics
### Step 1.1 - Trim the PCB
Use a cutter knife or saw to trim the PCB. After Trimming 8 rows and 9 columns should remain.

You can use a cutter knife to carve a line into the PCB where you can then break it by hand.

Carving the PCB:

![carving the pcb](images/1.1.1.jpg)

Breaking the PCB:

![breaking the pcb](images/1.1.2.jpg)

This is what the finished PCB should look like:

![finished pcb](images/1.1.3.jpg)


### Step 1.2 - Add the Buttons
Place the Buttons as shown in the picture:

![Buttons placed](images/1.2.1.jpg)

Make sure that all buttons are seated properly and are flat and level.

Then flip the PCB and Solder all the Buttons:

![soldering Buttons](images/1.2.2.jpg)

### Step 1.3 - Add connections to the Buttons
Use solder to connect the 3 pins marked in this picture. You can also use a tiny bit of wire for that.

![connect buttons](images/1.3.1.jpg)

Then add a wire connecting the "solder-blob" you just created with the pin of the 4th button as shown in the picture:

![connect buttons](images/1.3.2.jpg)

Every Button should now have one pin that is connected to one another. We will call this the "common connection"

### Step 1.4 - Add Wires to the Buttons
Cut 5 short pieces of wire (~ 4-5 cm each) and remove the isolation at both ends.

Add for wires to the connections shown in this image:

![add wires](images/1.4.1.jpg)

Take the 5th wire and add it to the common connection of the buttons:

![add wires](images/1.4.2.jpg)

Every button should now have one contact connected to a wire and one contact connected to the common connection.

### Step 1.5 - Add Wires to the Display
Cut another 4 short pieces of wire like you did in the step before.

Solder them to the 4 connections of your display on top (!). Do not use the pins on the bottom.

The result should look like this:

![display with wires](images/1.5.1.jpg)

Now flip the display and use your cutting pliers to remove the pins:

![cutting the pins](images/1.5.2.jpg)

The result should look like this:

![display finished](images/1.5.3.jpg)

### Step 1.6 - Add Display to the ESP8266

Now connect the cables from the display to the ESP8266 like shown in the Table. The Wires should come from the side with the ESP-Module, the other side than the USB-Connector is placed (see picture).


| Display-Pin | ESP8266-Pin |
| ----------- | ----------- |
| GND         | GND / G     |
| VCC         | 5V / VIN    |
| SCL         | D1          |
| SDA         | D2          |

The result should look like this:

![display connected](images/1.6.1.jpg)

### Step 1.7 - Add Buttons to the ESP8266

Now you have to connect the buttons in the same way, but from the other side (the side of the USB connector).

Connect the cables like shown in the picture:

![Buttons connection](images/1.7.1.jpg)

The result should look like this:

![Connected Front](images/1.7.2.jpg)
![Connected Back](images/1.7.3.jpg)

## Part 2 - Software

### Step 2.1 - Download Arduino IDE
[Download](https://www.arduino.cc/en/software/) and install the Arduino IDE.

### Step 2.2 - Open Project
You have to download the Tiny Arcade Project.
After you have downloaded the project, you can unzip the file and open the GameSystem.ino File inside the folder with the Arduino IDE.

### Step 2.3 - Install ESP8266-Boards
To upload the project, you first have to install the ESP8266-Boards in Arduino. Therefore you have to Open `File -> Preferences` and add the following Text into the "Additional boards manager URLs"-Field and press OK.
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

![Arduino Preferences](images/2.3.1.png)

After you have added this URL, you can open the Board Manager on the left hand side and search for ESP8266. If it does not show up, try restarting the Arduino IDE. Click Install and wait for it to finish.

![Board Manager](images/2.3.2.png)

### Step 2.4 - Install Libraries
Go to the Library Manager on the left hand side and search for `Adafruit SSD1306` click on Install and if asked, if you want to install the dependencies, select "Install All"

![Library Manager](images/2.4.1.png)

### Step 2.5 - Upload Programm
Now you should be able to upload the program. Connect the ESP-Board to your computer using an USB-Cable.

You have to select the board in the dropdown-menu. Click on the Port of your board (usually the highest number you see):

![Select Board](images/2.5.1.png)

In the Popup search for `D1 mini` and select `LOLIN(WEMOS) D1 R2 & mini`:

![Select Board](images/2.5.2.png)

You can now hit the upload-button on the top left on the window and upload the code. After the Upload is done, check if everything is working before moving on to the assembly.

## Part 3 - Assembly

### Step 3.1 - Add Components
Place the screen in the case. If you want, you can use a bit of hot glue, but in most cases this is not necessary.

Then add the buttons. If they dont fit, use a file or sandpaper to make the PCB a bit smaller on the edges that doont fit. Make sure, that the buttons are seated correctly and are pushed all the way through.

Finally add the ESP8266 on top. Make sure, the USB-Connector alligns with the cutout in the case. Now is also a good time to carefully organize the cables and get them away from the side walls.

### Step 3.2 - Add Lid
As a last step you can add the lid. It should hold all the PCBs in place when seated correctly. If you cant push it down, try reorganizing the cables to make everything fit.

When the lid is seated correctly and snaped into place, use two screws on the side of the case to hold the lid in place. 

Congratulations: you are done. Now connect your new console to any USB-Powersupply and start Gaming!