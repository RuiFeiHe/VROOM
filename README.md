# VROOM: Interactive rendering virtual environment with binocular vision.
Course Project, CS 21120970 Mixed Reality, ZJU, 2019 Winter. Instructor: Hujun Bao


浙江大学 计算机学院 专题研讨（混合现实）课程作业


##  Demo

![image](https://github.com/RuiFeiHe/VROOM/blob/master/img/VROOM.jpg)

Watch on youtube: https://www.youtube.com/watch?v=lPWbokYvWAI&feature=youtu.be



## Functions

1. Read the scene's model data from files to realize each link of the graphics pipeline;

2. Provide simultaneous drawing of left and right binocular images;

3. Provide a scene with multiple objects, use keyboard to assist mouse to interactively select objects or light sources, change the orientation and drawing parameters of the selected objects and light sources;

4. Be able to draw the texture and shadow of the virtual scene;

5. Use the mouse to realize the free flight roaming function of the camera.



## Details

### .h files

"Camera.h": the `Camera` class and `Two_camera` class are defined, which can realize the coordination between the camera and the callback function in the main program, achieve the camera's free roaming flight and binocular camera.

"Mesh.h": the mesh class is defined to serve the model class in model.h.

"Model.h": defines the model class, which is used to read in and load the. obj file loaded by the model loading library assimp mode.

"Room.h": a sphere class is defined for lamp drawing.

"Shader_s.h": defines a shader class, which is used to draw shaders on pipeline.

"stb_image.h": official document, used for texture use.



### GLSL files

“lamp.fs”，“lamp.vs”：shader source code for lighting
“shadow.fs”，“shadow.vs”：shader source code for shadow map generation
“shader.fs”，“shader.vs”：shader source code for final painting



### .c files

“glad.c”
“main.cpp”

