
OBJS=src/main.o
HDRS=src/outlinertypes.hh
SRCS=src/main.cc
CPPFLAGS=-g
LDFLAGS=-g
LDLIBUS=-lassimp

all:	cave-outliner

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS) $(SRCS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

