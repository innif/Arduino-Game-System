# Mini Arcade – Building Instructions

## What You'll Need

### Materials
- ESP8266 D1 Mini
- I2C 0.96 inch OLED Display
- 4x Push Buttons (6x6x5mm)
- Perforated Grid Board / Prototype PCB
- 2x Short M3 countersunk head screws (5mm-10mm)
- Thin wire for connections
- Solder
- USB Cable

### Tools
- Soldering Iron
- Cutter knife or saw (to cut PCB)
- Cutting pliers
- Wire strippers
- File / Sandpaper (if PCB needs resizing)
- Hot glue gun (optional)

## Part 1 – Electronics

### Step 1.1 - Trim the PCB
Use a cutter knife or saw to trim the PCB to the correct size. After trimming, 8 rows and 9 columns should remain.

You can use a cutter knife to score a line into the PCB where you want to break it, then snap it by hand.

Scoring the PCB:

![scoring the pcb](images/1.1.1.jpg)

Breaking the PCB:

![breaking the pcb](images/1.1.2.jpg)

This is what the finished PCB should look like:

![finished pcb](images/1.1.3.jpg)

### Step 1.2 - Install the Buttons
Place the buttons as shown in the picture:

![Buttons placed](images/1.2.1.jpg)

Make sure all buttons are seated properly and sit flat and level.

Then flip the PCB over and solder all the buttons in place:

![soldering buttons](images/1.2.2.jpg)

### Step 1.3 - Create Button Connections
Use solder to connect the 3 pins marked in this picture. You can also use a small piece of wire for this connection.

![connect buttons](images/1.3.1.jpg)

Then add a wire connecting the "solder bridge" you just created to the corresponding pin of the 4th button as shown in the picture:

![connect buttons](images/1.3.2.jpg)

Each button should now have one pin connected to the others. We'll call this the "common connection."

### Step 1.4 - Add Wires to the Buttons
Cut 5 short pieces of wire (~4-5 cm each) and strip the insulation from both ends.

Add four wires to the connections shown in this image:

![add wires](images/1.4.1.jpg)

Take the 5th wire and connect it to the common connection of the buttons:

![add wires](images/1.4.2.jpg)

Each button should now have one contact connected to an individual wire and one contact connected to the common connection.

### Step 1.5 - Add Wires to the Display
Cut another 4 short pieces of wire as you did in the previous step.

Solder them to the 4 connection points on the top of your display (!). Do not use the pins on the bottom.

The result should look like this:

![display with wires](images/1.5.1.jpg)

Now flip the display over and use your cutting pliers to remove the bottom pins:

![cutting the pins](images/1.5.2.jpg)

The result should look like this:

![display finished](images/1.5.3.jpg)

### Step 1.6 - Connect Display to the ESP8266

Connect the cables from the display to the ESP8266 according to the table below. The wires should come from the side with the ESP module (the opposite side from where the USB connector is located - see picture).

| Display Pin | ESP8266 Pin |
| ----------- | ----------- |
| GND         | GND / G     |
| VCC         | 5V / VIN    |
| SCL         | D1          |
| SDA         | D2          |

The result should look like this:

![display connected](images/1.6.1.jpg)

### Step 1.7 - Connect Buttons to the ESP8266

Now connect the buttons in the same manner, but from the other side (the side with the USB connector).

Connect the cables as shown in the picture:

![buttons connection](images/1.7.1.jpg)

The result should look like this:

![connected front](images/1.7.2.jpg)
![connected back](images/1.7.3.jpg)

## Part 2 - Software

### Step 2.1 - Download Arduino IDE
[Download](https://www.arduino.cc/en/software/) and install the Arduino IDE.

### Step 2.2 - Open Project
Download the Mini Arcade project files.
After downloading, unzip the file and open the `GameSystem.ino` file inside the folder with the Arduino IDE.

### Step 2.3 - Install ESP8266 Board Support
To upload the project, you first need to install ESP8266 board support in Arduino IDE. Open `File -> Preferences` and add the following URL to the "Additional boards manager URLs" field, then press OK:

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

![Arduino Preferences](images/2.3.1.png)

After adding this URL, open the Board Manager on the left side and search for "ESP8266". If it doesn't appear, try restarting the Arduino IDE. Click "Install" and wait for it to finish.

![Board Manager](images/2.3.2.png)

### Step 2.4 - Install Required Libraries
Go to the Library Manager on the left side and search for `Adafruit SSD1306`. Click "Install" and when asked about dependencies, select "Install All".

![Library Manager](images/2.4.1.png)

### Step 2.5 - Upload Program
Now you should be able to upload the program. Connect the ESP board to your computer using a USB cable.

Select the board in the dropdown menu. Click on your board's port (usually the highest number you see):

![Select Board](images/2.5.1.png)

In the popup, search for `D1 mini` and select `LOLIN(WEMOS) D1 R2 & mini`:

![Select Board](images/2.5.2.png)

Click the upload button in the top left of the window to upload the code. After the upload is complete, test that everything is working before proceeding to assembly.

## Part 3 - Assembly

### Step 3.1 - Install Components
Place the screen in the case. If desired, you can use a small amount of hot glue, but this is usually not necessary.

![Screen in Case](images/3.1.1.jpg)

Next, install the buttons. If they don't fit, use a file or sandpaper to slightly reduce the PCB size on the edges that don't fit. Ensure the buttons are seated correctly and pushed all the way through.

![Buttons in Case](images/3.1.2.jpg)

Finally, add the ESP8266 on top. Make sure the USB connector aligns with the cutout in the case. This is also a good time to carefully organize the cables and route them away from the side walls.

![ESP in Case](images/3.1.3.jpg)

### Step 3.2 - Attach the Lid
As the final step, attach the lid. It should hold all the PCBs in place when seated correctly. If you can't push it down, try reorganizing the cables to make everything fit properly.

![Lid on Box](images/3.2.1.jpg)

When the lid is seated correctly and snapped into place, use two screws on the side of the case to secure the lid.

![Lid on Box](images/3.2.2.jpg)

Congratulations! You're done. Now connect your new console to any USB power supply and start gaming!