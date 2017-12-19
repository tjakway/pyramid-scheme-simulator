#from https://www.systutorials.com/qa/2163/how-to-get-the-full-path-and-directory-of-a-makefile-itself
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PATH))

BIN_DIR=$(MKFILE_DIR)/bin
CMAKE_GENERATED_MAKEFILE=$(BIN_DIR)/Makefile


LIST_RULE_PATH=$(MKFILE_DIR)/mk/list_rule.txt

CLANG_CHECK_ARGS=-extra-arg=-Wno-undef -extra-arg=-Wno-unused-function

VERBOSE_VALGRIND_ARGS=--expensive-definedness-checks=yes --track-origins=yes --show-below-main=yes --fair-sched=yes --show-emwarns=yes
#also print memory info at the end
VERY_VERBOSE_VALGRIND_ARGS=--leak-check=full $(VERBOSE_VALGRIND_ARGS)

GREP_OPTIONS=-i -I -P -R
SRC_FOLDERS=src/ include/ test/

#to match a C++ identifier (variable, function name, etc.)
IDENTIFIER_PATTERN=[a-zA-Z[:punct:]]+

.PHONY: all
all: tags

.PHONY: tags
tags:
	ctags -R --exclude=bin --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ .

.PHONY: find-shared-ptr-references
find-shared-ptr-references:
	! grep $(GREP_OPTIONS) 'std::shared_ptr<$(IDENTIFIER_PATTERN)>\s*&' $(SRC_FOLDERS)

#match things like `std::vector<Foo> bar(baz);`
#you'll have to manually figure out which lines are actually function calls
.PHONY: find-vector-fill-ctor-uses
find-vector-fill-ctor-uses:
	! grep $(GREP_OPTIONS)  \
	    'std::vector<$(IDENTIFIER_PATTERN)>\s+$(IDENTIFIER_PATTERN)\($(IDENTIFIER_PATTERN)\)\s*;' \
	    $(SRC_FOLDERS)


#TODO
.PHONY: cmake
cmake:
	true #pass

compile_commands.json: cmake
	test -f compile_commands.json || ln -s $(BIN_DIR)/compile_commands.json $(MKFILE_DIR)/

#run clang-check on the source folders
#(obviously clang-check has to be on your PATH)
.PHONY: clang-check
clang-check: compile_commands.json
	find src/ include/ test -regextype egrep -regex ".*\.(cpp|h|hpp|cxx|c)" -type f \
	    -exec clang-check $(CLANG_CHECK_ARGS) {} +
