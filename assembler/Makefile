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
CPPFLAGS_assembler = -Ih
OBJS =  $(patsubst src/%.cpp,$(TARGETDIR)/%.o,$(wildcard src/*.cpp)) \
	$(patsubst %.cpp, $(TARGETDIR)/%.o, $(wildcard *.cpp))

SYSLIBS_assembler = -lm
USERLIBS_assembler = $(SYSLIBS_assembler)
DEPLIBS_assembler =
LDLIBS_assembler = $(USERLIBS_assembler)


install: all
	cp $(TARGETDIR)/assembler .

run: install
	./assembler

all: $(TARGETDIR)/assembler


# Link
$(TARGETDIR)/assembler: $(TARGETDIR) $(OBJS) $(DEPLIBS_assembler)
	$(LINK.cc) $(CCFLAGS_assembler) $(CPPFLAGS_assembler) -o $@ $(OBJS) $(LDLIBS_assembler)


# Compile source files into .o files

$(TARGETDIR)/%.o: %.cpp
	$(COMPILE.cc) -MMD $(CCFLAGS_assembler) $(CPPFLAGS_assembler) $< -o $@

include $(wildcard $(TARGETDIR)/*.d)

#### Clean target deletes all generated files ####
clean: clean_obj
	rm -f assembler $(TARGETDIR)/assembler
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

