# Simulation-Design Course at ODU
Simulation accomplished by CPP
### Project 1
A raw first person shooting game includes rigid-body pyhsics, so it can rotates and detect collision. 
![shooter](https://user-images.githubusercontent.com/56880104/128375858-a87f490a-5990-4b8c-9e4d-c3532c2a3bc0.gif)

### Project 2

Develeped the QuatApp showing 3 rotation spheres that are named as: sun (in red color), earth(in blue color), moon( in yellow color) with OpenGL visulization. Sun sphere has self-rotation, along with the moon and earth. Earth rotates around the sun while the moon rotates around earth. They are achieved by 3D rotation martrix. The app is based on the Cyclone Engine framework. 

![Orbit](https://user-images.githubusercontent.com/56880104/128252246-ce728c3a-518f-42e0-9d89-deede41f6069.gif)



### Task One: 

To develop a Monte Carlo simulation that simulates the occurrence of chip defects in the CPU chip manufacturing process. 

__Wafer specification:__
* 12 cm diameter wafer
 
* Chip sizes to be studied: 1x1 cm2, 1.5x1.5 cm2, 2x2 cm2

* Defects are uniformly distributed: A single defect on a chip results in a defective chip

* Note that a defect may not fall on a chip

* Cost to manufacture a single wafer: $5,000

### Task Two: PID Controller

Simulate the process of a PID controller.

Record every state at each timestamp.

Modifiy the Kp, Ki, Kd values to see the performace changes. 

### Task Three: Car following algorithm

Rather than try to maintain a constant speed, we are going to follow a car, trying to maintain a safe distance, defined as a time gap (allows a safe distance at various speeds)

##### Available Sensors

* Distance to previous car (d = x1(t) – x2(t))

* Current velocity (V)

##### Inputs

* Desired time gap between cars (T)

* Sensitivity (β): how quickly the car reacts to changes.

### Task Four: A discrete event simulation engine. 

Use linked list to achieve event arrival, event departure. 
