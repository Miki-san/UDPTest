# Указываем имя проекта модуля
project(Client)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем пути к исходным файлам
set(SOURCES client.cpp)

# Генерируем файл метаобъектов (для сигналов и слотов Qt)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Создаем исполняемый файл
add_executable(Client ${SOURCES})

# Указываем пути к заголовочным файлам
target_include_directories(Client 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}         # Для client.cpp
        ${CMAKE_SOURCE_DIR}/UDPClientWindow # Для udpclientwindow.h
)

# Подключаем Qt и UDPClientWindow
target_link_libraries(Client PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui UDPClientWindow)
