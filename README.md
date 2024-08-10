# Monopoly Automated Bank

## Description

This project is a system for managing "virtual" money in a game using RFID cards. The system allows you to perform the following actions:

- Increase the balance on a card
- Decrease the balance on a card
- Check the balance on a card
- Add a new card to the system
- Delete a card from the system

The project also supports adding a master card, which is required to perform all the above operations. Adding the master card is done by pressing the `*` key.

## Requirements

The project requires the following components:

- Arduino Uno or a compatible microcontroller
- MFRC522 RFID module
- 16x2 LCD display with I2C interface
- 4x4 Keypad

### Arduino Libraries

- `MFRC522` by GitHubCommunity
- `LiquidCrystal_I2C` by Frank de Brabander
- `Keypad` by Mark Stanley, Alexander Brevig

## Wiring Diagram

### MFRC522 RFID Module

- SDA -> Pin 10
- SCK -> Pin 13
- MOSI -> Pin 11
- MISO -> Pin 12
- IRQ -> Not used
- GND -> GND
- RST -> Not used
- VCC -> 3.3V

### LCD Display (I2C)

- VCC -> 5V
- GND -> GND
- SDA -> A4
- SCL -> A5

### 4x4 Keypad

Connect the rows and columns of the keypad to pins 2, 3, 4, 5, 6, 7, 8, and 9 on the Arduino.

## Installation

1. Download and install the Arduino IDE from the official website: [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the Arduino IDE and install the necessary libraries:
   - Go to **Sketch -> Include Library -> Manage Libraries**.
   - Find and install the `MFRC522`, `LiquidCrystal_I2C`, and `Keypad` libraries.
3. Connect the components to the Arduino according to the wiring diagram provided above.
4. Copy the code from the `MonopolyAutomatedBank.ino` file into the Arduino IDE or download MonopolyAutomatedBank.ino file.
5. Upload the code to your Arduino.

## Usage

### Main Menu

After starting the device, the main menu will be displayed on the screen. Navigate through the menu items using the `A` and `B` keys on the keypad. Press `C` to select a menu item.
First of all, add a master card, which is required to perform all the above operations. Adding the master card is done by pressing the `*` key.

### Card Operations

1. **Increase balance**:
   - Select this menu item.
   - Verify master card.
   - Scan the card whose balance you want to increase.
   - Enter the amount using the keypad and confirm by pressing `C`.
   
2. **Reduce balance**:
   - Select this menu item.
   - Verify master card.
   - Scan the card whose balance you want to decrease.
   - Enter the amount using the keypad and confirm by pressing `C`.
   
3. **Show balance**:
   - Select this menu item.
   - Verify master card.
   - Scan the card whose balance you want to check.
   - The current balance of the card will be displayed on the screen.
   
4. **Add new card**:
   - Select this menu item.
   - Verify master card.
   - Scan the new card you want to add.
   - The card will be added with an initial balance of 1000 units.
   
5. **Delete card**:
   - Select this menu item.
   - Verify master card.
   - Scan the card you want to delete from the system.
   - The card will be deleted if it exists in the system.

### Adding a Master Card

To add a master card, follow these steps:

- Press the `*` key on the keypad.
- Scan the card you want to designate as the master card.
- The master card will be saved, and it will need to be scanned before performing any operations.

## Author

This project was created and developed by [1onjonson](https://github.com/1onjonson).
