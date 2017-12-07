#from https://www.systutorials.com/qa/2163/how-to-get-the-full-path-and-directory-of-a-makefile-itself
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PATH))

BIN_DIR=$(MKFILE_DIR)/bin
CMAKE_GENERATED_MAKEFILE=$(BIN_DIR)/Makefile


LIST_RULE_PATH=$(MKFILE_DIR)/mk/list_rule.txt

VERBOSE_VALGRIND_ARGS=--expensive-definedness-checks=yes --track-origins=yes --show-below-main=yes --fair-sched=yes --show-emwarns=yes
#also print memory info at the end
VERY_VERBOSE_VALGRIND_ARGS=--leak-check=full $(VERBOSE_VALGRIND_ARGS)

.PHONY: all
all: tags

.PHONY: tags
tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ .

.PHONY: find-shared-ptr-references
find-shared-ptr-references:
	grep -i -I -P -R "std::shared_ptr<[a-zA-Z[:punct:]]+>\s*&" src/ include/ test/


#TODO
.PHONY: cmake
cmake:


#when I wrote this I didn't realize CMake generates
#a help target that does the same thing with nicer formatting...
#-----------------------------------------------
#append the list rule to the Makefile that cmake generates
#so you can run `make list` to get a list of the generated targets

#note: need to write to a temp file because:
# https://stackoverflow.com/questions/6696842/bash-redirect-input-from-file-back-into-same-file
# see also https://askubuntu.com/questions/752174/reading-and-writing-a-file-tee-command/752451#752451
.PHONY: cmake-append-list-rule
cmake-append-list-rule: cmake
	grep ".PHONY: list" $(CMAKE_GENERATED_MAKEFILE) || ( \
	    TMP=$$(mktemp) &&  \
	    cat $(CMAKE_GENERATED_MAKEFILE) $(LIST_RULE_PATH) \
		> "$$TMP" && \
		mv "$$TMP" $(CMAKE_GENERATED_MAKEFILE) )
