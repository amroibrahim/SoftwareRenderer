# 3D Software Renderer
3D Software Renderer built from scratch in C++

## Introduction to a Software Renderer Built from Scratch in C++
A software renderer is a programmatic approach to generating computer graphics entirely on the CPU, without relying on a GPU or external libraries like OpenGL or DirectX. Building one from scratch in C++ is both an educational and challenging endeavor that delves deep into the fundamentals of computer graphics, linear algebra, and system-level programming.

This software renderer is designed as a minimalistic yet powerful engine for rendering 3D graphics. It processes vertices, transforms geometry, and rasterizes pixels directly to an image buffer. By working at this low level, you gain complete control over the rendering pipeline, including transformations, shading, texture mapping, and lighting calculations.

Key Features:
Customizable Rendering Pipeline: Implements essential stages such as vertex transformation, clipping, and pixel rasterization.
Basic Shading Models: Supports flat shading for lighting effects.
Texture Mapping: Handles texture sampling and mapping with interpolation techniques.
Depth Buffering: Ensures correct pixel visibility using a z-buffer algorithm.

## Why Build a Software Renderer?
Developing a software renderer is a great way to:
* Understand the inner workings of modern graphics APIs like Vulkan or DirectX.
* Explore optimization techniques and data structures for high-performance computing.
* Dive into low-level concepts such as clipping, rasterization, and perspective correction.


| Scene    | Details | Notes   |
| -------- | ------- | ------- |
| 00 - Empty  | Empty scene just to make sure basic app functionality is working |        |
| 01 - Points Cloud | Validate the ability to basic pixel drawing |         |
| 02 - Triangle | Validate the ability to draw triangle to screen | multiple algorithms implemented, can only be switched by modifying SDL_Draw_Helper.cpp |
| 03 - Wireframe | Basic OBJ file parsing and 3D object rendering |         |
| 04 - Flat Shading | Basic flat shading implementation |         |
| 05 - Texture Mapping | Texture mapping implementation |         |
| 06 - FPS Camera | Basic camera controls implementation | WASD Keys + Mouse look supported (hold right click) |

## Screens
![Screen1](https://github.com/amroibrahim/SoftwareRenderer/blob/master/img/Scene_01.gif)  
![Screen2](https://github.com/amroibrahim/SoftwareRenderer/blob/master/img/Scene_03.gif)  
![Screen3](https://github.com/amroibrahim/SoftwareRenderer/blob/master/img/Scene_04.gif)  
![Screen4](https://github.com/amroibrahim/SoftwareRenderer/blob/master/img/Scene_05_1.gif)
![Screen4](https://github.com/amroibrahim/SoftwareRenderer/blob/master/img/Scene_05_2.gif)  

## Future Work
The next step will be to modify the existing software renderer to adopt a right-handed coordinate system, aligning it with the conventions used in OpenGL. This transition will involve carefully updating how geometric data is interpreted, as well as modifying the handling of lighting, depth calculations, and other transformations to match the right-handed coordinate conventions used in OpenGL.

## Reference 
https://github.com/ssloy/tinyrenderer/wiki  
https://www.youtube.com/watch?v=ih20l3pJoeU  


