cmake_minimum_required(VERSION 3.8)
project(Fanorona_7_0_0_DEFINITIVO)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        dati_globali.h
        funzioni.c
        funzioni_file.c
        generale.c
        interfacce.c
        main.c)

add_executable(Fanorona_7_0_0_DEFINITIVO ${SOURCE_FILES})