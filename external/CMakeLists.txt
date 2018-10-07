find_package(Miniupnpc REQUIRED)

add_subdirectory(miniupnp/miniupnpc)
set_property(TARGET libminiupnpc-static PROPERTY FOLDER "external")
if(MSVC)
  set_property(TARGET libminiupnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -wd4244 -wd4267")
else()
  set_property(TARGET libminiupnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-undef -Wno-unused-result -Wno-unused-value")
endif()
set(UPNP_LIBRARIES "libminiupnpc-static" PARENT_SCOPE)

add_subdirectory(sqlite3)
if (MSVC)
  set(STXXL_STD_RANDOM OFF) # msvc bug can't use <random>
else()
  set(USE_GNU_PARALLEL OFF CACHE BOOL "" FORCE) # no GNU parallel
  set(USE_STD_THREADS ON CACHE BOOL "" FORCE) # can't use pthread because of CMake bug
endif()

add_subdirectory(stxxl)
if (MSVC)
  if (STXXL_STD_RANDOM)
    message (FATAL_ERROR "Sanity check failed: Should not use std random on MSVC")
  endif()
endif()
