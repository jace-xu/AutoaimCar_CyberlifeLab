# AutoaimCar | Cyberlife Lab

An autonomous aiming robot that grew from a student competition entry into a research platform. Built by our team at South China University of Technology, it combines mobile navigation, computer vision, and a stabilized two-axis gimbal.

## From Competition to Research

### 1. Summer 2025: Our First Robot

We built our first version for Problem E of the 2025 National Undergraduate Electronic Design Contest. This was the starting point for our work on mobile aiming and gave us practical experience in integrating mechanics, embedded control, and vision.

### 2. 2026: A Better Competition Platform

For a similar university competition, the Xingkun Cup (Problem D), we developed an improved robot with a custom chassis and circuit boards, Raspberry Pi vision processing, and a brushless gimbal. It follows a track while aiming a laser at a target or tracing a circular path on it.

![Our 2025 robot and the upgraded 2026 platform](3.jpg)

**Left: the 2025 old version. Right: the upgraded 2026 robot.** The comparison shows how much our engineering skills have grown in mechanical design, hardware integration, vision, and motion control.

### 3. Beyond Competition: Mobile Aiming Research

We then used the upgraded car as an experimental platform to study aiming stability for mobile firefighting robots. This extended the project from completing competition tasks to comparing control methods through repeatable experiments.

## Research: Aiming Stability Under Obstacle Impacts

**Study:** *Design of a Firefighting Robot: An Experimental Comparison of Direct Visual and Gyroscope-Assisted Dual-Loop PID.*

Ground obstacles can shake a moving robot and disturb its aim. We compared two controllers on the same platform:

- **Direct visual dual-loop PID:** Uses image-position error to command motor speed.
- **Gyroscope-assisted dual-loop PID:** Converts visual error into angle commands and uses inertial feedback to stabilize the gimbal between visual updates.

Tests covered flat ground and obstacles of **5, 7.5, and 10 mm**, with **three trials per group**. We evaluated aiming RMSE, peak error, and success rate using an AprilTag target as a simulated fire source.

The gyroscope-assisted controller produced lower mean peak errors in all four conditions. Direct visual control had lower RMSE on flat ground and the two smaller obstacles, while gyroscope-assisted control had lower RMSE on the highest obstacle. The results highlight a trade-off between average tracking accuracy and resistance to sudden disturbances.

This study focused on aiming performance; testing with real water discharge and actual fire sources remains future work. See the [research poster](Research_poster.pdf) for the experimental setup, plots, and results.

## Platform

- **Chassis:** TI MSPM0G3507, encoder feedback, PID speed control, grayscale line tracking, and IMU heading control.
- **Vision:** Raspberry Pi 5, Python, and OpenCV for target detection, perspective correction, and circular trajectory generation.
- **Gimbal:** STM32, GM6020 brushless motors, CAN communication, PID control, IMU feedback, and a slip ring for continuous 360-degree rotation.
- **Hardware:** A SolidWorks-designed chassis, custom controller expansion board, and power distribution board.

<p align="center">
  <img src="1.jpg" alt="Assembled AutoaimCar platform" width="58%">
  <img src="2.jpg" alt="AutoaimCar during a laser aiming test" width="32%">
</p>

## Repository

- [Car_TI_M0/](Car_TI_M0/): Chassis control firmware.
- [Pi5_CV/](Pi5_CV/): Competition vision code for target tracking and circular trajectories.
- [STM32/](STM32/): Gimbal motor control source code, including CAN communication, PID speed control, and VOFA+ debugging.
- [Report.pdf](Report.pdf): Xingkun Cup project report (Chinese).
- [Research_poster.pdf](Research_poster.pdf): Follow-up research methods and experimental results.

## License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0-only). See [LICENSE](LICENSE) for the full text.

Third-party components retain their original licenses.
