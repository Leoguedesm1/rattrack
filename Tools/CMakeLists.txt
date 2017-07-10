cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
<<<<<<< HEAD:CMakeLists.txt
add_executable( calib calib_teste.cpp )
target_link_libraries( calib ${OpenCV_LIBS} )

=======
add_executable( tool videotool.cpp )
target_link_libraries( tool ${OpenCV_LIBS} )
>>>>>>> e0d8e4863ea75826192d991baa1bd0cbdbf75250:Tools/CMakeLists.txt
