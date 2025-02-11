
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("main")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(cpplib SHARED
    # List C/C++ source files with relative paths to this CMakeLists.txt.
    main.cpp json.cpp)

# --- Begin libcurl Integration ---

# Declare the imported libcurl library (adjust STATIC/SHARED as needed)
add_library(curl STATIC IMPORTED)

# Set the path to the libcurl binary for the current ABI
set_target_properties(curl PROPERTIES IMPORTED_LOCATION
        "${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libcurl.a")

# Include the libcurl header directory (adjust the path as necessary)
target_include_directories(cpplib PRIVATE
        "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/nlohmann")

# Optionally define CURL_STATICLIB for static builds of libcurl
target_compile_definitions(cpplib PRIVATE CURL_STATICLIB)

# --- End libcurl Integration ---

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(cpplib
    # List libraries link to the target library
    android
    log curl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions")
