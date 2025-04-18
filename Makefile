CXX = g++

CXXFLAGS = -Wall -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
		-Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body \
		-Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor \
		-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
		-Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default \
		-Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-range \
		-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs \
		-Wstack-protector -Wsuggest-override -Wbounds-attributes-redundant -Wlong-long -Wopenmp \
		-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer \
		-Wlarger-than=8192 -Wstack-protector -fPIE -Werror=vla -fsanitize=address

SOURCES = src/binary_tree_str.cpp src/akinator.cpp src/main_str.cpp src/stack_lib.cpp src/tree_loader.cpp

INCLUDES = -I./include

OBJECTS = $(SOURCES:%.cpp=build/%.o)

TARGET   := do

.PHONY: all clean

all: $(TARGET)
	@printf "Compilation done\n"

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@


build/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@


clean:
	@rm -rf $(TARGET) build
