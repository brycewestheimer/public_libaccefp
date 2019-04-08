cmake_minimum_required (VERSION 3.12.0)
PROJECT(ACCEFP)

######################
#set CMAKE build type#
# -default to release#
######################

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message("-- CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

############
#enable c++#
############

enable_language(CXX)

##################
#check for OpenMP#
##################

find_package(OpenMP)
if(OPENMP_FOUND)
   message("-- Detecting OpenMP with ${CMAKE_CXX_COMPILER_ID} CXX compiler - done")
else()
   message(FATAL_ERROR "Compiler does not support OpenMP.")
endif()

#####################################
#setup general c++ compilation flags#
#####################################

set(CMAKE_CXX_FLAGS "-std=c++11 ${OpenMP_CXX_FLAGS}")

########################
#disable GPU by default#
########################

option(HAVE_CUDA "Disables GPU code"  OFF)

######################################
#enable GPU if GPU board is specified#
######################################

if(GPU_BOARD)
   message("-- A GPU board was specified. GPU code is enabled.")
      set(HAVE_CUDA  ON)
else() 
	message("-- A GPU board was not specified. GPU code is disabled.")
endif(GPU_BOARD)

################################
# setup for GPU if requested   #
#  -must have at least cuda 6.0#
################################

if(HAVE_CUDA)

   SET(CUDA_SEPARABLE_COMPILATION ON)
   find_package(CUDA 6.0 REQUIRED)
   enable_language(CUDA)
   set(CMAKE_CUDA_COMPILER nvcc)
   set(CMAKE_CUDA_COMPILER_ENV_VAR "CUDACXX")

   string(TOUPPER ${GPU_BOARD} GPU_BOARD)

   set(CMAKE_CUDA_FLAGS "-rdc=true --ptxas-options=-v ")
   if(${GPU_BOARD} MATCHES FERMI)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch=sm_20 ")
   elseif(${GPU_BOARD} MATCHES KEPLER)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch=sm_35 ")
   elseif(${GPU_BOARD} MATCHES PASCAL)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch=sm_60 ")
   elseif(${GPU_BOARD} MATCHES VOLTA)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch=sm_70 ")
   else()
    message(FATAL_ERROR "Unrecognized CUDA board: ${GPU_BOARD}")
   endif()
   set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xptxas -dlcm=cg -maxrregcount=80 ")
   set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -L${CUDA_TOOLKIT_ROOT_DIR}/lib64/ -lcublas_device")

   option(HAVE_GPU "enable GPU" ON)
   option(HAVE_CUBLAS "enable cublas" ON)
   set(HAVE_GPU_STRING "1")
   set(HAVE_CUBLAS_STRING "1")
endif(HAVE_CUDA)

####################
##set math library##
####################

string(TOUPPER ${MATHLIB} MATHLIB)
message("-- Detecting Math Library: ${MATHLIB}") 

#set the math library
if(${MATHLIB} STREQUAL MKL)
   set(HAVE_MKL 1)
   set(MKL_STRING "1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMKL_ILP64")
elseif(${MATHLIB} STREQUAL ATLAS)
   set(HAVE_ATLAS 1)
   set(ATLAS_STRING "1")
elseif(${MATHLIB} STREQUAL OPENBLAS)
   set(HAVE_OPENBLAS 1)
   set(OPENBLAS_STRING "1")
elseif(${MATHLIB} STREQUAL ESSL)
   set(HAVE_ESSL 1)
   set(ESSL_STRING "1")
else()
   message(FATAL_ERROR "Math library not indicated! Please select the proper vendor and location for the math library installation.")
endif()

#set math library include directory
set(MATHLIB_INCLUDE_PATH ${MATHLIB_ROOT}/include)
message("-- Math Library include directory: ${MATHLIB_ROOT}") 

#########
#set MPI#
#########

if("${MPI_ROOT}" STREQUAL "") 
   message("-- Searching for MPI")
   find_package(MPI REQUIRED)
elseif(EXISTS ${MPI_ROOT})
   if(EXISTS ${MPI_ROOT}/include64)
      set(MPI_CXX_INCLUDE_PATH ${MPI_ROOT}/include64)
   else()
      set(MPI_CXX_INCLUDE_PATH ${MPI_ROOT}/include)
   endif()
else()
   message(FATAL_ERROR "MPI not found! Please use the -DMPI_ROOT flag to select an MPI for installation.")
endif()

message("-- MPI include directory: ${MPI_CXX_INCLUDE_PATH}")
if(EXISTS ${MPI_CXX_INCLUDE_PATH})
   option(HAVE_MPI "enable mpi" ON)
   message("-- Found MPI")
   message("   ** MPI include directory: ${MPI_CXX_INCLUDE_PATH}")
endif(EXISTS ${MPI_CXX_INCLUDE_PATH})

###################################
#set addtional include directories#
###################################

set(DTL_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dtl)

########################
#set additional options#
########################

# option(OPTION_NAME "corresponding string" ON)

##########################################################
#setup c++ compilation flags for release or debug version#
##########################################################

if(CMAKE_BUILD_TYPE MATCHES Release)
   set(CMAKE_CXX_FLAGS_RELEASE "-O3") 
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fpic")
   message("-- WARNING: compiler flags modified for debug version.")  
elseif (CMAKE_BUILD_TYPE MATCHES MinSizeRel)
   set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
   message("-- WARNING: compiler flags modified for minimum-size version.") 
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
   set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O3 -fpic")
   message("-- WARNING: compiler flags modified for debuggable release version.")  
endif()

if(BUILD_ALL MATCHES 1)
  set(BUILD_EFP 1)
  set(BUILD_EFMO 1)
  set(BUILD_MAKEFP 1)
endif()

if(BUILD_EFP MATCHES 1)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DBUILD_EFP")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBUILD_EFP")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DBUILD_EFP")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DBUILD_EFP")
endif()

if(BUILD_EFMO MATCHES 1)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DBUILD_EFMO")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBUILD_EFMO")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DBUILD_EFMO")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DBUILD_EFMO")
endif()

if(BUILD_MAKEFP MATCHES 1)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DBUILD_MAKEFP")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBUILD_MAKEFP")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DBUILD_MAKEFP")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DBUILD_MAKEFP")
endif()

#########################
#set include directories#
#########################

INCLUDE_DIRECTORIES(
                     ${PROJECT_SOURCE_DIR}/include/
                     ${MATHLIB_INCLUDE_PATH}
                     ${PROJECT_SOURCE_DIR}/src/
                     ${CUDA_TOOLKIT_INCLUDE}
                     ${MPI_CXX_INCLUDE_PATH}
                     ) 

###########################################
# TODO: CONTINUE MODIFYING SECTIONS BELOW #
###########################################

######################################
# Create include files from templates#
######################################

configure_file (
  "${PROJECT_SOURCE_DIR}/config/config.h.cmake.in"
  "${PROJECT_SOURCE_DIR}/config/config.h"
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/src/basis/config.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/basis/config.hpp"
  )

###############
#step into src#
###############

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(src/basis)
ADD_SUBDIRECTORY(src/integrals)
ADD_SUBDIRECTORY(src/bindings)
ADD_SUBDIRECTORY(src/array)

if(BUILD_HF MATCHES 1)
  ADD_SUBDIRECTORY(src/hf)
endif()

if(BUILD_MP2 MATCHES 1)
  ADD_SUBDIRECTORY(src/mp2)
endif()

if(BUILD_RI MATCHES 1)
  ADD_SUBDIRECTORY(src/densityFitting)
  ADD_SUBDIRECTORY(src/ri-mp2)
endif()

if(BUILD_CC MATCHES 1)
  ADD_SUBDIRECTORY(src/cc)
endif()

#############
#set targets#
#############
set(cchemTargets
 $<TARGET_OBJECTS:base>
 $<TARGET_OBJECTS:basis>
 $<TARGET_OBJECTS:integrals>
 $<TARGET_OBJECTS:bindings>
 $<TARGET_OBJECTS:array>
)

if(BUILD_HF)
 message("-- Adding HF module.") 
 list(APPEND cchemTargets
 $<TARGET_OBJECTS:hf>
 )
endif()

if(BUILD_MP2)
 message("-- Adding MP2 module.")
 list(APPEND cchemTargets
 $<TARGET_OBJECTS:mp2>
 )
endif()

if(BUILD_RI)
 message("-- Adding Density-Fitted HF module.")
 list(APPEND cchemTargets
 $<TARGET_OBJECTS:densityFitting>
 )
 message("-- Adding RI-MP2 module.")
 list(APPEND cchemTargets
 $<TARGET_OBJECTS:ri-mp2>
 )
 if(HAVE_CUDA)
  list(APPEND cchemTargets
  $<TARGET_OBJECTS:ri-mp2_kernel>
  )
 endif()
endif()

if(BUILD_CC)
 message("-- Adding CCSD(T) module.")
 list(APPEND cchemTargets
 $<TARGET_OBJECTS:cc>
 )
endif()

########################
#setup libcchem library#
########################

add_library(cchem ${cchemTargets})
if(HAVE_CUDA)
 set_target_properties(cchem PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
endif()

SET_TARGET_PROPERTIES(cchem PROPERTIES LINKER_LANGUAGE CXX)


#####################
#extra debug options#
#####################

IF(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

 get_cmake_property(_variableNames VARIABLES)
 list (SORT _variableNames)
 foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "environment")

ENDIF(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

######################
#set install location#
######################

install (TARGETS cchem DESTINATION ${PROJECT_SOURCE_DIR}/lib)

########################
#set up testing options#
########################
include(CTest)

add_test(NAME scf-cyt COMMAND ${PROJECT_SOURCE_DIR}/../tests/libcchem/scf/scripts/scf-cyt)

