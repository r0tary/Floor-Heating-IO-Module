# FLOOR-HEATING-IO-MODULE

## Description

- A floor heating controler device that is designed to operate 4 separate zones with TWAs (Thermal Wax Actuators) and 4 PT1000 temperature sensors per zone. The device is able to to get up to a specified temperature and will hold it there by use of a PI controller.
- The device is designed to be connected to a main controller, which can monitor it, communicate with it and set its set point temperature all through the MODBUS communication protocol.
- **NOT YET TESTED** Ability to connect to wireless thermostats using Zigbee and set temperature and retrieve temp reading.

## Software Description
- A STM32WB55 Nucleo board was used to vevelop the softwrae. Running freeRTOS with multiple threads - MODBUS thread, PI thread, temperature ADC thread.

```
  - IO_MODULE_proj //main STMCubeIDE project folder
  |  - Core
    |  - Inc
      |  - main.h
      |  - IO_Config.h //configuration file, sets the number of rooms
      |  - app_freertos.h // io_module_t structure decleration and enums used for registers and coils
      |  - ModbusConfig.h //configuration file for Modbus
    |  - Src
      |  - main.c
      |  - app_freertos.c //code for PID, temperature threads and other used functions
      |  - PID0.c //functions used to calculate PID values
    |  - Middlewares/Third_Party
      |  - FreeRTOS/Source //freeRTOS files
      |  - MODBUS-LIB //Modbus library folder
      |  - OLED_SSD1306 //OLED library folder (configured but not initialized)
- IO_MODULE_proj_Zigbee_FreeRTOS //project folder with Zigbee 
```

- Temperature set point values are stored in "Holding_Registers_Database", see [app_freertos.h](https://github.com/r0tary/Floor-Heating-IO-Module/blob/main/IO_MODULE_proj/Core/Inc/app_freertos.h) to see addresses.
- The MODBUS library used is a fork of [Modbus-STM32-HAL-FreeRTOS](https://github.com/alejoseb/Modbus-STM32-HAL-FreeRTOS), the fork is available [here](https://github.com/r0tary/Modbus-STM32)
- Both ADC and UART have DMA enabled.
- **MODBUS** - thread "StartTaskModbusSlave", timer "TimerT35", semaphore. Found in [Modbus.c](https://github.com/r0tary/Floor-Heating-IO-Module/blob/main/IO_MODULE_proj/Middlewares/Third_Party/MODBUS-LIB/Src/Modbus.c)
- **ADC** - thread "CalculateTemp_Thread". Found in [app_freertos.c](https://github.com/r0tary/Floor-Heating-IO-Module/blob/main/IO_MODULE_proj/Core/Src/app_freertos.c)
- **PI control** - thread "ControlTask", timer "ControlExecTim and "TwaControlTim"
- [Captured packets from Zigbee](https://github.com/r0tary/Floor-Heating-IO-Module/blob/main/Zigbee%20captured%20packets.pcapng)

## Contributors
|**Contributor(s)** | **Contribution** |
|---|---|
|[r0tary](https://github.com/r0tary)| Modbus implementation, freeRTOS implementation|
|[Dimi2509](https://github.com/Dimi2509)| ADC implemenattion, PI control implementation, freeRTOS implementation|
|[rausitis](https://github.com/rausitis)|Zigbee implementation, freeRTOS implementation|
|[Virtualiden-bot](https://github.com/Virtualiden-bot)|Zigbee implementation|
|[Azerio102](https://github.com/r0tary/Floor-Heating-IO-Module/settings/access)|PI control implementation|





