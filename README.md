# ALED-module
<p align="center">
  <img src="https://github.com/WiktorK02/ALED-module/assets/123249470/9a79acc4-f7d6-4146-9437-0b07264b8eae" alt="Opis obrazka">
</p>
Module for addressable LEDs that allows programming and connection with Ampio Smart Home automation

#### Schematic 
<img width="838" alt="Zrzut ekranu 2024-10-17 o 10 27 19" src="https://github.com/user-attachments/assets/d3ad4bb5-ca15-42b2-a27c-a2c38f07e021">
<img width="666" alt="Zrzut ekranu 2024-10-17 o 10 27 40" src="https://github.com/user-attachments/assets/11f7202f-8c11-476b-a4bd-dc54e3811250">

### 3D PCB layout + components
![](https://github.com/WiktorK02/ALED-module/assets/123249470/a3a85da0-7c97-4d97-af20-820a503ff207)

### Level shifter 
A level shifter is an electronic circuit used to safely interface between components that operate at different voltage levels. These devices are essential when you have a 3.3V microcontroller like the ESP32 or STM32, but need to communicate with peripherals that operate at 5V (WS2811).
#### Schematic 
<img width="679" alt="Zrzut ekranu 2024-10-17 o 10 17 44" src="https://github.com/user-attachments/assets/fb03944f-0fd1-4c0a-85e4-72deaee5cdb8">
<img width="807" alt="Zrzut ekranu 2024-10-17 o 10 19 44" src="https://github.com/user-attachments/assets/d1446eb5-8815-4ffa-8275-cbc46ecc91c4">


#### 3D PCB layout of Level shifter
<img width="1048" alt="Zrzut ekranu 2024-10-17 o 10 06 57" src="https://github.com/user-attachments/assets/8f44f7b4-5e23-469a-b20f-64ea24ac4f5d">



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
