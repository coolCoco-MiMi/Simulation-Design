# Simulation-Design
Simulation accomplished by CPP

### Taks One: 

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

* Sensitivity (β): how quickly the car reacts to changes
