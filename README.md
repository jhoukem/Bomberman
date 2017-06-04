# Bomberman
The old fashion bomberman game. Developed in C for learning purposes.  

## Licence
This project is under the [GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/) license.  

## Installation Guide
I will only talk about **windows's users** since Linux's users should already know how to do this.  
To compile the sources, you should have **MinGW** installed on your machine as well as **SDL2**, **SDL2_images**, **SDL2_ttf** and **SDL2_mixer** libraries.  
Refers to google for installation informations or maybe go to [this tutorial](https://www.caveofprogramming.com/c-for-complete-beginners/setting-up-sdl-windows.html)

### Eclipse project configuration
Create a new C project (if the choice is given, select the MinGW compiler). Next, fetch my repository sources using a git client (I personally use [this one](https://git-for-windows.github.io/)):   
`$ git clone http://github.com/jhoukem/Bomberman`  
Then extract all the files from the repository to your project root:  
`$ mv Bomberman/* my_project_root_path/`  

### Includes configuration
**Go to:** Project -> Properties -> C/C++ Build -> Settings  
In the *tool settings* tab, expand the **GCC C Compiler option**. Click on the *'Includes'* folder from the sources. 
In the include paths window, add the *'headers/'* folder.  

### C Linker Configuration  
**Still on:** Project -> Properties -> C/C++ Build -> Settings  
In the '*Tool settings*' tab, expand the **MinGW C Linker option**. Click on the *'Librairies'* folder. Then on the libraries windows, add the following libraries (the order is important):  
- mingw32  
- SDL2main  
- SDL2mainSDL2_image  
- SDL2_ttf  
- SDL2_mixer  
- pthread  

This should be sufficient to compile the sources (if you have installed the SDL2 libraries on your computer) but if you want your *'.exe'* file to have a icon (which is nicer) then you will have to follow these two more steps:  

### Resources file configuration (for exe icon)
**Still on:** Project -> Properties -> C/C++ Build -> Settings  
Go to the *'Build Steps'* tab and fill the command input field with the following line:  
`windres ../rsc/ressources.rc ../rsc/ressources.o`  
This will create a binary resource for the compiler so it can load the icon into the *'.exe'* file.  
Go back to the '*Tool Settings*' tab and in the **MinGW C Linker**, under *Miscellaneous*, add a new object with the path to the resource we created:  
`../rsc/ressources.o`  
(I used *'../rsc*' because the compiler current directory will be in the *Release/Debug* folder in our project so we have to go back once to be able to reach the *'rsc'* folder).  
And... That's it!

You are all done with the project configuration. Now go ahead and compile the sources (Ctrl + Maj + B).
Feel free to add any improvement to the game and make me know about it.

## Export the game to a PE (portable executable)
The *'.exe'* file need the SDL's dll to be in the same folder in order to run. Create a new folder and add the *'.exe'* file, the *'rsc'* folder and all the SDL dll.

Now you can just zip this folder and share it to a friend.

## Demo  

![demo1](https://cloud.githubusercontent.com/assets/9862039/26765895/d716d76e-4953-11e7-9dcf-7d65f140232d.png)
![demo2](https://cloud.githubusercontent.com/assets/9862039/26765896/d71a575e-4953-11e7-928b-e2ad7540caf3.png)
![demo3](https://cloud.githubusercontent.com/assets/9862039/26765894/d7136d7c-4953-11e7-8525-96e1097c62a8.png)