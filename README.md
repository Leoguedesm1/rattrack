This is a OPENCV and QT project.

Basically, the program identify a rat in video and start tracking his path.

PROGRAMS VERSION:

    - OPENCV: 3.0 (for installation i recommend "http://rodrigoberriel.com/2014/10/installing-opencv-3-0-0-on-ubuntu-14-04/" ATTENTION: The 'WITH-QT' flag must be disabled!)
    - QTCreator: 4.0 to 4.8 (ATTENTION: versions above 5.0 don't work!) link here: https://download.qt.io/archive/qt/4.8/4.8.6/
    - CMake: 3.0 or better
    
HOW TO COMPILE:

    1) Using terminal, enter in your project path
    2) create build directory and enter it
    3) $ qmake  ../sourcebuild/-project
    4) $ make
    5) run executable
    
    EXAMPLE:
    
    1) $ cd ../rattrack/QT
    2) $ mkdir build && cd build
    3) $ qmake ../QT/rattrack.pro
    4) $ make
    5) $ ./rattrack