# ProjectReticle

An open source, multiplatform, multithreaded, advanced, real-time rendering api based on OpenGL.

### Features:

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
                        features it implements from the developer that is using the API so you can focus on ther parts of your
                        project. So, if you are writing a Game, you need to only care about Physics, Game logic etc, and leave 
                        the rendering to Reticle.
