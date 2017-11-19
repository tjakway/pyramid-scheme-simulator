
.PHONY: tags
tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ .
