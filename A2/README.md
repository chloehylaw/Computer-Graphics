# Assignment 2
## Implementation Specifications

1. Extend your OpenGL Assignment 1 with the following functionality and features:
- Make the head and body of the Olaf spherical (use triangle mesh representations)
- Illuminate the scene by adding a point light source (white) 30 units above the Olaf using the Phong model
- Render the scene with snow texture on the ground mesh, carrot texture on the nose and shiny metal finish (color of your choice) for the hat (Key X for toggling texture on/off)
- Render the scene with shadows using two pass shadow algorithm (Key B to toggle)
- Make Olaf move by moving the legs, while also providing lateral or vertical movement to the hands. (One key of your choice for one small forward movement, and other key for continuous forward movement. Same for reverse movement).
- Make the Olaf turn right or left by a small amount (choose your own keys). 

2. Please note that the above are extensions to your program and previous key presses (of A1 must work as before, but with the extended rendering as required in this assignment.
- Pressing the spacebar should re-position the Olaf at a random location on the grid.
- The user can incrementally size up the Olaf by pressing ‘U’ for scale-up and ‘J’ for scale-down. Each key press should result in a small size change.
- The user can control the Olaf position and orientation using keyboard input i.e. A → move left, D → move right, W → move up, S → move down, a → rotate left 5 degrees about Y axis, d → rotate right 5 degrees about Y axis. You may add other rotations about other axes if you want.
- The world orientation is changed by using keyboard input i.e., left arrow → Rx, right arrow → R-x, up arrow → Ry, down arrow → R-y. (Rx denotes a small anti-clockwise rotation about positive x axis, R-x about negative x axis, etc.). Pressing the “Home” button should reset to the initial world position and orientation.
- A user can change rendering mode for the Olaf, i.e., points, lines, or triangles, based on keyboard input i.e., key ‘P’ for points, key ‘L’ for lines, key ‘T’ for triangles.
- The user can pan and tilt the camera as follows:
- While right button is pressed → use mouse movement in x direction to pan; and
- While middle button is pressed → use mouse movement in y direction to tilt.
- The user can zoom in and out of the scene - while left button is pressed → use mouse movement to move into/out of the scene.
- Window resize handling: The application should handle window resize events and correctly adjust the aspect ratio accordingly. This means, the meshes should not be distorted in anyway. 

3. The application must use OpenGL 4.1 and up and must also include brief comments explaining each step.
