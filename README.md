Before I start, I would like to give credit to cytron technologies for creating the libraries and examples for this to work. Heavily referenced from: https://cytrontechnologies.github.io/RFM-LoRa-Shield-Examples/


What is LoRa?

LoRa is a form of wireless communication like wifi or bluetooth. It provides great range at the expense of lower data transfer. In this page, I would like to showcase what I have learnt from using the Arduino LoRa Shield from: 
https://sg.cytron.io/c-wireless-devices/c-lora/c-lora-devices/p-cytron-lora-rfm-shield


My plan is to use the Arduino LoRa Shield to send data from long ranges and other communication purposes. My first attempt at learning is to make two arduino boards communicate with each other wirelessly, one as a receiver and another as a transmitter.

Under the simple communication folder, I have attached the receiver and transmitter .ino files.

- Receiver: The code basically reads any data send from a LoRa device.  

- Transmitter: The code basically showcase two functions. 
    
The first function is to read any serial input from the serial monitor and send that input to the receiver. 
    
The second function is to send a routine message so as to know if the receiver is still online. 

The problem is that the arduino only has one core, so to run both functions in a loop, all outputs will only be avaliable after the loop complete. Which means that the manual inputs will only appear after the routine messages.

The goal is to get the manual input separate from the routine message, but this was impossible due to the arduino delay() function. To get around this problem, I researched and found that there are other ways to include a delay function without using the delay() function that is provided by arduino. The solution is to use the millis() function. 

Solution referenced from: https://stackoverflow.com/questions/54565484run-two-functions-at-once

My second attempt at learning is to combine both the receiver and transmitter functions into one code. Under the same simple communication folder, the set.ino file basically shows how both functions work together, video below:


https://user-images.githubusercontent.com/90389913/228522089-fe4142e8-5541-4d30-b9f3-733aea70b20b.mp4


Plans for the future:
- To intergrate a oled display so as to remove the need for a serial monitor.
- To add buttons that sends simple text like "Hello" or certain coded messages.
- To add sensors so as to get data like temperature or humidity so as to determine if certain locations are raining or not.
- To create a personal communication device



