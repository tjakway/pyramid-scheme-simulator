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
