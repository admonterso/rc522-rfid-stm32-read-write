# -admonterso-rc522_rfid-stm32-read-write
this example shows how to write 16 byte token with rc522 on Mifare classic cards

I am using STM32f103cbt6. (bluePill)
and CUBEMX IDE
settings:

uart2 - for debbug
SPI1 - for connecting with rc522
also LED


evrery time when you slide the card on card on sector 2 will be read and random 16 bit token will be wrote there, you can chack data through debug UART.
