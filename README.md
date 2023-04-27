# FLOWER

This project, titled flower, allows for the user to interact with the window 
interface by using the mouse to draw lines. The goal here is to lead the path of
the water stream coming from the watering can to the flower pot/vase in order to
water the flower. The title of the mini game at the center of the screen, "FLOWER",
is used as an obstruction where the user cannot draw a path through it, it allows
for the user to get creative when drawing a path for the water.

A game that generates a watering can (at the top left corner of the screen), a vase with flowers (at the bottom right corner of the screen), and an obstruction (in the middle of the screen). Water particles fall from the watering can as soon as the program is run. The user then draws a path/ramp anywhere on the screen that there is open space and leads water from the watering can to the flower vase. The water flows down the drawn path and, hopefully, fills the vase.

TODO: Replace "TODO" messages with your own text
TODO: Add a description of your project here
TODO: Add a representative image for your project here

## How the program works
The program uses different elements to draw the scene, such as the water can, flower,
ground, and drawer meshes. These objects along with the cubes and spheres are paired
with their respective shaders in order to colorize the game into a monochrome format.
This simple monochrome design was used to emulate a corloized version of monchrome
pixel games from the past. A slight influence of "pixels" was used as I have always
been interested in pixel games along with my other interest of flowers and the flow
of water. This program is specifically unique from existing demos as it combines
the current pixel games with my interests. This game will also be combining and
building on top of the ideas/assignments from class: blobilism, pixmaps, and
particles.

The program incorporates blobilism as it allows the user to draw on the screen a
path from the watering can to the flowers. Pixmaps has a slight inflluence as the
areas of the window are tirangulated to locate the vase of the flowers where the
water droplets should go in. Particles is used to create the stream of water which is then repeated once it reached either the ground or the flower vase.

The math involved with this program is mostly involved with calculating the way
the water flows from the watering can. First of all in relation to the water
droplets just free falling, a variable of gravity is used to accelerate the water
droplet along the y-axis to simulate a bit of real water. Then when the user draws
a path, the program calculates whether a water droplet is colliding with the path
to then adjust the water droplet's position and velocity accordingly. The collision
is detectted by taking into account the size of the droplet and its position and
the width of the path and its position to then see if either coincides wiht the
other, if it does then that means the two elements have collided. This then means
the water droplet's position is readjusted to go along with the path aka it roles
along the path as it readjusts its position and velocity.

If the water droplet reaches the ground then the it "dies" and respawns at the
start of the watering can's spout. If it reaches the vase then it collects up into 
vase until 10 particles are collected then an object known as the water level rises
to act as if the vase is being filled with water. After the object rises to the
height of the vase it then stops filling up the vase even if more water comes in.


## How to build

*Windows*

Open git bash to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ start project-template.sln
```

Your solution file should contain four projects.
To run from the git bash command shell, 

```
project-template/build $ ../bin/Debug/demo.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
project-template $ mkdir build
project-template $ cd build
project-template/build $ cmake ..
project-template/build $ make
```

To run each program from build, you would type

```
project-template/build $ ../bin/demo
```


## Demo of basic features

TODO: Document the main demos your viewer 
![Image_1](images/horizontal-line.png)

## Unique features 

TODO: Show the unique features you made

