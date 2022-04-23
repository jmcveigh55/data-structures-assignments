CC := gcc
SRCDIR := src
SRCSUBDIRS := $(shell find $(SRCDIR) -mindepth 1 -type d) # Directories in SRCDIR
BUILDDIR := build
BUILDSUBDIRS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCSUBDIRS)) # Directories in src/ with root dir. swapped to BUILDDIR
TARGET := check

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)") # SRCEXT files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o)) # SOURCES with ext. and root dir. swapped to .o and BUILDDIR
CFLAGS := -g -Wall -Wno-unused-function
INC := -I include
LIB := -l m

all: $(TARGET)

debug: CFLAGS += -D DEBUG
debug: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p ${BUILDSUBDIRS}
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: debug clean
