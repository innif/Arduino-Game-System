# Mini Arcade – Building Instructions
## You will Need
### Material
- ESP8266 D1 Mini
- I2C 0.96 inch OLED Display
- 4x Push Button 6x6x5mm
- Perforated Grid Board / Prototype PCB
- Some thin Wire 
- Some Solder 
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

## Part 3 - Assembly