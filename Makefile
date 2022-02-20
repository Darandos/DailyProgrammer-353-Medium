CXX = g++-7
CXXFLAGS = -ggdb3 -std=c++17
TESTINCLUDE = -I lib/googletest/googletest/include/ -I src/
LIBGTESTDIR = lib/googletest/lib
LIBGTEST = $(LIBGTESTDIR)/libgtest_main.a $(LIBGTESTDIR)/libgtest.a
SOURCEDIR = src
SOURCEFILES = $(wildcard $(SOURCEDIR)/*.cc)
OBJECTDIR = build
OBJECTS = $(patsubst $(SOURCEDIR)/%.cc, $(OBJECTDIR)/%.o, $(SOURCEFILES))
TARGETDIR = bin
TESTDIR = test
TESTFILES = $(wildcard $(TESTDIR)/*.cc)
TESTOBJECTS = $(patsubst $(TESTDIR)/%.cc, $(OBJECTDIR)/$(TESTDIR)/%.o, $(TESTFILES))

.PHONY: all
all: $(TARGETDIR)/daily_programmer

$(TARGETDIR)/daily_programmer: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: test
test: $(TARGETDIR)/test

$(OBJECTDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.cc $(SOURCEFILES)
	$(CXX) $(CXXFLAGS) $(TESTINCLUDE) -o $@ -c $<

$(TARGETDIR)/test: $(TESTOBJECTS) $(OBJECTDIR)/pancake_stack.o $(LIBGTEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

.PHONY: clean
clean:
	rm -f $(OBJECTDIR)/*.o
	rm -f $(OBJECTDIR)/$(TESTDIR)/*.o
	rm -f $(TARGETDIR)/*