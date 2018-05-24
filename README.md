CppMicroserviceExampleGenerator
==================

Introduction
------------

When trying out the
[CppMicroServices Examples](http://docs.cppmicroservices.org/en/stable/doc/src/getting_started.html), I found myself having trouble working with the provided samples or even more creating my own sample project to test out the provided functionality.

This being mainly due to me being a college student and not having years of experience in c++ and CMake I thought some kind of example project generator could come in handy. So this is mainly thought for programmers checking out CppMicroservices for the first time and being a little overwhelmed by the examples provided.

Following is a step by step guide for creating the simplest service I could think of.

Requirements
------------
- Visual Studio 2017
- CMake (I use 3.9 or above)
- [CppMicroservices](https://github.com/CppMicroServices/CppMicroServices/releases/tag/v3.3.0)

Supported Platforms
-------------------

Not much I can say here. The project was created using Windows, other platforms I haven't tried.
(I will eventually but then again I have exams to pass)

Quick Start
-----------
I will assume that you have configured and built CppMicroservices in a seperate folder e.g. D:\CppMicroServices_bin

First you should clone this repository:
`<git clone https://github.com/Sinalcoco/CppMicroserviceExampleGenerator.git>`
Feedback regarding coding style, bugs or feature request are very welcome!

Configuration using CMake and building should be straightforward.

When done you should have a CppMicroserviceExampleGenerator.exe under D:\CppMicroserviceExampleGenerator_bin\src\Debug (or any other of course I just have to assume one for the sake of this guide)

I recommend copying the exe as well as the template folder (found in D:\CppMicroserviceExampleGenerator) to a fresh directory (I'll copy them straight to D:\).

Now open up a command prompt and start the program as follows:
`<CppMicroserviceExampleGenerator Example template>`
The first argument is the name of the project you'd like to generate.
The second argument is the path to the template directory. In case this isn't in the same folder I highly recommend using the absolute path.

Now you should see a directory called ExampleProject which contains the following:
- Example_dir (directory containing the service interface)
- ExampleConsumer_dir (directory containing a service consumer)
- ExampleImplementation_dir (directory containing a specific implementation of the service interface that can be retrieved later)
- CMakeLists.txt (project configuration)
- main.cpp (used for loading our bundles, installing and starting them)

Now configure the ExampleProject using CMake.
It will prompt you to specify the CppMicroService_Dir (in my case D:/CppMicroServices_bin). 
Now hit generate and open the project.

Right click ALL_BUILD and hit build.

Now the only thing left to do is to tell Visual Studio where the CppMicroService dll is located and what dlls (our bundles) to load.
So right click ExampleProject and go to settings->Debugging.

There in Arguments you have to provide the paths to our bundle dlls.
For me it looks like this: `<D:\ExampleProject_bin\Debug\ExampleImplementation.dll D:\ExampleProject_bin\Debug\ExampleConsumer.dll>`

In Environment you have to add the CppMicroService dll to the PATH variable like so: `<PATH=D:\CppMicroServices_bin\bin\Debug;%PATH%>`.

Last two steps are to set the ExampleProject as startup project and place a breakpoint on the line with `<return 0;>` in the main.cpp

If all has worked starting the project should open a terminal displaying "Ping received!".

I know it seems kinda lame but hold on.

If you take time inspecting the created files you'll find theres a nice strong coupling between the implementation and the service consumer. The functionality should be easy to expand.

I might provide an in-depth explanation of what the lines of code do but for now you'll have to stick with the [CppMicroServices Getting Started Section](http://docs.cppmicroservices.org/en/stable/doc/src/getting_started.html) which I think is describing what is happening really well.

In case of further questions or suggestions please feel free to let me know.
