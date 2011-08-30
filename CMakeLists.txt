project(kdroid)

cmake_minimum_required(VERSION 2.8)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( src )
