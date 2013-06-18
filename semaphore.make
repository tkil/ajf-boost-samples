# -*- mode: makefile-gmake -*-
#

CXXFLAGS += -O3
LIBS += -lboost_chrono-mt -lboost_thread-mt -pthread

EXES := semaphore-boost semaphore-mutex semaphore-pthread

.PHONY : all demo clean

all : $(EXES)

semaphore-boost semaphore-mutex semaphore-pthread : semaphore.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

semaphore-boost : CXXFLAGS += -DUSE_BOOST=1

semaphore-mutex : CXXFLAGS += -DUSE_MUTEX=1

semaphore-pthread : CXXFLAGS += -DUSE_PTHREAD=1

demo :
	./semaphore-boost
	./semaphore-mutex
	./semaphore-pthread

clean :
	$(RM) $(EXES)
