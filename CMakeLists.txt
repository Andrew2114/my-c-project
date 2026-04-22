cmake_minimum_required(VERSION 3.10)
project(MyCProject VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Основная программа
add_executable(my_program src/homework_1/main.c)

# Тесты
add_executable(tests src/homework_1/test.c)

# Флаги компиляции
target_compile_options(my_program PRIVATE -Wall -Wextra)
target_compile_options(tests PRIVATE -Wall -Wextra)

# Сообщения для отладки
message(STATUS "Building my_program from src/homework_1/main.c")
message(STATUS "Building tests from src/homework_1/test.c")
