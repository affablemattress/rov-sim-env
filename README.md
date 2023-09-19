# rov-sim-env

A simulation environment for our ROV project at IEEE METU RAS.

### Why?

Alright. This won't be easy and will take a couple of months (at least). But I've been dragging my feet about writing a proper desktop program using OpenGL and Dear IMGUI for months now. It's finally the perfect time to do it.

### What?

Yeah, we have a ROV project going on, little guy is just a cube at this point and will stay a cube for an undetermined period of time (forever?). It'll have 6 (or 8?) motors vector-thrust for propulsion, will have a camera for computer vision applications, and a microphone array for locating audio sources.

The system consists of: AHRS (using the on board IMU & barometer), computer vision (with the help of a danky & it's low-res image), sound source location (using 3, hopefully low cost, underwater mics & a ton of processing), communications and telemetry (with a RS-432 UART line, wireless  not possible casue water is mean to smol antennas), land computer UI (one of us knows some C# so... we'll go with a Windows forms applet I guess, smh...), power distribution (we'll use a bunch of 18650 and have cooked our little distrubituon PCB), **and -finally- control (this part is where this project comes in)**.

My plan is to write a RK-4 physics engine in order to model the behaviour of the ROV with a (hopefully) nice visualisation using OpenGL. In order to realistically model the crafts behaviour I'll emulate the control code of the ROV (which runs on a Cortex M4 STM32 dev. board IRL) along with the AHRS data. I have access to the full control source code of the ROV, so emulation shouldn't be a problem. But emulating the sensor input will be something I've never tried before, should be fun.

## How
* OpenGL with GLFW for cross compatibility w/Windows & Mac. We need cross compatibility since we're a team of 8 and I want everyone to be able to run this.  
* I want to be able to import the CAD model of the craft for modelling.  
* Physics should include drag forces and should support partial submersion of the craft. And of course motors should work like real motors, another thing I'm not sure how to implement.
* I'll use Dear IMGUI for GUI. A GUI will be essential for tuning the control parameters and such.
