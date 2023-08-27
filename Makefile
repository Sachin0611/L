PROJECTNAME := $(shell basename $(shell pwd))

SOURCEDIR := src
BUILDDIR =
SOURCES := $(shell echo `find . -type f -path "./**/$(PROJECTNAME)/*.cpp"` | cut -c 3-)
OBJECTS = $(foreach SOURCE,$(SOURCES),$(addprefix $(BUILDDIR)/,$(shell basename $(SOURCE:%.cpp=%.o))))
BINARY = $(addprefix $(BUILDDIR)/,$(PROJECTNAME))
HEADERDIR := $(SOURCEDIR)/
BINARYSOURCES := $(shell find . -type f -name "*.cpp" -not -path "./**/$(PROJECTNAME)**/*.cpp")
TESTSSOURCE := $(shell find . -type f -path "./**/$(PROJECTNAME)-tests/*.cpp")
LIBS =

NULLPIPE =

RELEASE ?=false
AGGRESSIVE_OPTIMIZE ?=false
STRICT ?=false
DEFAULT_FLOATING_POINT_TYPE ?=f64
USE_QUADMATH ?=false

CFLAGS = -Wall -Wextra -Werror -std=c++20
CC_NAME =

CFLAGS += -DL_DEFAULT_FLOATING_TYPE=$(DEFAULT_FLOATING_POINT_TYPE)

ifneq ($(shell echo | $(CXX) -dM -E - | grep "__clang__"),)
	CC_NAME =clang
else
	ifneq ($(shell echo | $(CXX) -dM -E - | grep "__GNUC__"),)
		CC_NAME =gnu
	else
		$(error EITHER CLANG OR GCC IS REQUIRED TO COMPILE $(PROJECTNAME))
	endif
endif

ifeq ($(RELEASE),false)
	CFLAGS += -g -O0
	BUILDDIR = build/debug
else
	BUILDDIR += build/release
	CFLAGS += -flto

	ifneq ($(CC_NAME),clang)
		CFLAGS += -s
	endif

	ifeq ($(AGGRESSIVE_OPTIMIZE),true)
		CFLAGS += -Ofast -mtune=native -march=native
		ifeq ($(CC_NAME),gnu)
			CFLAGS += -fmodulo-sched -fmodulo-sched-allow-regmoves \
				      -fgcse-las -fdevirtualize-speculatively -fira-hoist-pressure \
					  -floop-parallelize-all -ftree-parallelize-loops=4
		endif
	else
		CFLAGS += -O3
	endif
endif

ifeq ($(STRICT),true)
	CFLAGS += -DSTRICT
endif

ifeq ($(USE_QUADMATH),true)
	CFLAGS += -DQUADMATH_USED -lquadmath
endif

ifneq ($(OS),Windows_NT)
	UNAME_S := $(shell uname -s)

	NULLPIPE = /dev/null
else
	NULLPIPE = NUL
endif

main: mac_clean help

build: init $(BINARY)

$(BINARY): $(BINARYSOURCES) lib$(PROJECTNAME).a
	$(CXX) $(CFLAGS) $(LIBS) $(BINARYSOURCES) -I$(HEADERDIR) -L$(BUILDDIR) -l$(PROJECTNAME) -o $@

$(OBJECTS): $(SOURCES)
	$(foreach SOURCE,$(SOURCES),`$(CXX) $(CFLAGS) -fPIE -I$(HEADERDIR) -c $(SOURCE) -o $(addprefix $(BUILDDIR)/,$(shell basename $(SOURCE:%.cpp=%.o)))`)

lib$(PROJECTNAME).a: $(OBJECTS)
	ar -crs $(BUILDDIR)/lib$(PROJECTNAME).a $(OBJECTS)

.PHONY: init
init:
	@-mkdir -p $(BUILDDIR) 2> $(NULLPIPE)

.PHONY: clean
clean: mac_clean
	@-rm -fdr build/*

.PHONY: mac_clean
mac_clean:
	@-find . -type f -name ".DS_Store" -delete

# make-tests-out-dir:
# 	-mkdir $(BUILDDIR)/tests

# .PHONY: compile-tests
# compile-tests: build make-tests-out-dir
# 	$(foreach TESTSOURCE,$(TESTSSOURCE),`$(CXX) $(CFLAGS) $(TESTSOURCE) -I$(HEADERDIR) -L$(BUILDDIR) -l$(PROJECTNAME) -o $(addprefix $(BUILDDIR)/tests/,$(shell basename $(TESTSOURCE:%.cpp=%)))`)

# .PHONY: test
# test: compile-tests
# 	./test.sh $(BUILDDIR)/tests

SEPERATOR = "--------------------"

.PHONY: help
help:
	@echo $(SEPERATOR)
	@echo "Available subcommands:"
	@echo "\tbuild: Builds the project"
# @echo -e "\ttest:   Builds and tests the project"
	@echo $(SEPERATOR)
	@echo "Available options:\n"
	@echo "RELEASE=(true | false):\n\t\tIf true, compiles in release mode, with optimizations,"
	@echo "\t\tif false compiles in debug mode, with debug info and minimal optimizations.\n\t\tDefault: false\n"
	@echo "AGGRESSIVE_OPTIMIZE=(true | false):\n\t\tIf true, compiles with \`-Ofast -mtune=native -march=native\`,"
	@echo "\t\t!!!Only effective with RELEASE being true!!!. Default: false\n"
	@echo "STRICT=(true | false):\n\t\tIf true, compiles in strict mode. Default: false\n"
	@echo "DEFAULT_FLOATING_POINT_TYPE=(f32 | f64 | bigf):\n\t\tControlls the default floating point type for libl. Default: f64\n"
	@echo "USE_QUADMATH=(true | false):\n\t\tIf true, uses libquadmath (https://github.com/gcc-mirror/gcc/tree/master/libquadmath) with __float128\n"
	@echo $(SEPERATOR)
