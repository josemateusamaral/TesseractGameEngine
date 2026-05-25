  Tesseract Game Engine is a software rendering game engine that intends to provide an easy way to render large scenes using just the CPU. It is fully made using C++ and SDL2, so it can run pretty much everywhere.<br><br>
  The engine is in its beginning state, so it will take some time to be really useful.<br><br>
  The engine was born in the course of computer science at the Intituto Intituto Federal Catarinense on the campus of Blumenau. It was initially intended to be an assessment method for object-oriented programming.<br><br>


  <br>
  RUN TEST SAMPLE ON WINDOWS<br>

  - Install MSYS2 on https://www.msys2.org/.<br>
  - Using MSYS2 UCRT terminal install the necessary to compile the project using the commands bellow: <br>
      __> pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain<br>
      __> pacman -S mingw-w64-ucrt-x86_64-cmake<br>
      __> pacman -S mingw-w64-ucrt-x86_64-SDL2<br>
      __> pacman -S mingw-w64-ucrt-x86_64-assimp<br>
      __> pacman -S mingw-w64-ucrt-x86_64-SDL2_ttf<br>
  - Then add the path C:\msys64\ucrt64\bin to your environment variables.<br>
  - Now you should be able to run ./test.bat to run the test.cpp example and also use tools like gcc and g++. This is the easiest way to start working on windows without have to set too many things.<br>

<br><br>
RUN TEST SAMPLE ON UBUNTU<br>

  - Run the commands bellow to install the necessary librarys.<br>
      __> sudo apt install cmake<br>
      __> sudo apt install libsdl2-dev<br>
      __> sudo apt install libassimp-dev<br>
      __> sudo apt install libsdl2-ttf-dev<br>
  - Now you should be able to run sh ./test.sh to run the test.cpp example. This is the easiest way to start working on Ubuntu without have to set too many things.<br>




