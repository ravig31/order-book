# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ravindugamage/main/order-book

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ravindugamage/main/order-book/build

# Include any dependencies generated for this target.
include CMakeFiles/orderbook.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/orderbook.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/orderbook.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/orderbook.dir/flags.make

CMakeFiles/orderbook.dir/codegen:
.PHONY : CMakeFiles/orderbook.dir/codegen

CMakeFiles/orderbook.dir/src/main.cpp.o: CMakeFiles/orderbook.dir/flags.make
CMakeFiles/orderbook.dir/src/main.cpp.o: /Users/ravindugamage/main/order-book/src/main.cpp
CMakeFiles/orderbook.dir/src/main.cpp.o: CMakeFiles/orderbook.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ravindugamage/main/order-book/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/orderbook.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/orderbook.dir/src/main.cpp.o -MF CMakeFiles/orderbook.dir/src/main.cpp.o.d -o CMakeFiles/orderbook.dir/src/main.cpp.o -c /Users/ravindugamage/main/order-book/src/main.cpp

CMakeFiles/orderbook.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/orderbook.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ravindugamage/main/order-book/src/main.cpp > CMakeFiles/orderbook.dir/src/main.cpp.i

CMakeFiles/orderbook.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/orderbook.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ravindugamage/main/order-book/src/main.cpp -o CMakeFiles/orderbook.dir/src/main.cpp.s

CMakeFiles/orderbook.dir/src/OrderBook.cpp.o: CMakeFiles/orderbook.dir/flags.make
CMakeFiles/orderbook.dir/src/OrderBook.cpp.o: /Users/ravindugamage/main/order-book/src/OrderBook.cpp
CMakeFiles/orderbook.dir/src/OrderBook.cpp.o: CMakeFiles/orderbook.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ravindugamage/main/order-book/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/orderbook.dir/src/OrderBook.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/orderbook.dir/src/OrderBook.cpp.o -MF CMakeFiles/orderbook.dir/src/OrderBook.cpp.o.d -o CMakeFiles/orderbook.dir/src/OrderBook.cpp.o -c /Users/ravindugamage/main/order-book/src/OrderBook.cpp

CMakeFiles/orderbook.dir/src/OrderBook.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/orderbook.dir/src/OrderBook.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ravindugamage/main/order-book/src/OrderBook.cpp > CMakeFiles/orderbook.dir/src/OrderBook.cpp.i

CMakeFiles/orderbook.dir/src/OrderBook.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/orderbook.dir/src/OrderBook.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ravindugamage/main/order-book/src/OrderBook.cpp -o CMakeFiles/orderbook.dir/src/OrderBook.cpp.s

# Object files for target orderbook
orderbook_OBJECTS = \
"CMakeFiles/orderbook.dir/src/main.cpp.o" \
"CMakeFiles/orderbook.dir/src/OrderBook.cpp.o"

# External object files for target orderbook
orderbook_EXTERNAL_OBJECTS =

orderbook: CMakeFiles/orderbook.dir/src/main.cpp.o
orderbook: CMakeFiles/orderbook.dir/src/OrderBook.cpp.o
orderbook: CMakeFiles/orderbook.dir/build.make
orderbook: /opt/homebrew/lib/libfmt.11.0.2.dylib
orderbook: CMakeFiles/orderbook.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/ravindugamage/main/order-book/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable orderbook"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/orderbook.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/orderbook.dir/build: orderbook
.PHONY : CMakeFiles/orderbook.dir/build

CMakeFiles/orderbook.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/orderbook.dir/cmake_clean.cmake
.PHONY : CMakeFiles/orderbook.dir/clean

CMakeFiles/orderbook.dir/depend:
	cd /Users/ravindugamage/main/order-book/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ravindugamage/main/order-book /Users/ravindugamage/main/order-book /Users/ravindugamage/main/order-book/build /Users/ravindugamage/main/order-book/build /Users/ravindugamage/main/order-book/build/CMakeFiles/orderbook.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/orderbook.dir/depend

