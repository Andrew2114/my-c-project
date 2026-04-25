cmake_minimum_required(VERSION 3.10)
project(MyCProject VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# CSV Printer (твое задание)
add_executable(csv_printer src/homework_csv-printer/main.c)
add_executable(csv_tests src/homework_csv-printer/test.c src/homework_csv-printer/main.c)
target_compile_definitions(csv_tests PRIVATE TEST_MODE)

# Остальные задания (если нужно)
add_executable(avl_tree src/homework_AVL-tree/main.c)
add_executable(automaton src/homework_avtomat-for-num/main.c)
add_executable(states src/homework_gosudarstva/main.c)
# Основная программа
add_executable(my_program src/homework_csv-printer/main.c)

add_executable(my_program src/homework_avtomat-for-num/main.c)
add_executable(my_program src/homework_AVL-tree/main.c)
add_executable(my_program src/homework_gosudarstva/main.c)

# Тесты - компилируем с флагом TEST_MODE
add_executable(tests src/homework_csv-printer/test.c src/homework_csv-printer/main.c)
target_compile_definitions(tests PRIVATE TEST_MODE)

# Флаги
target_compile_options(csv_printer PRIVATE -Wall -Wextra)
target_compile_options(csv_tests PRIVATE -Wall -Wextra -Wno-unused-but-set-variable)
target_compile_options(avl_tree PRIVATE -Wall -Wextra)
target_compile_options(automaton PRIVATE -Wall -Wextra)
target_compile_options(states PRIVATE -Wall -Wextra)
