# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_Database Database/*.cpp Database/*.h)
file(GLOB_RECURSE sources_Entities Entities/*.cpp Entities/*.h)
file(GLOB_RECURSE sources_Client Client/*.cpp Client/*.h)
file(GLOB_RECURSE sources_Common Common/*.cpp Common/*.h)
file(GLOB_RECURSE sources_Grid Grid/*.cpp Grid/*.h)
file(GLOB_RECURSE sources_Packet Packet/*.cpp Packet/*.h)
file(GLOB_RECURSE sources_Server Server/*.cpp Server/*.h)
file(GLOB_RECURSE sources_Tools Tools/*.cpp Tools/*.h)

set(EPS_SRCS
  ${EPS_SRCS}
  ${sources_Database}
  ${sources_Entities}
  ${sources_Client}
  ${sources_Common}
  ${sources_Grid}
  ${sources_Packet}
  ${sources_Server}
  ${sources_Tools}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/dep/poco/Net/include
  ${CMAKE_SOURCE_DIR}/dep/poco/Foundation/include
  ${CMAKE_SOURCE_DIR}/dep/poco/Data/include
  ${CMAKE_SOURCE_DIR}/dep/poco/Data/MySQL/include
  ${CMAKE_SOURCE_DIR}/dep/cryptocpp

  ${CMAKE_SOURCE_DIR}/dep/sparsehash/src
  
  ${CMAKE_CURRENT_SOURCE_DIR}/Database
  ${CMAKE_CURRENT_SOURCE_DIR}/Entities
  ${CMAKE_CURRENT_SOURCE_DIR}/Client
  ${CMAKE_CURRENT_SOURCE_DIR}/Common
  ${CMAKE_CURRENT_SOURCE_DIR}/Grid
  ${CMAKE_CURRENT_SOURCE_DIR}/Packet
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${CMAKE_CURRENT_SOURCE_DIR}/Tools
  
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_definitions(-DPOCO_STATIC)

link_directories(${CMAKE_SOURCE_DIR}/dep/poco/lib)

add_executable(EtrinumProjectServer
  ${EPS_SRCS}
)

target_link_libraries(EtrinumProjectServer
  cryptlib
  libmysql
  ${OPENSSL_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

install(TARGETS EtrinumProjectServer DESTINATION "${CMAKE_INSTALL_PREFIX}")