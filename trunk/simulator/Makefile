#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 

# Define a lookup dir

vpath %.cpp .:src

# Define the target directories.
TARGETDIR=GNU-i386-Linux
## Target: simulator
CPPFLAGS_simulator = -Ih
OBJS =  $(patsubst src/%.cpp,$(TARGETDIR)/%.o,$(wildcard src/*.cpp)) \
	$(patsubst %.cpp, $(TARGETDIR)/%.o, $(wildcard *.cpp))

SYSLIBS_simulator = -lm
USERLIBS_simulator = $(SYSLIBS_simulator)
DEPLIBS_simulator =
LDLIBS_simulator = $(USERLIBS_simulator)


install: all
	cp $(TARGETDIR)/simulator .

run: install
	./simulator

all: $(TARGETDIR)/simulator


# Link
$(TARGETDIR)/simulator: $(TARGETDIR) $(OBJS) $(DEPLIBS_simulator)
	$(LINK.cc) $(CCFLAGS_simulator) $(CPPFLAGS_simulator) -o $@ $(OBJS) $(LDLIBS_simulator)


# Compile source files into .o files

$(TARGETDIR)/%.o: %.cpp
	$(COMPILE.cc) -MMD $(CCFLAGS_simulator) $(CPPFLAGS_simulator) $< -o $@

include $(wildcard $(TARGETDIR)/*.d)

#### Clean target deletes all generated files ####
clean: clean_obj
	rm -f simulator $(TARGETDIR)/simulator
	$(CCADMIN)
	rm -rf $(TARGETDIR)

clean_obj:
	rm -f $(OBJS)

# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

