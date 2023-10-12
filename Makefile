CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -Werror -ggdb -funroll-loops -DTERM=$(TERM)
CXXFLAGS = -Wall -Werror -ggdb -funroll-loops -DTERM=$(TERM)

LDFLAGS = -lm -lncurses -lstdc++

BIN = FlappyBird
OBJS = FlappyBird.o

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CC) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.* gmon.out

#run program in debug mode
val:
	valgrind --leak-check=full ./$(BIN)
