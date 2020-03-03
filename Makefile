CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g# $(shell root-config --cflags)
LDFLAGS=-g# $(shell root-config --ldflags)
LDLIBS=#$(shell root-config --libs)

SRCS=main.cpp filter.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: filter

filter: $(OBJS)
	$(CXX) $(LDFLAGS) -o filter $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
