# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim")
  file(MAKE_DIRECTORY "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim")
endif()
file(MAKE_DIRECTORY
  "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/xiao_flow_sim"
  "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix"
  "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix/tmp"
  "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix/src/xiao_flow_sim-stamp"
  "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix/src"
  "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix/src/xiao_flow_sim-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix/src/xiao_flow_sim-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/fuent/Desktop/samples_test_app/xiao_flow_sim/build/_sysbuild/sysbuild/images/xiao_flow_sim-prefix/src/xiao_flow_sim-stamp${cfgdir}") # cfgdir has leading slash
endif()
