cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(xiao_flow_sim)

target_sources(app PRIVATE src/main.c)
