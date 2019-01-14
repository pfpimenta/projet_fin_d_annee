# Partie configurable
TARGET  := projetFA
BUILD   := build
SOURCES := lib lib/libGridMapping src
# INCLUDE := lib

CXXFLAGS += -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

CPPFLAGS = -I/usr/include/irrlicht

LDFLAGS  += -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11
##############################################################################
.SUFFIXES:
.SECONDARY:
.PHONY: clean
# ----------------------------------------------------------------------------
%.o: %.cpp
	@echo '[1m[[32mC++[37m][0m' $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@
# ----------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT  := $(CURDIR)/$(BUILD)/$(TARGET)
export VPATH   := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)
CCFILES        := $(foreach dir,$(SOURCES),$(sort $(notdir $(wildcard $(dir)/*.cpp))))
export LD      := $(CXX)
export OFILES  := $(CCFILES:.cpp=.o)
export OUTPUT  := $(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean...
	@rm -fr $(BUILD) $(OUTPUT)
else
# main targets
$(OUTPUT): $(OFILES)
	@echo '[1m[[35mLD[37m][0m' $(notdir $@)
	@$(LD) $^ -o $@ $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)

-include $(DEPSDIR)/*.d
endif





# SET(UI src/mainwindow.ui)
# SET(MOC src/window.hpp src/render_area.hpp)
# QT4_WRAP_CPP(MOC_GENERATED ${MOC})
# QT4_WRAP_UI(UI_GENERATED ${UI})


# add_executable(
# 	projetFA
# 	${source_files}
#   ${UI_GENERATED}
# 	${MOC_GENERATED}
# )
