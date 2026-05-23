# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-src")
  file(MAKE_DIRECTORY "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-src")
endif()
file(MAKE_DIRECTORY
  "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-build"
  "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix"
  "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix/tmp"
  "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix/src/pugixml-populate-stamp"
  "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix/src"
  "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix/src/pugixml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix/src/pugixml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/hobbes/projects/cad-solution/dxf/_deps/pugixml-subbuild/pugixml-populate-prefix/src/pugixml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
