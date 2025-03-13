@echo off
REM Create a build directory if it doesn't exist
if not exist build (
    mkdir build
)
cd build

REM Configure the project with Ninja generator
cmake -G Ninja ..

REM Build the project
cmake --build .

REM Pause to see output
cd ..
pause
