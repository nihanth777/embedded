# Smart-phone based Automatic Dispenser
## Problem Statement
This project looks into the problem of going to the billing desk after refuelling. many a times the billing desk is qiet far and the weather is not conducive. Hence, this project tries to increase the conveniance of the customer by automating the payment process.

## Proposed Solution-
we propose a RFID based sutomated re-fuelling solution, which identifies the car and user account uniquely using RFID tag and bills the associated user account.

## Steps to the proposed solution-
1. Identifying the vehicle using RFID
2. Communicating this information with main controller
3. Main controller communicates with the cloud
4. user receives notification on smart-phone on app and approves it.
5. main controller receives cost information from the dispenser
6. Main controller performs the transaction.



Readme File
1. Raspberry Pi 1 B+ does not have any inbuilt wifi module for network connection. So, the options are as follows. 
	a. to use USB or ethernet tethering from a router
	b. install external drivers to interface an external wifi adapters 
a. For USB tethering, connect the mobile phone which has got an internet connection to the usb port. 
Go to Settings -> Connections -> Mobile Hotspot and Tethering 
Switch on the USB tethering
On the top right hand corner, it will be observed that the two red crosses will turn into two blue arrows which confirms internet access in the device. 

b. For projects involving networking and TCP IP communication, it is necessary to use a wifi Adpater. The wifi adapter comes with a installation manual but it is advised to follow the steps below.
The raspberry pi needs to be connected with a USB tethered network as the process will require access to the internet.
The driver is specific to the model of the wifi adapter. For our project, it was 8188eu.
Then the following codes needs to be run in terminal to install the wifi drivers :
 	sudo wget http://downloads.fars-robotics.net/wifi-drivers/install-wifi
	sudo mv install-wifi /usr/bin/
 	sudo install-wifi
In the process, it will check for the kernel version and the latest one compatible with that version will be downloaded and installed. Else it will return the kernel version and asking you to downgrade or opgrade your kernel.
After upgrading the kernel from Raspbian website, the above codes needs to be run once again. Once the installation gets completed, the raspberry pi needs to be rebooted. After it gets switched on, connect the wifi driver and select the network to connect.  







