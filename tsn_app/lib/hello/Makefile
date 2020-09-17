AR = ar
ARFLAGS = cr
LIB = libhello.a
RM = -rm -rf
SRCS := $(wildcard *.cc)
OBJS := $(patsubst %.cc,./%.o,$(notdir $(SRCS)))

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)


clean:
	$(RM) $(OBJS) *.bak *~

cleanall:clean
	$(RM) $(LIB)

.PHONY: all clean