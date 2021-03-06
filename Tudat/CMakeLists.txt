 #    Copyright (c) 2010-2015, Delft University of Technology
 #    All rights reserved.
 #
 #    Redistribution and use in source and binary forms, with or without modification, are
 #    permitted provided that the following conditions are met:
 #      - Redistributions of source code must retain the above copyright notice, this list of
 #        conditions and the following disclaimer.
 #      - Redistributions in binary form must reproduce the above copyright notice, this list of
 #        conditions and the following disclaimer in the documentation and/or other materials
 #        provided with the distribution.
 #      - Neither the name of the Delft University of Technology nor the names of its contributors
 #        may be used to endorse or promote products derived from this software without specific
 #        prior written permission.
 #
 #    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 #    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 #    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 #    COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 #    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 #    GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 #    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 #    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 #    OF THE POSSIBILITY OF SUCH DAMAGE.
 #
 #    Changelog
 #      YYMMDD    Author            Comment
 #      110820    S.M. Persson      File created.
 #      111025    K. Kumar          Adapted file to work with Revision 194.
 #      111027    K. Kumar          Updated to auto-search for Eigen3 and Boost; manual setting
 #                                  examples included as comments. Added suppression of warnings
 #                                  from Boost.
 #      111028    K. Kumar          Corrected Boost variables and changed build-type to Debug.
 #      111103    K. Kumar          Updated CMake directory name in module path.
 #      111117    K. Kumar          Added system and filesystem Boost components.
 #      170212    S. Billemont      Added support for UserSettings.txt file.
 #      120505    K. Kumar          Removed superfluous include sub-directory macro.
 #      120913    K. Kumar          Added output message for build type and set default to
 #                                  "Release".
 #      120921    M.I. Ganeff       Modified for SpiceInterface.
 #      130204    K. Kumar          Removed obsolete TudatCoreEnvironment.cmake code block.
 #      130325    K. Kumar          Added warning message in case Boost version is 1.52 and GCC
 #                                  version is 4.4 for Legendre polynomials code.
 #
 #    References
 #
 #    Notes
 #

# Specify minimum CMake version required.
cmake_minimum_required(VERSION 2.6)

# Specify project name.
project(Tudat)

# Set module path to local CMake scripts.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/External/CMake)

# Set root-directory for code to current source directory.
set(CODEROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

# Define the basic input (trunk).
set(SRCROOT "${CODEROOT}/Tudat")

# Set testing options based on platform.
enable_testing()

# Set platform-specific compiler flags.
if (WIN32)
  if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /Ox /W3 /FC -D_SCL_SECURE_NO_WARNINGS")
    # Because we are using static boost libraries, with static runtime, we need to force MSVC to
    # also use static runtime: (from http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace).
    foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      # Find all dynamic runtime (MD) references and replace with static (MT)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
  else()
    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -Wall -g -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor")
  endif()
else()
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wall -g -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor")
endif()

# -------------------------------------------------------------------------------------------------
# DO NOT MANUALLY EDIT ANY SETTINGS AFTER THIS POINT IN THE FILE.
# 
# Pass any additional settings directly to CMake (e.g., via command-line) or by using a config file
# (e.g., UserSettings.txt).
# -------------------------------------------------------------------------------------------------

# Load user specific settings. These can overwrite all previous settings.
if (NOT TUDAT_USER_SETTINGS AND NOT DEFINED ENV{TUDAT_USER_SETTINGS})
  # Load from default paths, least important first.
  include("../UserSettings.txt" OPTIONAL)
  include("UserSettings.txt" OPTIONAL)
else()
	MESSAGE(STATUS "Loading preset...")
  # Load the user specified file.
  if (TUDAT_USER_SETTINGS)
    include($TUDAT_USER_SETTINGS OPTIONAL)
  else()  
    include($ENV{TUDAT_USER_SETTINGS} OPTIONAL)
  endif()
endif()

# Set CMake build-type. If it not supplied by the user (either directly as an argument of through
# the "UserSettings.txt" file, the default built type is "Release". NB: It is important this
# section is placed AFTER the "UserSettings.txt" file is included, to ensure that that the correct
# indiciation is given for which build type has been selected.
if((NOT CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message(STATUS "WARNING: building release version!")
    set(CMAKE_BUILD_TYPE Release)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "WARNING: building debug version!")
endif()

# Set lib and bin directories where static libraries and unit tests are built.
if(NOT LIB_ROOT)
  if(TUDAT_BUILD_DIR)
    set(LIB_ROOT "${TUDAT_BUILD_DIR}/lib")
  else( )
    set(LIB_ROOT "${PROJECT_BINARY_DIR}/../lib")
  endif( )
endif( )
if(NOT BIN_ROOT)
  if(TUDAT_BUILD_DIR)
    set(BIN_ROOT "${TUDAT_BUILD_DIR}/bin")
  else( )
    set(BIN_ROOT "${PROJECT_BINARY_DIR}/../bin")
  endif( )
endif( )

# Set the global macros for setting up targets.
macro(setup_tudat_library_target target_name CUSTOM_OUTPUT_PATH)
  set_property(TARGET ${target_name} PROPERTY LIBRARY_OUTPUT_DIRECTORY "${LIB_ROOT}")
  set_property(TARGET ${target_name} PROPERTY ARCHIVE_OUTPUT_DIRECTORY "${LIB_ROOT}")
endmacro(setup_tudat_library_target)

macro(setup_custom_test_program target_name CUSTOM_OUTPUT_PATH)
  set_property(TARGET ${target_name} PROPERTY RUNTIME_OUTPUT_DIRECTORY "${BIN_ROOT}/unit_tests")
  get_property(CUSTOM_TEST_PROGRAM_NAME TARGET ${target_name} PROPERTY OUTPUT_NAME)
  add_test("${target_name}" "${BIN_ROOT}/unit_tests/${target_name}")
endmacro(setup_custom_test_program)

# Define the install target to create a distribution of Tudat.
if(NOT TUDAT_BUNDLE_DISTRIBUTION_PATH)
    set(TUDAT_BUNDLE_DISTRIBUTION_PATH "${CODEROOT}/tudatBundle")
endif(NOT TUDAT_BUNDLE_DISTRIBUTION_PATH)

if(NOT TUDAT_DISTRIBUTION_PATH)
    set(TUDAT_DISTRIBUTION_PATH "${TUDAT_BUNDLE_DISTRIBUTION_PATH}/tudat")
endif(NOT TUDAT_DISTRIBUTION_PATH)

# Install the Tudat files.
install(DIRECTORY "${SRCROOT}/"
        DESTINATION "${TUDAT_DISTRIBUTION_PATH}/Tudat"
        PATTERN ".DS_STORE" EXCLUDE
        PATTERN "CMakeLists.txt.user" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN ".git" EXCLUDE
        PATTERN ".bzr" EXCLUDE
)

install(FILES ${CODEROOT}/LICENSE ${CODEROOT}/NOTICE 
              ${CODEROOT}/README ${CODEROOT}/UserSettings.txt.example
        DESTINATION "${TUDAT_DISTRIBUTION_PATH}")

# Set the main sub-directories.
set(ASTRODYNAMICSDIR "/Astrodynamics")
set(BASICSDIR "/Basics")
set(EXTERNALDIR "/External")
set(INPUTOUTPUTDIR "/InputOutput")
set(MATHEMATICSDIR "/Mathematics")

# Include the top-level directories.
include_directories(AFTER
  "${CODEROOT}"
)

# Find Eigen3 library on local system.
find_package(Eigen3 REQUIRED)

# Include Eigen3 directories.
# Set CMake flag to suppress Eigen warnings (platform-dependent solution).
if(NOT APPLE)
  include_directories(SYSTEM AFTER "${EIGEN3_INCLUDE_DIR}")
else( )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${EIGEN3_INCLUDE_DIR}\"")
endif( )

# Configure Boost options.
if(NOT Boost_USE_STATIC_LIBS)
  set(Boost_USE_STATIC_LIBS ON)
endif()
if(NOT Boost_USE_MULTITHREADED)
  set(Boost_USE_MULTITHREADED ON)
endif()
if(NOT Boost_USE_STATIC_RUNTIME)
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

# Find Boost libraries on local system.
find_package(Boost 1.45.0 
             COMPONENTS date_time system unit_test_framework filesystem regex REQUIRED)

# Include Boost directories.
# Set CMake flag to suppress Boost warnings (platform-dependent solution).
if(NOT APPLE)
  include_directories(SYSTEM AFTER "${Boost_INCLUDE_DIRS}")
else( )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${Boost_INCLUDE_DIRS}\"")
endif( )

# Check if Boost version is 1.52 and emit warning statement for Legendre polynomial code under 
# GCC 4.4.
if(Boost_VERSION EQUAL 105200)
  if(CMAKE_CXX_COMPILER_VERSION MATCHES ".*4\\.4.*")
    set(BOOST_GCC_WARNING "Detected Boost 1.52, this version is known to be incompatible with")
    set(BOOST_GCC_WARNING "${BOOST1520_GCC44_WARNING} the LegendrePolynomials under GCC 4.4.")
    message(WARNING "${BOOST_GCC_WARNING}")
  endif()
endif()

# Add an option to toggle the generation of the API documentation.
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)

# If documentation should be built, find Doxygen package and setup config file.
if(BUILD_DOCUMENTATION)
  find_package(Doxygen)

  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
            "Doxygen is needed to build the documentation. Please install it correctly.")
  endif()

  # Configure the Template Doxyfile for our specific project.
  configure_file(${CODEROOT}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)

  # Add a custom target to run Doxygen when ever the project is built.
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
  add_custom_target (docs ALL COMMAND ${DOXYGEN_EXECUTABLE} 
                     ${PROJECT_BINARY_DIR}/Doxyfile 
                     SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)

  # Install documentation.
  install(DIRECTORY "${CODEROOT}/Documentation" 
          DESTINATION "${TUDAT_DISTRIBUTION_PATH}")
endif()

# Set whether to use the SPICE library integration with Tudat or not. If it not supplied by the
# user (either directly as an argument or through the "UserSettings.txt" file, the default setting
# is "OFF". NB: It is important this section is placed AFTER the "UserSettings.txt" file is
# included, to ensure that that the correct indiciation is given for whether SPICE should be
# included or not has been selected.

# Add an option to toggle the inclusion of SPICE.
if(NOT USE_CSPICE)
  message(STATUS "SPICE disabled!")
elseif(USE_CSPICE)
  message(STATUS "SPICE enabled!")

  # Find CSPICE library on local system.
  find_package(Spice)

  # Include CSpice directories.
  if(NOT APPLE)
    include_directories(SYSTEM AFTER "${SPICE_INCLUDE_DIR}")
  else( )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem \"${SPICE_INCLUDE_DIR}\"")
  endif( )
endif()

# Set sub-directories.
set(SUBDIRS ${APPLICATIONSDIR} ${ASTRODYNAMICSDIR} ${BASICSDIR} ${INPUTOUTPUTDIR} ${MATHEMATICSDIR} ${EXTERNALDIR})

# Add sub-directories to CMake process.
foreach(CURRENT_SUBDIR ${SUBDIRS})
add_subdirectory("${SRCROOT}${CURRENT_SUBDIR}")
endforeach(CURRENT_SUBDIR)
