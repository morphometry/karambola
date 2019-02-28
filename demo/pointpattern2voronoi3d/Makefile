CXX     = /usr/bin/g++
#CFLAGS = -march=native -Wall -Werror -ansi -pedantic
CFLAGS = -Wall -Werror -ansi -pedantic
#CFLAGS += -g
CFLAGS += -O2

OBJ    = FileUtil.o CommandLineParser.o CommentParser.o PointPattern2Voronoi3d.o main.o
BIN    = pointpattern2voronoi3d

BINDIR = /usr/local/bin


SRC = $(OBJ:%.o=%.cc)
HDR = $(OBJ:%.o=%.h)
DEPENDFILE = .depend

all:	dep $(BIN)

dep:	$(SRC)
	$(CXX) -MM $(SRC) > $(DEPENDFILE)

-include $(DEPENDFILE)

run:	$(BIN)
	find data/ -maxdepth 1 -iname \*.xyz.bz2 -print0 | nice -n 10 xargs -0 -n 1 -P 4 scripts/pointpattern2voronoi.sh

$(BIN):	$(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $(BIN)

install: $(BIN)
	cp $(BIN) $(BINDIR)

uninstall: $(BIN)
	rm -f $(BINDIR)/$(BIN)

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(DEPENDFILE) *~ 

%.o: %.cc 
	$(CXX) $(CFLAGS) -c $< -o $@

