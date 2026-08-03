# Housekeeping for the LeetCode solutions.
#
# Each solution is a standalone translation unit compiled ad hoc, so the only
# thing worth automating is sweeping up what the compiler leaves behind.
#
#   make clean   remove the build output of every solution in the repository

# Recursive wildcard: $(call rwildcard,<dir>/,<pattern>)
rwildcard = $(foreach d,$(wildcard $(1)*),$(call rwildcard,$(d)/,$(2)) $(filter $(subst *,%,$(2)),$(d)))

SOURCES := $(call rwildcard,,*.cpp)

# Leftovers that give themselves away by their extension, wherever they sit.
DEBRIS := $(foreach p,*.o *.obj *.exe *.out *.ilk *.pdb,$(call rwildcard,,$(p)))

# A binary linked by GCC or Clang on Unix carries no extension, so the only
# thing that marks it as build output is its name: the source's, minus the
# .cpp. Deriving the candidates from the sources also keeps the sweep from
# touching anything that was not compiled here.
BINARIES := $(wildcard $(basename $(SOURCES)))

ARTIFACTS := $(strip $(DEBRIS) $(BINARIES))

ifeq ($(OS),Windows_NT)
# Don't assume a Unix shell: make only finds sh.exe when it is on PATH (Git
# Bash, MSYS), and falls back to running recipes through cmd otherwise. Going
# through `cmd /C del` with quoted, backslashed paths works either way.
DELETE = cmd /C del /Q $(subst /,\,$(patsubst %,"%",$(ARTIFACTS)))
else
DELETE = rm -f $(patsubst %,"%",$(ARTIFACTS))
endif

.PHONY: clean
clean:
ifeq ($(ARTIFACTS),)
	@echo Nothing to clean.
else
	$(DELETE)
endif
