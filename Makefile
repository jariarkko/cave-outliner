
OBJS=src/main.o src/svg.o
HDRS=src/outlinertypes.hh src/svg.hh
SRCS=src/main.cc src/svg.cc
SUPP=Makefile
CPPFLAGS=-g -Wall
LDFLAGS=-O4
LDLIBS=-lassimp

all:	cave-outliner

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS) $(SRCS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
