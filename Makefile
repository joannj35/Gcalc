SOURCE := graph.cpp graph_wrapper.cpp gcalc.cpp Exception.cpp main.cpp
OBJECT := $(SOURCE:.cpp=.o)
PY_O := graph.o graph_wrapper.o Exception.o

COMP_FLAG += -std=c++11 -w -DNDEBUG -Wl,-rpath=/usr/local/lib64 -L/usr/local/lib64b -lstdc++

CC=/usr/local/bin/gcc

CXX=/usr/local/bin/g++
 
gcalc: $(OBJECT)
	$(CXX) -g $(COMP_FLAG) -o $@ $(OBJECT)

libgraph.a: $(PY_O)
	ar -rs $@ $^
	
graph_wrapper.o: graph.h graph_wrapper.h
	$(CXX) -c $(COMP_FLAG) $(@:.o=.cpp) -o $@
	
Exception.o: Exception.h
	$(CXX) -c -fPIC $(COMP_FLAG) $(@:.o=.cpp) -o $@

graph.o: graph.h Exception.h
	$(CXX) -c -fPIC $(COMP_FLAG) $(@:.o=.cpp) -o $@
    
gcalc.o: gcalc.h graph.h Exception.h
	$(CXX) -c $(COMP_FLAG) $(@:.o=.cpp) -o $@

main.o: gcalc.h graph.h Exception.h
	$(CXX) -c $(COMP_FLAG) $(@:.o=.cpp) -o $@

clean:
	rm -f gcalc $(OBJECT)