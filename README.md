# Computer Graphics & Integrated Systems Projects

This repo features 3 separate programming ecercises revolving around familiarization with OpenGL 3.3. A detailed readme (GR) can be found for each exercise in their respective folders.

<br>

## OpenGL - A

<img alt="demo img here"  src="https://raw.githubusercontent.com/Zengineers/cg-is-projects/main/opengl-a/2d.gif?token=GHSAT0AAAAAABOKBCNWURYE7P2LH7FLPIZ4YPVSYOQ"
width = "250"
height = "250"/>

The goal of this exercise was to draw 2D shapes on a window using the
[GLFW](https://www.glfw.org/), 
[GLM](https://glm.g-truc.net/0.9.9/index.html), and
[GLEW](http://glew.sourceforge.net/) libraries as well as a fragment shader and a vertex shader.
 
 <br>

 ## OpenGL - B

<img alt="demo img here"  src="https://raw.githubusercontent.com/Zengineers/cg-is-projects/main/opengl-b/pyramid.gif?token=GHSAT0AAAAAABOKBCNWJIGYUP6MZW3M2AI2YPVSYCA"
width = "250"
height = "250"/>

 The goal of this exercise was to dynamically draw a 3D pyramid on a window (using the abovementioned libraries).
 The pyramid as well as the camera are manipulated using basic transformations (translation, rotation and scaling).

<br>

 ## OpenGL - C

<img alt="demo img here"  src="https://raw.githubusercontent.com/Zengineers/cg-is-projects/main/opengl-c/solar-system.gif?token=GHSAT0AAAAAABOKBCNXCI2OUSFGGXKG5TDAYPVSXPQ"
width = "250"
height = "250"/>

 The goal of this exercise was to create 3D replica scene of a solar system using OpenGL 3.3 (as well as the abovementioned libraries). 3D sphere objects were loaded from .obj files. Textures were loaded from .jpg files and applied to the objects. Camera and objects are manipulated using basic transformations. The user can spawn a meteor which starts from the camera position and travels towards the sun of the solar system. Collision detection is also implemented between the meteor and the planets/sun.