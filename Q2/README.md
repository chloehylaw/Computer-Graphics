# Quiz 2
## Tasks
-  Briefly (but identifiably) cite all the resources used (e.g., online as URLs or offline as book
or article titles, notes, own project etc.)
- Certain tasks need to be done in order, but some are not. Read through them all; do the
sequences you absolutely need and the ones you are most comfortable with first, and then
do as many as you can: 

## Requirements
- Replace the three axes X, Y, Z each with a stretched cube and a cone on top. Apply a
texture to each of the axis matching their respective colors. Apply a texture to the ground
mesh (you must make use of the Repeat texture parameter) as well. (6 Pts)
- Using Quiz 1 Timex approach, model and animate first 2 characters of your first name
and last two digits of your student ID. For instance, David with an ID 2623456 should
model and animate characters D5A6. Front face of each letter/digit must be transparent
(Use a suitable alpha value). (6 Pts)
  1. The entire model should be centered at x = 0; y = 0; z = 0.
  2. The entire model should be on a skateboard which is to be modeled using cubes.
  3. Model one skateboard for the entire model which should help the model move.
  4. You are encouraged to use your cube utility functions/methods/classes from A1, Q1.
  5. Apply a distinct texture to each letter/digit and a metallic texture to the skateboard,
and a glossy texture to the wheels. 

- Apply a suitable texture to the sky cube (large cube modeled in Q1) from inside. (2 Pts)
- Add a light source (make sure it is a spotlight, using the Phong illumination model) and
place it 30 units out on the z-axis facing the model. Allow for it to be turned on and off
with ‘L’. Ambient light is necessary to be turned on when the main light source at the
top is off. (4 Pts)
- Add one camera onto the model’s front; unobscured; disable/enable it using the key ‘M’
(first person view). Add another one at the back (opposite), with key ‘B’. Now you have
three cameras (the default main and the 2 with the model.). ‘R’ from Quiz 1 resets back
to the main camera among other things. (6 Pts)
- Add another camera which will circle around the center of the scene using arrow keys.
Attach a spotlight to this camera which will move with the camera and can be toggled
using a key. Choice of type of light, material properties, and color is left to you. Circling
camera’s Y and the look at point must remain constant. (5 Pts)
- Render the scene’s letters/digits and the new axes with shadows using two pass shadow
algorithms. (Define a key to toggle it on and off). (6 Pts)
- The application must use OpenGL 4.1 and onwards and must include brief comments
explaining each step. Grader will ask for a small modification to understand your grasp
of your code. (5 Pts)
