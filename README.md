  Tesseract Game Engine is a software rendering game engine that intends to provide an easy way to render large scenes using just the CPU. It is fully made using C++ and SDL2, so it can run pretty much everywhere.<br><br>
  The engine is in its beginning state, so it will take some time to be really useful.<br><br>
  The engine was born in the course of computer science at the Intituto Instituto Federal Catarinense on the campus of Blumenau. It was initially intended to be an assessment method for object-oriented programming.<br><br>

  RUN TESTE SAMPLE<br>
 ./test.bat<br>

  <video src="[https://drive.google.com/uc?export=download&id=SEU_ID_AQUI](https://drive.google.com/file/d/1aLPpXzN54Vi1J7_5zMhwwL5-e3bVoVs-/view?usp=drive_link)" width="600" controls>
    Seu navegador não suporta vídeos.
  </video>

  <br><br><br>
  COMPILING TIPS FOR WINDOWS<br>

  - The CMakeLists.txt was made to be used with cmake on windows.<br>
  - Install MSYS2 on https://www.msys2.org/.<br>
  - Using MSYS2 UCRT terminal install the necessary to compile the project using the commands bellow: <br>
  __> pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain<br>
  __> pacman -S mingw-w64-ucrt-x86_64-cmake<br>
  __> pacman -S mingw-w64-ucrt-x86_64-SDL2<br>
  - Then add the path C:\msys64\ucrt64\bin to your environment variables.<br>
  - Now you should be able to run ./test.bat to run the teste.cpp example and also use tools like gcc and g++. This is the easiest way to start working on windows without have to set too many things.<br> 




