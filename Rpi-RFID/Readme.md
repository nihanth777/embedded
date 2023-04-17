# Installing libraries for RFID
Follow the instructions in the following link to install bcm2853 library
[bcm2835](https://github.com/luvbum/RPi-RFID "https://github.com/luvbum/RPi-RFID")

# Setting up the Raspberry pi
Follow the instruction in the following link to connect the wiring of MFRC522 RFID with RPi 1 B+
[pin connection](https://github.com/GormYa/MFRC522 "https://github.com/GormYa/MFRC522")

# Execute the following command to the run the rfidd server
''' sh
$ g++ MFRC2835.cpp main.cpp -std=c++11 -lbcm2835 -lpthread
$ sudo ./a.out
'''
