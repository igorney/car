project(car)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp
                               car.cpp starlayers.cpp)

enable_abcg(${PROJECT_NAME})
