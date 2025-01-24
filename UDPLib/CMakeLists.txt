# Указываем имя проекта модуля
project(UDPLib)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем пути к исходным файлам
set(SOURCES udplib.cpp)
set(HEADERS udplib.h)

# Создаем статическую библиотеку
add_library(UDPLib STATIC ${SOURCES} ${HEADERS})

# Подключаем Boost
target_include_directories(UDPLib PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(UDPLib PUBLIC ${Boost_LIBRARIES})