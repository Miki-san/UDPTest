# ��������� ��� ������� ������
project(UDPClientWindow)

# ��������� �������� C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ��������� ���� � �������� ������
set(SOURCES udpclientwindow.cpp)
set(HEADERS udpclientwindow.h)

# ���������� ���� ������������ (��� �������� � ������ Qt)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ������� ����������� ����������
add_library(UDPClientWindow STATIC ${SOURCES} ${HEADERS})

# ���������, ��� ������ ������������ �����
target_include_directories(UDPClientWindow 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}  # ��� udpclientwindow.h
        ${CMAKE_SOURCE_DIR}/UDPLib   # ��� udplib.h
)

# ���������� Qt � UDPLib
target_link_libraries(UDPClientWindow PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui UDPLib)