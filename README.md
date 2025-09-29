## Simple little 3D Renderer

I made this this past week to try to learn more about rendering and graphics, etc (never actually got to take a class on it).
It uses SDL3 to create a window with a frame buffer, but all of the rendering and rasterization is done by my terribly inefficient code.

Currently, if you run `make` and then execute `bin/ba.exe`, it will show only one object ([The Utah Teapot](https://users.cs.utah.edu/~dejohnso/models/teapot.html)). 
You can move around with WASD and Space+Left Control.

Things to follow (?):
- Mouse control for angling camera
- Maybe reorganize everything cuz I kinda don't like it rn.
- Add rotation/transpose matrix to object class so models can be placed
- Lighting (we'll see!)
- Mess around with adding another spatial dimension (again, we'll see!)

