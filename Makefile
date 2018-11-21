#Use -fshort-enums to shrink the size of enums to the min possible- nobody
#cares on the PC, but this will hopefully run on a micro someday, and there
#it may well matter.
CC=gcc
CFLAGS=-Wall -Os -fshort-enums
BIN=HALE

#There's got to be a smarter way to do this, but comment if you don't want Python
CFLAGS+= `python3-config --cflags --ldflags --libs`

DEPDIR=.d

# Add .d to Make's recognized suffixes.
SUFFIXES += .d

#We don't need to clean up when we're making these targets
NODEPS:=clean tags svn
#Find all the C++ files in the src/ directory
SOURCES:=$(shell find . -name "*.c")
OBJS =$(SOURCES:%.c=%.o)
#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(patsubst %.c,$(DEPDIR)/%.d,$(SOURCES))


#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	#Chances are, these files don't exist.  GMake will create them and
	#clean up automatically afterwards
	-include $(DEPFILES)
endif


.DEFAULT_GOAL := bin

default: bin

#This is the rule for creating the dependency files
$(DEPDIR)/%.d: %.c $(DEPDIR)
	$(CC) $(CFLAGS) -MM -MT '$(patsubst %.c,%.o,$<)' $< -MF $@

#This rule does the compilation
%.o: %.c $(DEPDIR)/%.d %.h
	#@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

$(DEPDIR):
	mkdir -p $(DEPDIR)

#Default to build the project
bin: $(OBJS)
	$(CC) $(CFLAGS) *.o -o $(BIN)

clean:
	rm -rf $(DEPDIR) *.o $(BIN)

