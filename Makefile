#CXX=clang++-15
ifdef height
	TOWER_HEIGHT := $(height)
ifdef move
	MOVE_FLAG := -DMOVE_NUM=$(move)
else
endif
else
	TOWER_HEIGHT := 4
endif
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -O3 -DTOWER_HEIGHT=$(TOWER_HEIGHT) $(MOVE_FLAG) -ftemplate-depth=10000
SOURCES = $(wildcard *.cpp)
EXECS = $(SOURCES:%.cpp=%)

all: $(EXECS)

clean:
	rm -fv $(EXECS)
