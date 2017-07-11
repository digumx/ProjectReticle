# ProjectReticle

An open source, multiplatform, multithreaded, advanced, real-time rendering api based on OpenGL.

## Features:

Some features that reticle has, or aims to have:

 -  **Open Source:**    Not really a feature, but yeah, it's open source.
 -  **Real-Time:**      This means that, under sufficiently modern hardware, Reticle will render frames within 16.66 ms, making 
                        Reticle ideal for Games, interactive simulations, etc.
 -  **Advanced:**       Reticle will not just be a simple OpenGL renderer - it will pack features like _PBR_, _Deffered Shading_,
                        _Extended lights_, _Fast soft shadowing_, _Realtime Reflections_, and a lot more. That is to say, Reticle
                        aims at making rich visuals with high graphical fidelity.
 -  **Multi-Platform:** The code written in Reticle is completely platform agnostic - it does not depend on any platform specific
                        feature, API, or any other such thing. Just build for your target platform, and the code works out of the 
                        box. With that said, currently, the code has only been tested for Windows 8.1, and Ubuntu Linux 16.04 LTS.
                        More OS-es will be added to this list.
 -  **Multi-Threaded:** Reticle is designed to effectively use multiple CPU cores by running in multiple threads in a very dynamic
                        and scalable way. Most realtime rendering engines use only upto 4 hardware threads, while modern CPUs can
                        offer upto 16 or more. And, currently, CPU manufactureres are pushing for more CPU cores with better multi-
                        threading rather than makaing faster CPUs with same number of cores. Reticle can take advantage of this 
                        treand and utilize the cores modern CPUs promise.
 -  **Easy to use:**    Reticle is be a powerful, yet simple and easy to use API that abstracts all the nitty gritty of the 
                        features it implements from the developer that is using the API so you can focus on ther parts of yourproject. So, if you are writing a Game, you need to only care about Physics, Game logic etc, and leave the rendering to Reticle.
                        
## Dependencies:
 
   To provide a wide ganut of filetype support, consistency, cross platform efficiency, etc, Reticle uses a number of third party open source libraries. Currently, precompiled binaries of these libraries are included with Reticle in the Dependencies folder, and Reticle.cbp is configured to link to these. However, the user may build Rericle to library binaries they have built themselves by downloading their sources. A list of libraries used:
 
 -  **OpenGL:**     Used as a 3D rendering API.
 -  **GLEW:**       To manage extensions to OpenGL, and obtain pointers to OpenGL functions in a cross platform manner.
 -  **GLFW:**       To make and manage windows, and to handle keyboard and mouse input.
 -  **stb_image:**  For providing support for a large variety of image filetypes. Check stb_image's page for a list of                             filetypes supported.
 -  **assimp;**     To provide support for a large variety of 3D asset file formats. See assimp's page for list.
 -  **glm:**        As a math library.
 
## License:
 
   Reticle, including the example resources it comes with but excliding it's dependencies is licensed under the MIT License, which allows free redistribution, modification and inclusion in commercial and nkn-commercial projects. See LICENSE.md for details.
   
   This license does not cover the licensing of the dependencies of Reticle, see their individual license files for details.

## Setup Instructions:
 
   Currently, Reticle comes as a CodeBlocks project, configured by default to compile for linux. A cmake build system will be 
 added in the near future. Building the Reticle project on Windows can be done by reconfiguring the CodeBlocks project. Note that by default, Reticle compiles as a shared object on Linux, and a static library on Windows. Also, Reticle links to precompiled binaries of it's delendenies. These are placed in the Dependencies folder. The Reticle binaries are bu default in the bin/Debug and bin/Release folders.
 
 ### Linux:
 
 **Building:** The Reticle project builds out of the box on linux. Just open the .cbp file in CodeBlocks and build the targets. By default, Reticle compiles to a shared object file on linux.
 
 **Linking:** To use Reticle, add tbe patb to the .so file to the linker search directory list, and link to it with -lReticle.
 
 ### Windows:
 
 **Building:** On windows, reconfigure the Reticle.cbp file so that the build target is a static library, remove all linking options and all non relative compiler and linker search directories. In includes.h, change #define PLATFORM_LINUX to PLATFORM_WNDOWS.
 
 **Linking:** Link to the Reticle.a file, glfw3, glew32s and assimp.a. Also link to your global OpenGL, and libgdi32 files.
 
 ## Using Reticle:
 
 After Reticle has been successfully built and linked to, start writing some simple code with the following steps:
 
  -  Add the the Headers and Includes directory to your project's compiler search directory list.
  -  Include reticle.h.
  -  Before creating any Reticle objects, or calling any functions, call RetiRenderer::setReticleRootDiretory() with a string path to the directory named Reticle with the Sources, Headers, etc subdirectories. This may iether be an absolute path, or a path relative to the cwd at call time. This string may be formatted according to OS specifications.
  -  Then call RetiRenderer::init().
  -  Write the applicaion code.
