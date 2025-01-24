# Указываем имя проекта модуля
project(Server)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем пути к исходным файлам
set(SOURCES server.cpp)

# Создаем исполняемый файл для сервера
add_executable(Server server.cpp)

# Указываем директории для заголовочных файлов
target_include_directories(Server 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}         # Для client.cpp
        ${Boost_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/UDPLib          # Для udplib.h
)

# Связываем сервер с библиотекой UDPLib и Boost
target_link_libraries(Server PUBLIC UDPLib ${Boost_LIBRARIES})
