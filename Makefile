# Housekeeping for the LeetCode solutions.
#
# Each solution is a standalone translation unit compiled ad hoc, so the only
# thing worth automating is sweeping up what the compiler leaves behind.
#
#   make clean   remove every .obj and .exe in the repository

# Recursive wildcard: $(call rwildcard,<dir>/,<pattern>)
rwildcard = $(foreach d,$(wildcard $(1)*),$(call rwildcard,$(d)/,$(2)) $(filter $(subst *,%,$(2)),$(d)))

ARTIFACTS := $(strip $(call rwildcard,,*.obj) $(call rwildcard,,*.exe))

ifeq ($(OS),Windows_NT)
# Don't assume a Unix shell: make only finds sh.exe when it is on PATH (Git
# Bash, MSYS), and falls back to running recipes through cmd otherwise. Going
# through `cmd /C del` with quoted, backslashed paths works either way.
DELETE = cmd /C del /Q $(subst /,\,$(patsubst %,"%",$(ARTIFACTS)))
else
DELETE = rm -f $(ARTIFACTS)
endif

.PHONY: clean
clean:
ifeq ($(ARTIFACTS),)
	@echo Nothing to clean.
else
	$(DELETE)
endif
