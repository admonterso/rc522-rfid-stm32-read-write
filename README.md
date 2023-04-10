# Read/Write data on Mifare Classic using STM32 and RC522 RFID reader.
This example shows how to write 16 byte data (token) with RC522 on Mifare Classic cards.

I am using STM32f103cbt6. (bluePill)
and CUBEMX IDE
settings:

uart2 - for debbug
SPI1 - for connecting with rc522
also LED

card for wich this wroks are Mifare classic 1k (chippest <3)
evrery time when you slide the card on card on sector 2 will be read and random 16 bit token will be wrote there, you can chack data through debug UART.
