CC = gcc # C compiler
CFLAGS = -std=c99 # flag for C standard
LDFLAGS =  # linker flags
GCOVFLAGS = -fprofile-arcs -ftest-coverage

SRCS = testme.c

#target: dependencies
#	rule to build

# first target is default; will execute if just run "make"
testme: ${SRCS}
	${CC} ${SRCS} ${LDFLAGS} ${GCOVFLAGS} -o testme
	./testme

# @:.o=.c is called an implicit rule
# tells compiler that source files end in .c, targets end in .o
# will build all .o files from .c files

.PHONY: clean
clean:
	rm testme ${OBJS}

# .PHONY indicates that clean is not a file
