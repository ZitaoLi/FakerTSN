CXX     := g++

TEST_DIR = ./samples
TEST_INC = ../include
INCLUDES:= -I$(TEST_INC)

GTEST_HEADERS = ../lib/gtest/include/gtest/*.h \
				../lib/gtest/include/gtest/internal/*.h

CXXFLAGS += -g -Wall -Wextra -pthread $(INCLUDES) -MD

DIR_TEST = ./
DIR_BIN	= ../bin
DIR_BIN_TEST = $(DIR_BIN)/test

TESTS = $(DIR_BIN_TEST)/sample1_unittest
HELLO = $(DIR_BIN_TEST)/hello
TEST_REFLECTOR = $(DIR_BIN_TEST)/test_reflector
TEST_TIMER = $(DIR_BIN_TEST)/test_timer
TEST_CONFIGSETTING = $(DIR_BIN_TEST)/test_configsetting
TEST_QUEUE = $(DIR_BIN_TEST)/test_queue
TEST_TINYXML2 = $(DIR_BIN_TEST)/test_tinyxml2
TEST_PORT_MANAGER = $(DIR_BIN_TEST)/test_port_manager
TEST_REACTOR = $(DIR_BIN_TEST)/test_reactor
TEST_SENDER = $(DIR_BIN_TEST)/test_sender
TEST_RECEIVER = $(DIR_BIN_TEST)/test_receiver
TEST_LIBPCAP = $(DIR_BIN_TEST)/test_libpcap
TEST_NETWORKING = $(DIR_BIN_TEST)/test_networking
TEST_H2_FORWARD = $(DIR_BIN_TEST)/test_h2_forward
TEST_H4_RECEIVER = $(DIR_BIN_TEST)/test_h4_receiver
TEST_MONITOR = $(DIR_BIN_TEST)/test_monitor

GTEST_LIB	= ../lib/gtest/libgtest.a ../lib/gtest/libgtest_main.a ../lib/hello/libhello.a ../lib/tinyxml2/libtinyxml2.a

GTEST_SRCS	= $(wildcard $(TEST_DIR)/*.cc)
GTEST_OBJS	= $(patsubst %.cc,${DIR_BIN}/%.o,$(notdir $(GTEST_SRCS)))
GTEST_DEPS	= $(wildcard $(TEST_DIR)/*.h)

LIBS = -lpcap -lnet -lrt
OBJS = $(filter-out $(DIR_BIN)/main.o, $(wildcard ${DIR_BIN}/*.o))

TEST_SRCS = $(wildcard ./*.cc)
TEST_OBJS = $(patsubst %.cc, ${DIR_BIN_TEST}/%.o, $(notdir $(TEST_SRCS)))

TARGETS := $(TEST) $(HELLO) $(TEST_REFLECTOR) $(TEST_CONFIGSETTING) $(TEST_TIMER) $(TEST_QUEUE) $(TEST_TINYXML2) $(TEST_PORT_MANAGER) $(TEST_REACTOR) $(TEST_SENDER) $(TEST_RECEIVER) $(TEST_LIBPCAP) $(TEST_NETWORKING) $(TEST_H2_FORWARD) $(TEST_H4_RECEIVER) $(TEST_MONITOR)

all : $(TARGETS)

# $(TARGETS) : $(TEST_OBJS)
# 	$(CXX) $(CXXFLAGS) -lpthread $< $(OBJS) -o $@ $(GTEST_LIB) $(LIBS)

# ${DIR_BIN_TEST}/%.o : $(DIR_TEST)/%.cc $(GTEST_HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@



# $(DIR_BIN)/sample1.o : $(TEST_DIR)/sample1.cc $(TEST_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/sample1.cc -o $@ 

# $(DIR_BIN)/sample1_unittest.o : $(TEST_DIR)/sample1_unittest.cc $(TEST_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/sample1_unittest.cc -o $@ 

# $(DIR_BIN)/$(TESTS) : $(DIR_BIN)/sample1.o $(DIR_BIN)/sample1_unittest.o
# 	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB)

# $(TEST_DIR)/sample1.o : $(TEST_DIR)/sample1.cc $(TEST_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/sample1.cc -o $@ 

# $(TEST_DIR)/sample1_unittest.o : $(TEST_DIR)/sample1_unittest.cc $(TEST_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/sample1_unittest.cc -o $@ 

# $(DIR_BIN)/$(TESTS) : $(TEST_DIR)/sample1.o $(TEST_DIR)/sample1_unittest.o
# 	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB)



$(DIR_BIN_TEST)/hello.o : hello.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(HELLO) : $(DIR_BIN_TEST)/hello.o
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) 

# test_reflector
$(DIR_BIN_TEST)/test_reflector.o : test_reflector.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_REFLECTOR) : $(DIR_BIN_TEST)/test_reflector.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_timer
$(DIR_BIN_TEST)/test_timer.o : test_timer.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TIMER) : $(DIR_BIN_TEST)/test_timer.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_configsetting
$(DIR_BIN_TEST)/test_configsetting.o : test_configsetting.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_CONFIGSETTING) : $(DIR_BIN_TEST)/test_configsetting.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_queue
$(DIR_BIN_TEST)/test_queue.o : test_queue.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_QUEUE) : $(DIR_BIN_TEST)/test_queue.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_tinyxml2
$(DIR_BIN_TEST)/test_tinyxml2.o : test_tinyxml2.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TINYXML2) : $(DIR_BIN_TEST)/test_tinyxml2.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_port_manager
$(DIR_BIN_TEST)/test_port_manager.o : test_port_manager.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_PORT_MANAGER) : $(DIR_BIN_TEST)/test_port_manager.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_reactor
$(DIR_BIN_TEST)/test_reactor.o : test_reactor.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_REACTOR) : $(DIR_BIN_TEST)/test_reactor.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_sender
$(DIR_BIN_TEST)/test_sender.o : test_sender.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_SENDER) : $(DIR_BIN_TEST)/test_sender.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_receiver
$(DIR_BIN_TEST)/test_receiver.o : test_receiver.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_RECEIVER) : $(DIR_BIN_TEST)/test_receiver.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_libpcap
$(DIR_BIN_TEST)/test_libpcap.o : test_libpcap.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_LIBPCAP) : $(DIR_BIN_TEST)/test_libpcap.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_networking
$(DIR_BIN_TEST)/test_networking.o : test_networking.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_NETWORKING) : $(DIR_BIN_TEST)/test_networking.o 
	$(CXX) $(CXXFLAGS) -lpthread $^ $(OBJS) -o $@ $(GTEST_LIB) $(LIBS)

# test_h2_forward
$(DIR_BIN_TEST)/test_h2_forward.o : test_h2_forward.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_H2_FORWARD) : $(DIR_BIN_TEST)/test_h2_forward.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_h4_receiver
$(DIR_BIN_TEST)/test_h4_receiver.o : test_h4_receiver.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_H4_RECEIVER) : $(DIR_BIN_TEST)/test_h4_receiver.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

# test_monitor
$(DIR_BIN_TEST)/test_monitor.o : test_monitor.cc $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_MONITOR) : $(DIR_BIN_TEST)/test_monitor.o $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $@ $(GTEST_LIB) $(LIBS)

clean :
	rm -f $(DIR_BIN_TEST)/*.o $(DIR_BIN_TEST)/*.d $(DIR_BIN_TEST)/*.a $(DIR_BIN_TEST)/*.so
	rm -f *.o *.d *.a *.so

-include ${DIR_BIN_TEST}:$(OBJS:.o=.d)