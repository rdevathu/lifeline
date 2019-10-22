# Lifeline

![Lifeline](/images/Lifeline.png)

Lifeline is a project that aims to create a low-cost high utility CPR vest that actively gives the user feedback and instruction on their compressions and additionally instruction on their rescue-breaths. Timing and efficency are critical when taking care of a patient undergoing a cardiac event. Every minute can increase their likelihood of survival incredibly. 

This project makes use of just a few major components which include:
- 3 Arduino Nanos
  - These microcontrollers split up the work among themselves to handle all the subfunctions that need to be dealt with.
- 1 HX711 Load Cell Amplifier  and 4 Load Cells
  - The HX711 reads the signal from the 4 loads cells to accurately tell the Arduino the load or force applied by the user.
- 1 SD Card Reader
  - This allows for the audio files to be stored and read.
- 1 Sound Amplifier
  - This allows the sound signal to be amplified before going out to the speakers and essentially drive the speakers.
- 2 Speakers
  - Allow for ample volume even in loud and hectic conditions so that the user is able to hear all commands clearly
- 1 Li-Ion Battery
  - Allows for wireless operation of the device and storage for longer periods. Also allows for greater portability
- Multiple non-function parts
  - All these parts are mounted on a vest which contains two load plates. The first load plate gives the user a comfortable place to rest and push with their hands and the second redistributes the load once it passes through the load cell. 
  
