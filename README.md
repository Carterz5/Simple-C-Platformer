Compilation instructions

* Ubuntu (24.04)
```
sudo apt install build-essential
sudo apt install libglew-dev
sudo apt install libglfw3
sudo apt install libglfw3-dev
sudo apt install libcglm-dev
sudo apt install libopenal-dev
sudo apt install libalut-dev
git clone https://github.com/Carterz5/Cnake.git

cd Cnake
make

```

* Windows

install MSYS2 https://www.msys2.org/
```
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-glew
pacman -S mingw-w64-ucrt-x86_64-glfw
pacman -S mingw-w64-ucrt-x86_64-cglm
pacman -S mingw-w64-ucrt-x86_64-openal
pacman -S mingw-w64-ucrt-x86_64-freealut
pacman -S make
pacman -S git

git clone https://github.com/Carterz5/Simple-C-Platformer.git

cd Cnake


make
```