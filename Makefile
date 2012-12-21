CXX 		 = clang++
CXXFLAGS = -O2 -Wall -Wextra -std=c++0x
LDFLAGS  = -L/usr/local/lib -L/usr/local/opt/boost
INCLUDES = -Iinclude -I/usr/local/include -I/usr/local/include/boost -I/usr/local/opt/boost/include
LIBS 		 = -lboost_program_options-mt -lboost_system-mt
SERVER   = server
CLIENT   = client
TARGETS  = $(SERVER) $(CLIENT)
SRC_DIR  = src
SERVER_OBJ = $(SRC_DIR)/server.o
CLIENT_OBJ = $(SRC_DIR)/client.o

OBJS		 = $(SERVER) $(CLIENT)

LIB_BOOST_TEST:=-lboost_unit_test_framework-mt

all: $(TARGETS)

# test: test/test.cpp src/server.hpp
# 	$(CXX) $(CXXFLAGS) $(LIB_BOOST_TEST) test.cpp -o $@
# 	./test

$(SERVER): $(SERVER_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(SERVER_OBJ) $(LIBS)

$(CLIENT): $(CLIENT_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(CLIENT_OBJ) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(TARGETS) $(OBJS)
