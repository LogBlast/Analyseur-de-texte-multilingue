cmake_minimum_required(VERSION 3.28)
project(fisa34_projet4 C)

set(CMAKE_C_STANDARD 11)

add_executable(fisa34_projet4 main.c
        analyseur.c
        analyseur.h
        langue.c
        langue.h)
