# project directories
# project_dir = $(shell pwd)
include_dir := ./include
src_dir     := ./src
lib_dir     := ./lib
build_dir   := ./build

# targets
targets     := main
targets     := $(foreach t, $(targets), $(t).exe)
target_libs := 
target_dlls := 

# compile and link flags
INCLUDE_FLAGS := $(patsubst %,-I%,$(include_dir))
CPPFLAGS      := $(INCLUDE_FLAGS)
CFLAGS        := -Wall
CXXFLAGS      := -Wall -std=c++11
LDFLAGS       :=
LDLIBS        :=
LOADLIBES     := -L $(lib_dir)

# 
_source_dirs  = $(shell ls -xR ./src|grep :|tr -d :)
_sources_s    = $(foreach subdir, $(_source_dirs), $(wildcard $(subdir)/*.s))
_objects_s    = $(subst $(src_dir), $(build_dir), $(_sources_s:.s=.o))
_sources_c    = $(foreach subdir, $(_source_dirs), $(wildcard $(subdir)/*.c))
_objects_c    = $(subst $(src_dir), $(build_dir), $(_sources_c:.c=.o))
_sources_cpp  = $(foreach subdir, $(_source_dirs), $(wildcard $(subdir)/*.cpp))
_objects_cpp  = $(subst $(src_dir), $(build_dir), $(_sources_cpp:.cpp=.o))

sources      = $(_sources_s) $(_sources_c) $(_objects_cpp)
objects      = $(_objects_s) $(_objects_c) $(_objects_cpp)
dependences  = $(_objects_c:.o=.d) $(_objects_cpp:.o=.d)

# phony targets
.PHONY : all clean run

all: $(targets) $(target_libs) $(target_dlls)
	@echo "Build done!"

$(targets): $(objects)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(target_libs): $(objects)
	$(AR) $(ARFLAGS) $@ $<

$(target_dlls): $(objects)
	$(CC) -shared $^ $(LOADLIBES) $(LDLIBS) -o $@

$(build_dir)/%.o: $(src_dir)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(build_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(build_dir)/%.d: $(src_dir)/%.c
	@mkdir -p $(@D)
	$(CC) -MM $(CPPFLAGS) $< | sed 's,\([^.]*\)\.o[ :]*,$(build_dir)/\1.o $@: ,g' > $@;

$(build_dir)/%.d: $(src_dir)/%.cpp
	@mkdir -p $(@D)
	$(CC) -MM $(CPPFLAGS) $< | sed 's,\([^.]*\)\.o[ :]*,$(build_dir)/\1.o $@: ,g' > $@;

-include $(dependences)

run: $(firstword $(targets))
	@./$<

clean:
	$(RM) $(targets) $(target_libs) $(target_dlls)
	$(RM) -r $(build_dir)

rebuild: clean all
