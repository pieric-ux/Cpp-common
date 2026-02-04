
OS = $(shell uname)

# Program name
NAME = libcommon.a

# Source and object directories
SRCDIR = srcs
COREDIR = core
IODIR = io
NETDIR = net
SOCKETDIR = sockets
UTIlSDIR = utils
RAIIDIR = raii
LOADERDIR = loader
OBJDIR = objs

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wshadow -MMD -MP -std=c++98
DEBUG_FLAGS = -g3 -fno-omit-frame-pointer -fstack-protector-all

INCLUDES = -I includes

# vpath to specify where to find the .cpp files
vpath %.cpp \
	$(SRCDIR) \
	$(SRCDIR)/$(COREDIR) \
	$(SRCDIR)/$(COREDIR)/$(IODIR) \
	$(SRCDIR)/$(COREDIR)/$(NETDIR)/$(SOCKETDIR) \
	$(SRCDIR)/$(COREDIR)/$(RAIIDIR) \
	$(SRCDIR)/$(COREDIR)/$(UTIlSDIR) \
	$(SRCDIR)/$(LOADERDIR)

# Sources and object files
SRCES = EventFactoryIO.cpp SelectEventIO.cpp PollEventIO.cpp \
		ASocket.cpp ATcpSocket.cpp Addrinfo.cpp TcpClient.cpp TcpServer.cpp \
		SharedPtr.cpp \
		Directory.cpp fileUtils.cpp stringUtils.cpp timeUtils.cpp \
		Loader.cpp

OBJS_SRCES = $(addprefix $(OBJDIR)/, $(SRCES:.cpp=.o))

# Default rule: make all and compile the program
all: $(NAME)

debug: CXXFLAGS = $(DEBUG_FLAGS)

# Rebuild with debug flags
debug: re

# Sanitize
sanitize: DEBUG_FLAGS += -fsanitize=address 
sanitize: debug 

# Rule to compile with Leaks check
leaks:
ifeq ($(OS), Darwin)
	MallocStackLogging=YES leaks --outputGraph=common.memgraph --fullContent --fullStackHistory --atExit -- ./$(NAME)
else ifeq ($(OS), Linux)
	valgrind --leak-check=full --track-origins=yes --log-file=valgrind.log --show-leak-kinds=all --trace-children=yes --track-fds=all ./$(NAME)
endif

# Compile each .cpp file to .o
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile the final executable
$(NAME): $(OBJS_SRCES)
	ar -rcs $(NAME) $(OBJS_SRCES)

# Compile testPoll.cpp
$(TEST_POLL_OBJ): $(TEST_POLL_SRC)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to clean up object files
clean:
	@rm -rf $(OBJDIR)
	@rm -f $(TEST_POLL)

# Rule to clean up object files and executable
fclean: clean
	@rm -f $(NAME)

# Rule to recompile everything
re: fclean all

# Rule to generate documentation with Doxygen
doc:
	@echo "Generating documentation with Doxygen..."
	@doxygen doc/Doxyfile
	@echo "Documentation generated in doc/html/"

# Rule to open documentation in browser
opendoc:
	@open doc/html/index.html

# Rule to clean documentation
cleandoc:
	@echo "Cleaning documentation..."
	@rm -rf doc/html doc/latex doc/html/Log42.tag
	@echo "Documentation cleaned."

.PHONY: all clean fclean re bonus debug sanitize doc opendoc cleandoc
