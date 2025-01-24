# Указываем имя проекта модуля
project(UDPClientWindow)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем пути к исходным файлам
set(SOURCES udpclientwindow.cpp)
set(HEADERS udpclientwindow.h)

# Генерируем файл метаобъектов (для сигналов и слотов Qt)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Создаем статическую библиотеку
add_library(UDPClientWindow STATIC ${SOURCES} ${HEADERS})

# Указываем, где искать заголовочные файлы
target_include_directories(UDPClientWindow 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}  # Для udpclientwindow.h
        ${CMAKE_SOURCE_DIR}/UDPLib   # Для udplib.h
)

# Подключаем Qt и UDPLib
target_link_libraries(UDPClientWindow PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui UDPLib)