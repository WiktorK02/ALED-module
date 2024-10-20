# ALED-module
<p align="center">
  <img src="https://github.com/WiktorK02/ALED-module/assets/123249470/9a79acc4-f7d6-4146-9437-0b07264b8eae" alt="Opis obrazka">
</p>
Module for addressable LEDs that allows programming and connection with Ampio Smart Home automation<br><br>

![](https://github.com/WiktorK02/ALED-module/assets/123249470/a3a85da0-7c97-4d97-af20-820a503ff207)<br> <br>
### Module Overview: I/O, LED Control, and PWM Conversion 
The module features 4 I/O inputs with built-in ground isolation through optocouplers, ensuring safe and reliable operation. It includes 3 outputs for controlling up to 3 individual LED strips. Additionally, it has 4 PWM inputs that convert PWM signals to analog, with each input equipped with a pull-up resistor for enhanced signal stability.
## Level shifter 
A level shifter is an electronic circuit used to safely interface between components that operate at different voltage levels. These devices are essential when you have a 3.3V microcontroller like the ESP32 or STM32, but need to communicate with peripherals that operate at 5V (WS2811).<br><br>
<img width="1048" alt="Zrzut ekranu 2024-10-17 o 10 06 57" src="https://github.com/user-attachments/assets/8f44f7b4-5e23-469a-b20f-64ea24ac4f5d">
### Features Overview
The module includes a 7-36V to 5V step-down linear regulator, a reverse polarity protection system, four indicator LEDs showing output operation status, and a level shifter that boosts voltage from 3.3V to 5V.


## Task list
- [x] test code 
- [x] fix upper module GP8501
- [x] smooth analog values from led_width_mconf.cpp
- [x] test out with esp32
- [x] configure mapped values with reference 3.3v 
## How to Contribute
1. Fork the Project
2. Clone repo with your GitHub username instead of ```YOUR-USERNAME```:<br>
```
$ git clone https://github.com/YOUR-USERNAME/ALED-module
```
3. Create new branch:<br>
```
$ git branch BRANCH-NAME 
$ git checkout BRANCH-NAME
```
4. Make changes and test<br>
5. Submit Pull Request with comprehensive description of change
## License 
[MIT license](LICENSE)
