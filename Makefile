# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fan/robot_sjtu/code/follower

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fan/robot_sjtu/code/follower

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/fan/robot_sjtu/code/follower/CMakeFiles /home/fan/robot_sjtu/code/follower/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/fan/robot_sjtu/code/follower/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named follower

# Build rule for target.
follower: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 follower
.PHONY : follower

# fast build rule for target.
follower/fast:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/build
.PHONY : follower/fast

Particle.o: Particle.cpp.o
.PHONY : Particle.o

# target to build an object file
Particle.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/Particle.cpp.o
.PHONY : Particle.cpp.o

Particle.i: Particle.cpp.i
.PHONY : Particle.i

# target to preprocess a source file
Particle.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/Particle.cpp.i
.PHONY : Particle.cpp.i

Particle.s: Particle.cpp.s
.PHONY : Particle.s

# target to generate assembly for a file
Particle.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/Particle.cpp.s
.PHONY : Particle.cpp.s

ParticleFilter.o: ParticleFilter.cpp.o
.PHONY : ParticleFilter.o

# target to build an object file
ParticleFilter.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/ParticleFilter.cpp.o
.PHONY : ParticleFilter.cpp.o

ParticleFilter.i: ParticleFilter.cpp.i
.PHONY : ParticleFilter.i

# target to preprocess a source file
ParticleFilter.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/ParticleFilter.cpp.i
.PHONY : ParticleFilter.cpp.i

ParticleFilter.s: ParticleFilter.cpp.s
.PHONY : ParticleFilter.s

# target to generate assembly for a file
ParticleFilter.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/ParticleFilter.cpp.s
.PHONY : ParticleFilter.cpp.s

comm.o: comm.cpp.o
.PHONY : comm.o

# target to build an object file
comm.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/comm.cpp.o
.PHONY : comm.cpp.o

comm.i: comm.cpp.i
.PHONY : comm.i

# target to preprocess a source file
comm.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/comm.cpp.i
.PHONY : comm.cpp.i

comm.s: comm.cpp.s
.PHONY : comm.s

# target to generate assembly for a file
comm.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/comm.cpp.s
.PHONY : comm.cpp.s

db_utils.o: db_utils.cpp.o
.PHONY : db_utils.o

# target to build an object file
db_utils.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/db_utils.cpp.o
.PHONY : db_utils.cpp.o

db_utils.i: db_utils.cpp.i
.PHONY : db_utils.i

# target to preprocess a source file
db_utils.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/db_utils.cpp.i
.PHONY : db_utils.cpp.i

db_utils.s: db_utils.cpp.s
.PHONY : db_utils.s

# target to generate assembly for a file
db_utils.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/db_utils.cpp.s
.PHONY : db_utils.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/main.cpp.s
.PHONY : main.cpp.s

motion.o: motion.cpp.o
.PHONY : motion.o

# target to build an object file
motion.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/motion.cpp.o
.PHONY : motion.cpp.o

motion.i: motion.cpp.i
.PHONY : motion.i

# target to preprocess a source file
motion.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/motion.cpp.i
.PHONY : motion.cpp.i

motion.s: motion.cpp.s
.PHONY : motion.s

# target to generate assembly for a file
motion.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/motion.cpp.s
.PHONY : motion.cpp.s

motor.o: motor.cpp.o
.PHONY : motor.o

# target to build an object file
motor.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/motor.cpp.o
.PHONY : motor.cpp.o

motor.i: motor.cpp.i
.PHONY : motor.i

# target to preprocess a source file
motor.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/motor.cpp.i
.PHONY : motor.cpp.i

motor.s: motor.cpp.s
.PHONY : motor.s

# target to generate assembly for a file
motor.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/motor.cpp.s
.PHONY : motor.cpp.s

objectDetection.o: objectDetection.cpp.o
.PHONY : objectDetection.o

# target to build an object file
objectDetection.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/objectDetection.cpp.o
.PHONY : objectDetection.cpp.o

objectDetection.i: objectDetection.cpp.i
.PHONY : objectDetection.i

# target to preprocess a source file
objectDetection.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/objectDetection.cpp.i
.PHONY : objectDetection.cpp.i

objectDetection.s: objectDetection.cpp.s
.PHONY : objectDetection.s

# target to generate assembly for a file
objectDetection.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/objectDetection.cpp.s
.PHONY : objectDetection.cpp.s

obstacle.o: obstacle.cpp.o
.PHONY : obstacle.o

# target to build an object file
obstacle.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/obstacle.cpp.o
.PHONY : obstacle.cpp.o

obstacle.i: obstacle.cpp.i
.PHONY : obstacle.i

# target to preprocess a source file
obstacle.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/obstacle.cpp.i
.PHONY : obstacle.cpp.i

obstacle.s: obstacle.cpp.s
.PHONY : obstacle.s

# target to generate assembly for a file
obstacle.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/obstacle.cpp.s
.PHONY : obstacle.cpp.s

radiotap-parser.o: radiotap-parser.c.o
.PHONY : radiotap-parser.o

# target to build an object file
radiotap-parser.c.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/radiotap-parser.c.o
.PHONY : radiotap-parser.c.o

radiotap-parser.i: radiotap-parser.c.i
.PHONY : radiotap-parser.i

# target to preprocess a source file
radiotap-parser.c.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/radiotap-parser.c.i
.PHONY : radiotap-parser.c.i

radiotap-parser.s: radiotap-parser.c.s
.PHONY : radiotap-parser.s

# target to generate assembly for a file
radiotap-parser.c.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/radiotap-parser.c.s
.PHONY : radiotap-parser.c.s

robotctrl.o: robotctrl.cpp.o
.PHONY : robotctrl.o

# target to build an object file
robotctrl.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/robotctrl.cpp.o
.PHONY : robotctrl.cpp.o

robotctrl.i: robotctrl.cpp.i
.PHONY : robotctrl.i

# target to preprocess a source file
robotctrl.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/robotctrl.cpp.i
.PHONY : robotctrl.cpp.i

robotctrl.s: robotctrl.cpp.s
.PHONY : robotctrl.s

# target to generate assembly for a file
robotctrl.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/robotctrl.cpp.s
.PHONY : robotctrl.cpp.s

rssi.o: rssi.cpp.o
.PHONY : rssi.o

# target to build an object file
rssi.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/rssi.cpp.o
.PHONY : rssi.cpp.o

rssi.i: rssi.cpp.i
.PHONY : rssi.i

# target to preprocess a source file
rssi.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/rssi.cpp.i
.PHONY : rssi.cpp.i

rssi.s: rssi.cpp.s
.PHONY : rssi.s

# target to generate assembly for a file
rssi.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/rssi.cpp.s
.PHONY : rssi.cpp.s

vff.o: vff.cpp.o
.PHONY : vff.o

# target to build an object file
vff.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/vff.cpp.o
.PHONY : vff.cpp.o

vff.i: vff.cpp.i
.PHONY : vff.i

# target to preprocess a source file
vff.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/vff.cpp.i
.PHONY : vff.cpp.i

vff.s: vff.cpp.s
.PHONY : vff.s

# target to generate assembly for a file
vff.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/vff.cpp.s
.PHONY : vff.cpp.s

vision.o: vision.cpp.o
.PHONY : vision.o

# target to build an object file
vision.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/vision.cpp.o
.PHONY : vision.cpp.o

vision.i: vision.cpp.i
.PHONY : vision.i

# target to preprocess a source file
vision.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/vision.cpp.i
.PHONY : vision.cpp.i

vision.s: vision.cpp.s
.PHONY : vision.s

# target to generate assembly for a file
vision.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/vision.cpp.s
.PHONY : vision.cpp.s

wifi_sniffer.o: wifi_sniffer.cpp.o
.PHONY : wifi_sniffer.o

# target to build an object file
wifi_sniffer.cpp.o:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/wifi_sniffer.cpp.o
.PHONY : wifi_sniffer.cpp.o

wifi_sniffer.i: wifi_sniffer.cpp.i
.PHONY : wifi_sniffer.i

# target to preprocess a source file
wifi_sniffer.cpp.i:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/wifi_sniffer.cpp.i
.PHONY : wifi_sniffer.cpp.i

wifi_sniffer.s: wifi_sniffer.cpp.s
.PHONY : wifi_sniffer.s

# target to generate assembly for a file
wifi_sniffer.cpp.s:
	$(MAKE) -f CMakeFiles/follower.dir/build.make CMakeFiles/follower.dir/wifi_sniffer.cpp.s
.PHONY : wifi_sniffer.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... follower"
	@echo "... rebuild_cache"
	@echo "... Particle.o"
	@echo "... Particle.i"
	@echo "... Particle.s"
	@echo "... ParticleFilter.o"
	@echo "... ParticleFilter.i"
	@echo "... ParticleFilter.s"
	@echo "... comm.o"
	@echo "... comm.i"
	@echo "... comm.s"
	@echo "... db_utils.o"
	@echo "... db_utils.i"
	@echo "... db_utils.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... motion.o"
	@echo "... motion.i"
	@echo "... motion.s"
	@echo "... motor.o"
	@echo "... motor.i"
	@echo "... motor.s"
	@echo "... objectDetection.o"
	@echo "... objectDetection.i"
	@echo "... objectDetection.s"
	@echo "... obstacle.o"
	@echo "... obstacle.i"
	@echo "... obstacle.s"
	@echo "... radiotap-parser.o"
	@echo "... radiotap-parser.i"
	@echo "... radiotap-parser.s"
	@echo "... robotctrl.o"
	@echo "... robotctrl.i"
	@echo "... robotctrl.s"
	@echo "... rssi.o"
	@echo "... rssi.i"
	@echo "... rssi.s"
	@echo "... vff.o"
	@echo "... vff.i"
	@echo "... vff.s"
	@echo "... vision.o"
	@echo "... vision.i"
	@echo "... vision.s"
	@echo "... wifi_sniffer.o"
	@echo "... wifi_sniffer.i"
	@echo "... wifi_sniffer.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

