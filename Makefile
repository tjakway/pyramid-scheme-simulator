
.PHONY: all
all: tags

.PHONY: tags
tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ .

.PHONY: find-shared-ptr-references
find-shared-ptr-references:
	grep -i -I -P -R "std::shared_ptr<[a-zA-Z[:punct:]]+>\s*&" src/ include/ test/
