# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.2.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD\cmake-build-debug"

# Utility rule file for LD.

# Include the progress variables for this target.
include CMakeFiles\LD.dir\progress.make

CMakeFiles\LD:
	call "C:/Program Files/Epic Games/UE_4.25/Engine/Build/BatchFiles/Build.bat" LD Win64 Development "-project=C:/Users/ELE5d5b9c842bce2/Documents/Unreal Projects/LD/LD.uproject" -game -progress -buildscw

LD: CMakeFiles\LD
LD: CMakeFiles\LD.dir\build.make

.PHONY : LD

# Rule to build all files generated by this target.
CMakeFiles\LD.dir\build: LD

.PHONY : CMakeFiles\LD.dir\build

CMakeFiles\LD.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\LD.dir\cmake_clean.cmake
.PHONY : CMakeFiles\LD.dir\clean

CMakeFiles\LD.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD" "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD" "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD\cmake-build-debug" "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD\cmake-build-debug" "C:\Users\ELE5d5b9c842bce2\Documents\Unreal Projects\LD\cmake-build-debug\CMakeFiles\LD.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles\LD.dir\depend

