# AutoaimCar | Cyberlife Lab

An autonomous mobile robot for visual aiming and target tracking, developed at South China University of Technology. Originally built for the 2025 National Undergraduate Electronic Design Contest (Problem E), it was upgraded for the 2026 Xingkun Cup and later used for research on mobile aiming stability.

## Platform

- **Chassis:** TI MSPM0G3507 with PID speed control, line tracking, and IMU heading control.
- **Vision:** Raspberry Pi 5 and OpenCV for target detection, perspective correction, and circular trajectory generation.
- **Gimbal:** STM32-controlled GM6020 motors, CAN communication, and IMU feedback for stabilized aiming.
- **Hardware:** Custom chassis, controller expansion board, and power distribution board.

<p align="center">
  <img src="1.jpg" alt="Assembled AutoaimCar platform" width="58%">
  <img src="2.jpg" alt="AutoaimCar during a laser aiming test" width="32%">
</p>

![Comparison of the 2025 and 2026 robots](3.jpg)

**Left: 2025 version. Right: 2026 upgrade.** The comparison reflects our progress in mechanical design, hardware integration, and control.

## Research

Using this platform, we compared direct visual and gyroscope-assisted dual-loop PID for mobile aiming under obstacle impacts, motivated by firefighting applications. Gyroscope assistance reduced mean peak aiming errors across all tested conditions; RMSE performance depended on the terrain. See the [research poster](Research_poster.pdf) for methods and results.

## Repository

- [Car_TI_M0/](Car_TI_M0/): Chassis firmware.
- [Pi5_CV/](Pi5_CV/): Vision and trajectory generation code.
- [STM32/](STM32/): Gimbal motor control and debugging code.
- [Report.pdf](Report.pdf): Competition report (Chinese).
- [Research_poster.pdf](Research_poster.pdf): Research summary and results.

## License

[GNU GPL v3.0](LICENSE) (GPL-3.0-only). Third-party components retain their original licenses.
