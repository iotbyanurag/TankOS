
# The following flags and variables can be defined to affect the behaviour of this Makefile.
# (flag = variable that is just checked for existence, value irrelevant. Defined like this on the command-line: 'VERBOSE=')
# VERBOSE: (flag) Causes make to print all executed shell-commands
# PLATFORM: String identifying target build-platform. A platform-specific Makefile called Build$(PLATFORM).mk is required.
#			Defaults to 'Avr'.
#			Call 'make platforms' for a list of available platforms
# PROJ: String identifying a project. Shortcut-commands will be generated to access commands for that project (like link, hex, lss, flash, etc.)
#			'make clean' will still clean all available projects.
#			Call 'make projects' for a list of available projects
# MAIN: Filename (without the .main.c ending) of a main-file in an executable project. This main-file will be the one linked into the executable.
#			The Object.mk files in executable projects have defauls for this.
# LSS: (flag) Causes the creation of .lss and .eep files when linking for platform AVR. Excluded by default.
# DEBUG: (flag) Instructs platform-dependent Makefiles to create debug-symbols in archives and objects. Also switches to a separate build-directory.
# SPEED: (flag) Instructs platform-dependent Makefiles to optimize for Speed instead of Size. Also switches to yet another build-directory.
# NOOPT: (flag) Instructs platform-dependent Makefiles to do no optimizations. Also switches to yet another build-directory. This flag takes precedence over the SPEED flag.
# STUDIO: (flag) If set, the output-artifacts will always be copied to the Atmel Studio output location, to be used in the Atmel Studio simulator. (-> The target 'studio' will be called implicitely).
# AUTO_DISCOVER: (flag) Causes the makefile to automatically find project-makefiles (Project.mk) and projects. The project might not be in the correct order of their dependencies.
# DONT_LINK_ALL: (flag) Causes only the main output to be produced, not every possible output. For executable objects, this means the linker is invoked only once.

# Possibility to define global parameters here
-include make_parameters

# Make sure this is at least defined
MAKECMDGOALS ?=

ifeq ($(origin VERBOSE), undefined)
.SILENT:
endif

ifeq ($(origin PLATFORM), undefined)
PLATFORM := Avr
endif

FIND := find

ifeq ($(origin AUTO_DISCOVER), undefined)
AllProjects := Unity AntonAvrLib AntonAvrLib-Test Tank-Shared Kernel-Tank-MASTER Kernel-Tank-IO Kernel-Simulator Main-Tank-MASTER Main-Tank-IO Main-Simulator Test-Scheduler
ProjectMakefiles := $(foreach p, $(AllProjects), $p/Project.mk)
else
ProjectMakefiles := $(shell $(FIND) . -maxdepth 2 -name Project.mk)
AllProjects := $(foreach p, $(ProjectMakefiles), $(shell basename $(shell dirname $p)))
endif

ALL_PLATFORMS := $(shell $(FIND) . -maxdepth 1 -name Build\*.mk | \
						sed -re 's|./Build(.*).mk|\1|g')

# Include shortcut-commands for the defined project.
# This is above the all-command, to make the build-command the default if PROJ is defined.
ifneq ($(origin PROJ), undefined)
build: $(PROJ)
hex: hex_$(PROJ)
eep: eep_$(PROJ)
lss: lss_$(PROJ)
link: link_$(PROJ)
size: size_$(PROJ)
lib: lib_$(PROJ)
map: map_$(PROJ)
flash: flash_$(PROJ)
flashv: flashv_$(PROJ)
studio: studio_$(PROJ)
clean_target: clean_target_$(PROJ)
relink: relink_$(PROJ)
run: run_$(PROJ)
endif

all: $(AllProjects)

clean: $(foreach p, $(AllProjects), clean_$p)

projects:
	@echo Available projects: $(AllProjects)

platforms:
	@echo Available platforms: $(ALL_PLATFORMS)

# Fake-targets should be always up-to-date to not cause the actual targets to be constantly rebuilt. See Main.mk.
.fake_targets/%:
	mkdir -p $(@D); touch $@

# Check, if the platform-dependent Makefile is present.
$(if $(wildcard Build$(PLATFORM).mk),,$(error Illegal platform $(PLATFORM). Available platforms: $(ALL_PLATFORMS)))

# From http://blog.jgc.org/2007/01/what-makefile-am-i-in.html
# Function to query the current makefile. Example: current-makefile = $(call which-makefile)
which-makefile = $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
get-basedir = $(shell basename $(shell dirname $(call which-makefile)))

# TODO include the projects ordered by dependencies.

ifneq ($(MAKECMDGOALS), projects)
ifneq ($(MAKECMDGOALS), platforms)
include $(ProjectMakefiles)
endif
endif

# Each project has two project-specific makefiles: Project.mk and (optionally) Objects.mk

# Project.mk:
# Defines name of the project, type of the project (library or executable), dependencies and symbols for the preprocessor.
# See an existing example

# Objects.mk:
# Projects can define objects (possibly from other projects), that should be included when linking or archiving.
# Following reasons:
#  - Kernel-projects should define the kernel-objects they require from other libraries. By default, objects named *.kernel.o are excluded.
#  - Main-projects can define additional objects to be linked, that should override symbols defined in libraries
# Take care to APPEND to the 'objects'-variable, as it already contains other objects!
