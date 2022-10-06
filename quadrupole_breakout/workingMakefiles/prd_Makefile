SRC_DIR := source
OBJ_DIR := build
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -L $(HDF5_LIB_PATH) -lhdf5 -lhdf5_cpp -lz -lclipper
CPPFLAGS := -Werror -Wall -Wextra -pedantic -Wno-long-long -Wfatal-errors -Weffc++ -Wshadow -Wmissing-declarations -Wconversion -O3 -march=native
#CPPFLAGS = -ggdb3 -O0

rich.exe: $(OBJ_FILES)
	$(CXX)  $(OBJ_FILES) $(RICH_ROOT)/library_production/librich.a $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $< -I $(RICH_ROOT)
