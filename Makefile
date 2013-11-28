PROGRAM  = mysqlcli
TARGETS  = main cdo_mysqlcli cdo_mysqlstmt
DEP      = .depend
SRCS     = $(TARGETS:%=%.cpp)
OBJS     = $(addsuffix .o, $(basename $(SRCS)))

CXXFLAGS = -L/opt/mysql/server-5.6/lib -L/usr/local/lib -I/opt/mysql/server-5.6/include -I/usr/local/include -Wall -g
CXXLIBS  = -lmysqlclient -levent
CXX      = g++

all: dep $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(CXXLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

dep:
ifeq ($(DEP),$(wildcard $(DEP)))
-include $(DEP)
else
	$(CXX) -MM $(CXXFLAGS) $(SRCS) > $(DEP)
endif

clean:
	rm -f $(PROGRAM) $(OBJS) $(DEP)

