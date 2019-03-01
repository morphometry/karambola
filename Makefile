
# CXX = C++-Compiler
CXX      = g++

CXXFLAGS  = -Wall -O2 -DNDEBUG #-ansi
#CXXFLAGS= -Wall -g
BINARY = karambola
LIBRARY = libkarambola.a

VERSION_NUMBER = 2.0

LDFLAGS= -lgsl -lgslcblas
CXXFLAGS += -DVERSION_NUMBER=$(VERSION_NUMBER)

LIBOBJ = \
	lib/Triangulation.o \
	minkowski_functions/calculate_wx00.o \
	minkowski_functions/calculate_w010.o \
	minkowski_functions/calculate_w110.o \
	minkowski_functions/calculate_w210.o \
	minkowski_functions/calculate_w310.o \
	minkowski_functions/calculate_w020.o \
	minkowski_functions/calculate_w120.o \
	minkowski_functions/calculate_w220.o \
	minkowski_functions/calculate_w320.o \
	minkowski_functions/calculate_w102.o \
	minkowski_functions/calculate_w103.o \
	minkowski_functions/calculate_w104.o \
	minkowski_functions/calculate_w202.o \
	minkowski_functions/calculate_sphmink.o \
	minkowski_functions/calculate_wxxx.o \
	lib/check_surface.o \
	print_functions/print_explanations.o \
	print_functions/print_CompWiseScalarMinkValResultType.o \
	print_functions/print_CompWiseVectorMinkValResultType.o \
	print_functions/print_CompWiseMatrixMinkValResultType.o \
	print_functions/write_surface_props_to_file.o \
	print_functions/write_CompWiseTensor3MinkValResultType_to_file.o \
	print_functions/write_CompWiseTensor4MinkValResultType_to_file.o \
	print_functions/write_CompWiseMatrixMinkValResultType_to_file.o \
	print_functions/write_CompWiseVectorMinkValResultType_to_file.o \
	print_functions/write_CompWiseScalarMinkValResultType_to_file.o \
	print_functions/write_CompWiseEigenSystemMinkValResultType_to_file.o \
	print_functions/write_CompWiseSphMinkMinkValResultType_to_file.o \
	lib/MinkValResultType.o \
	lib/calculate_eigensystem.o \
	lib/readpoly.o \
	lib/tensor.o \
	lib/gsleig.o \
	lib/check_command_line.o \
	lib/help.o \
#	minkowski_functions/calculate_voxel_w000.o \
	minkowski_functions/calculate_voxel_w010.o \
	minkowski_functions/calculate_voxel_w020.o \
	minkowski_functions/lookup_tables.o \

TEST_SOURCES = \
    test_suite/test_box.cpp \
    test_suite/test_kugel.cpp \
    test_suite/test_polyreader.cpp \

TEST_OBJECTS = $(TEST_SOURCES:%.cpp=%.o)

HDR = *.h lib/*.h minkowski_functions/*.h print_functions/*.h Makefile

default: $(BINARY)
lib: $(LIBRARY)

all: $(BINARY) test

$(LIBRARY): $(HDR) $(LIBOBJ)
	ar cr $(LIBRARY) $(LIBOBJ)

$(BINARY): $(HDR) lib $(BINARY).o
	$(CXX) -o $(BINARY) $(BINARY).o $(LIBRARY) $(LDFLAGS)

bilimbi: $(HDR) lib bilimbi.o
	$(CXX) -o bilimbi bilimbi.o $(LIBRARY) $(LDFLAGS)

%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -o $*.o -c $<

runtests: $(TEST_OBJECTS) test_suite/runtests.o
	$(CXX) $(CXXFLAGS) -o test_suite/runtests $(TEST_OBJECTS) test_suite/runtests.o $(LIBRARY) $(LDFLAGS)

test: runtests
	(cd test_suite && ./runtests)


clean:
	find . -name \*.o -delete
	rm -f $(BINARY) bilimbi test_suite/runtests $(LIBRARY)

tar:
	git archive --format=tar --prefix=karambola-$(VERSION_NUMBER)/ master | bzip2 >../karambola-$(VERSION_NUMBER).tar.bz2
#	bzcat ../karambola-$(VERSION_NUMBER)-tmp.tar.bz2 | tar --delete karambola-$(VERSION_NUMBER)/pdf | bzip2 >../karambola-$(VERSION_NUMBER).tar.bz2
	bzcat ../karambola-$(VERSION_NUMBER).tar.bz2 | tar --delete karambola-$(VERSION_NUMBER)/demo | bzip2 >../karambola-$(VERSION_NUMBER)-nodemos.tar.bz2
#	rm ../karambola-$(VERSION_NUMBER)-tmp.tar.bz2
