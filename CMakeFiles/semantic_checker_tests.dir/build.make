# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pbixby/326/project-parkerbixby

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pbixby/326/project-parkerbixby

# Include any dependencies generated for this target.
include CMakeFiles/semantic_checker_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/semantic_checker_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/semantic_checker_tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/semantic_checker_tests.dir/flags.make

CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o: tests/semantic_checker_tests.cpp
CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o -c /home/pbixby/326/project-parkerbixby/tests/semantic_checker_tests.cpp

CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/tests/semantic_checker_tests.cpp > CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.i

CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/tests/semantic_checker_tests.cpp -o CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.s

CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o: src/token.cpp
CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o -c /home/pbixby/326/project-parkerbixby/src/token.cpp

CMakeFiles/semantic_checker_tests.dir/src/token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/src/token.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/src/token.cpp > CMakeFiles/semantic_checker_tests.dir/src/token.cpp.i

CMakeFiles/semantic_checker_tests.dir/src/token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/src/token.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/src/token.cpp -o CMakeFiles/semantic_checker_tests.dir/src/token.cpp.s

CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o: src/mypl_exception.cpp
CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o -c /home/pbixby/326/project-parkerbixby/src/mypl_exception.cpp

CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/src/mypl_exception.cpp > CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.i

CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/src/mypl_exception.cpp -o CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.s

CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o: src/lexer.cpp
CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o -c /home/pbixby/326/project-parkerbixby/src/lexer.cpp

CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/src/lexer.cpp > CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.i

CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/src/lexer.cpp -o CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.s

CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o: src/ast_parser.cpp
CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o -c /home/pbixby/326/project-parkerbixby/src/ast_parser.cpp

CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/src/ast_parser.cpp > CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.i

CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/src/ast_parser.cpp -o CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.s

CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o: src/symbol_table.cpp
CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o -c /home/pbixby/326/project-parkerbixby/src/symbol_table.cpp

CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/src/symbol_table.cpp > CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.i

CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/src/symbol_table.cpp -o CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.s

CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o: CMakeFiles/semantic_checker_tests.dir/flags.make
CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o: src/semantic_checker.cpp
CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o: CMakeFiles/semantic_checker_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o -MF CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o.d -o CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o -c /home/pbixby/326/project-parkerbixby/src/semantic_checker.cpp

CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pbixby/326/project-parkerbixby/src/semantic_checker.cpp > CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.i

CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pbixby/326/project-parkerbixby/src/semantic_checker.cpp -o CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.s

# Object files for target semantic_checker_tests
semantic_checker_tests_OBJECTS = \
"CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o" \
"CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o" \
"CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o" \
"CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o" \
"CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o" \
"CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o" \
"CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o"

# External object files for target semantic_checker_tests
semantic_checker_tests_EXTERNAL_OBJECTS =

semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/tests/semantic_checker_tests.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/src/token.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/src/mypl_exception.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/src/lexer.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/src/ast_parser.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/src/symbol_table.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/src/semantic_checker.cpp.o
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/build.make
semantic_checker_tests: /usr/lib/x86_64-linux-gnu/libgtest.a
semantic_checker_tests: CMakeFiles/semantic_checker_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pbixby/326/project-parkerbixby/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable semantic_checker_tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/semantic_checker_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/semantic_checker_tests.dir/build: semantic_checker_tests
.PHONY : CMakeFiles/semantic_checker_tests.dir/build

CMakeFiles/semantic_checker_tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/semantic_checker_tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/semantic_checker_tests.dir/clean

CMakeFiles/semantic_checker_tests.dir/depend:
	cd /home/pbixby/326/project-parkerbixby && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pbixby/326/project-parkerbixby /home/pbixby/326/project-parkerbixby /home/pbixby/326/project-parkerbixby /home/pbixby/326/project-parkerbixby /home/pbixby/326/project-parkerbixby/CMakeFiles/semantic_checker_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/semantic_checker_tests.dir/depend
