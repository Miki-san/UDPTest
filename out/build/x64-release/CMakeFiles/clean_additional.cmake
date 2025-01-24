# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "Client\\CMakeFiles\\Client_autogen.dir\\AutogenUsed.txt"
  "Client\\CMakeFiles\\Client_autogen.dir\\ParseCache.txt"
  "Client\\Client_autogen"
  "UDPClientWindow\\CMakeFiles\\UDPClientWindow_autogen.dir\\AutogenUsed.txt"
  "UDPClientWindow\\CMakeFiles\\UDPClientWindow_autogen.dir\\ParseCache.txt"
  "UDPClientWindow\\UDPClientWindow_autogen"
  )
endif()
