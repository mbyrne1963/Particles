# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-src"
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-build"
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix"
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/mbyrn/CLionProjects/Particles/cmake-build-debug-coverage/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
