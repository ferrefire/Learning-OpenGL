This is a project I'm working on to learn OpenGL.

------------------------------------------------

How to build

1. Clone the repository.
2. Create a 'build' directory inside te cloned repository.
3. Execute 'cmake ..' inside the build directory.
4. Linux: Execute 'make' or 'make -j' for parralel building.
4. Windows: Execute 'cmake --build . --configure Release'.

-------------------------------------------------

How to run

Linux: Execute './learningOpenGL'.

Windows: Execute 'start .\Release\learningOpenGL'.

-------------------------------------------------

Command arguments

'FULL': Launch the program in fullscreen at full resolution.
'CIN=': The program will play a cinematic at launch and print the average FPS afterwards. Some pre-existing cinematics are: "grass", "testCin", "generalQuick".
'MAKECIN=': Create your own cinematic by pressing right mouse button to place a interpolation point. Press Q to save.

-------------------------------------------------

Controls

WASD: relative horizontal movement.
LEFT CTRL and SPACE: relative vertical movement.
MOUSE SCROLL WHEEL: increase or decrease movement speed.
Q: exit the program.
V: enable or disable vsync.
L: enable or disable wireframe mode.

-------------------------------------------------

Notes

This project is absolutely not finished.
When using windows, the terrain generation is not 100% working and can show straight lines.
On Nvidia graphics cards, the terrain generation is working even worse and can result in visual artefacts.
On Nvidia graphics cards, the grass does not work correctly and will result in very bad performance when viewed from up close.

-------------------------------------------------
