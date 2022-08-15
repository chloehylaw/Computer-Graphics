# Quiz 1
## Tasks

- Creates a 100x100 square grid (ground surface) in the XZ plane centered at the origin.
- Creates a set of three lines 5 grid units in length, in 3 different colors, representing each
coordinate axis in virtual world space, centered at the origin.
- Places a virtual camera with the world space origin as the point of focus.
- For display and animation:
- Create a GLFW window of size 1024x768 with double buffering support.
- Render the coordinate axis, ground and all the models in the window.
- The application should use a perspective view to display all the objects and enable
hidden surface removal. 

## Requirements
- Create a large enough cube to occupy the entire grid and set the color to sky blue. Your
models as well as camera will be placed inside this cube which is your world. (2 Pts)
- For this quiz, you are required to design six alphabets from your last name. If your last
name is shorter than 6 characters, borrow from your first name and your ID if the first
name is also not long enough. 

  1. Design one skateboard each for every character. Every character must be placed on
its respective skateboard. You may create a basic model for the skateboard with four
cubic wheels and a flattened cubic plank. (5Pts)
  2. Model the six characters and place them on the corresponding skateboards. (5 Pts)
  3. You may make use of cube utility functions/methods/classes from your assignment
and design all the characters in the old Timex watch display style. Please refer to
https://fontmeme.com/fonts/gau-font-cube-font/ for shape related help. 

- The application should handle following input for each character. 
  1. Each character should be selected by pressing a key from 1 to 6. (1 Pt)
  2. The user can rotate each character in clockwise or anti-clockwise direction around
Y-axis which is pointing upwards. Assign apt keys for rotational purposes; however,
each character needs to be independently rotatable and of distinct color. (2 Pts)
  3. The user can control camera position while looking at each character using keys 1
to 6. You must adjust the camera position as well as look-at-point to the center of
the character you are looking at. (2 Pts) 

- The application must use OpenGL 4.1 and onwards and must include brief comments
explaining each step. Grader will ask for a small modification to understand your grasp
of your code. (3 Pts)
