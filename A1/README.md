# Assignment 1

## Implementation Specifications:
Develop an OpenGL application with the following functionality and features:
- Creates a 100x100 square grid (ground surface) in the XZ plane centered at the origin.
- Creates a set of three lines 5 grid units in length, in 3 different colors, representing each coordinate axis in virtual world space, centered at the origin.
- Creates a model of the Olaf (a snowman) like the one depicted in the figure by suitably transforming one unit cube to create different body parts. The Olaf is initially positioned at the center of the grid facing along X axis. The body parts should be independent parts so they can be rotated/moved on their own. We recommend that you use hierarchical modelling, so that a single transformation applied to Olaf’s origin will apply to all of Olaf. 
- Places a virtual camera with the world space origin as the point of focus.
- For display and animation:
  - Create a GLFW window of size 1024x768 with double buffering support.
  - Render the coordinate axis, ground and Olaf in the window.
  - The application should use a perspective view to display all the objects and enable hidden surface removal. 
- The application should handle the following input:
  - Pressing the spacebar should re-position the Olaf at a random location on the grid.
  - The user can incrementally size up the Olaf by pressing ‘U’ for scale-up and ‘J’ for cale-down. Each key press should result in a small size change.
  - The user can control the Olaf position and orientation using keyboard input i.e. A → ove left, D → move right, W → move up, S → move down, a → rotate left 5 degrees out Y axis, d → rotate right 5 degrees about Y axis. You may add other rotations bout other axis, if you want.
  - The world orientation is changed by using keyboard input i.e. left arrow → Rx, right rrow → R-x, up arrow → Ry, down arrow → R-y. (Rx denotes a small anti-clockwise rotation about positive x axis, R-x about negative x axis, etc.) Pressing “Home” button hould reset to the initial world position and orientation.
  - The user can change rendering mode for the car, i.e. points, lines, triangles based on keyboard input i.e. key ‘P’ for points, key ‘L’ for lines, key ‘T’ for triangles.
  - The user can pan and tilt the camera as follows:
    - While right button is pressed → use mouse movement in x direction to pan; and
    - While middle button is pressed → use mouse movement in y direction to tilt.
  - The user can zoom in and out of the scene - while left button is pressed → use mouse movement to move into/out of the scene.
  - The application should use OpenGL 4.1 and onwards and must include brief comments explaining each step. 
