# Read/Write 16byte Token on Mifare Classic using STM32 and RC522 RFID reader.
## Problem
In order to restrict duplication of the cards or **replay attacks**, there are a few different methods like rolling codes, two-factor auth, and more. In this repository, there is a different approach to preventing replay attacks. 

## Solution
One of the ways is to generate something like **rolling-codes**. We call the data, written inside the card, token. The token will be checked if is valid while reading having its own logic.

## Technical Description
This example shows how to write 16 byte data (token) with RC522 on Mifare Classic cards.

In this project was used `STM32f103cbt6` (bluePill) and [CUBEMX IDE](https://www.st.com/en/development-tools/stm32cubeide.html).
Settings:
- UART2 - for debbug
- SPI1 - for connecting with rc522
- LED - PC13 as an indicator

Cards for which this works are Mifare Classic 1k (cheapest <3). Each time when you slide a card on sector 2, it will read and generate random 16 bit token that be written on the same sector.

