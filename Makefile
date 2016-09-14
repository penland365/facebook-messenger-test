CC := clang++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
BINNAME := fbmt
TARGET := $(BINDIR)/$(BINNAME)
OPENSSLDIR := /usr/local/Cellar/openssl/1.0.2h_1
INSTALLDIR := $(HOME)/bin


SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -Werror -std=c++14 # -Wall
#INC := -I include /usr/local/Cellar/boost/1.61.0_1/include
INC := -I /usr/local/Cellar/boost/1.61.0_1/include -I include -I $(OPENSSLDIR)/include
LIBS := -L lib -lboost_program_options-mt -lboost_system-mt -L $(OPENSSLDIR)/lib -lssl -lcrypto

$(TARGET): $(OBJECTS)
	@echo "Building . . ."
	@echo " $(CC) $^ -o $(TARGET) $(LIBS)"; $(CC) $^ -o $(TARGET) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning . . .";
	@echo "$(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)

.PHONY: CLEAN

.PHONY: install
install: 
	@cp -fi $(TARGET) $(INSTALLDIR) 
	@echo "installed $(BINNAME) to $(HOME)/bin"

.PHONY: uninstall
uninstall:
	@rm $(INSTALLDIR)/$(BINNAME)
	@echo "$(BINNAME) removed"
