cmake_minimum_required(VERSION 3.15)

# Устанавливаем имя проекта
project(UDPTest)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь к Boost
set(Boost_ROOT "C:/Libraries/boost_1_87_0")
set(Boost_INCLUDE_DIR "C:/Libraries/boost_1_87_0")
set(Boost_LIBRARY_DIR "C:/Libraries/boost_1_87_0/stage/lib")
set(Boost_USE_STATIC_LIBS ON) # Используем статические библиотеки Boost
set(Boost_USE_MULTITHREADED ON) # Используем многопоточность
set(Boost_USE_STATIC_RUNTIME OFF)

# Подключение Boost
set(Boost_NO_WARN_NEW_VERSIONS ON)
find_package(Boost REQUIRED COMPONENTS system)
if (NOT BOOST_FOUND)
    message(FATAL_ERROR "Boost is required but was not found.")
endif()

# Указываем путь к Qt5
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")

# Подключение Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)
if (NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt5 is required but was not found.")
endif()

# Добавляем поддиректории
add_subdirectory(UDPLib)
add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(UDPClientWindow)