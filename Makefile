#CXX=clang++-15
ifdef height
	TOWER_HEIGHT := $(height)
ifdef move
	MOVE_NUM := $(move)
else
	MOVE_NUM := 0
endif
else
	TOWER_HEIGHT := 4
	MOVE_NUM := 0
endif
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -O3 -DTOWER_HEIGHT=$(TOWER_HEIGHT) -DMOVE_NUM=$(MOVE_NUM) -ftemplate-depth=10000
SOURCES = $(wildcard *.cpp)
EXECS = $(SOURCES:%.cpp=%)

all: $(EXECS)

clean:
	rm -fv $(EXECS)
