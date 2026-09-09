# Install script for directory: C:/ncs/v3.4.0/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Zephyr-Kernel")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/ncs/toolchains/dcbdc366a1/opt/zephyr-sdk/gnu/arm-zephyr-eabi/bin/arm-zephyr-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/azure-sdk-for-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/cirrus-logic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/nrf_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/hostap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/canopennode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/chre/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/cmsis-dsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/cmsis-nn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/cmsis_6/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/dhara/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/hal_st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/hal_tdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/libsbc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/percepio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/cmake/reports/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim/zephyr/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
