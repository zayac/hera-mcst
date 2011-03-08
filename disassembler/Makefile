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
## Target: disassembler
CPPFLAGS_disassembler = -Ih
OBJS =  $(patsubst src/%.cpp,$(TARGETDIR)/%.o,$(wildcard src/*.cpp)) \
	$(patsubst %.cpp, $(TARGETDIR)/%.o, $(wildcard *.cpp))

SYSLIBS_disassembler = -lm
USERLIBS_disassembler = $(SYSLIBS_simulator)
DEPLIBS_disassembler =
LDLIBS_disassembler = $(USERLIBS_simulator)


install: all
	cp $(TARGETDIR)/disassembler .

run: install
	./disassembler

all: $(TARGETDIR)/disassembler


# Link
$(TARGETDIR)/disassembler: $(TARGETDIR) $(OBJS) $(DEPLIBS_simulator)
	$(LINK.cc) $(CCFLAGS_disassembler) $(CPPFLAGS_simulator) -o $@ $(OBJS) $(LDLIBS_simulator)


# Compile source files into .o files

$(TARGETDIR)/%.o: %.cpp
	$(COMPILE.cc) -MMD $(CCFLAGS_disassembler) $(CPPFLAGS_simulator) $< -o $@

include $(wildcard $(TARGETDIR)/*.d)

#### Clean target deletes all generated files ####
clean: clean_obj
	rm -f disassembler $(TARGETDIR)/simulator
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

