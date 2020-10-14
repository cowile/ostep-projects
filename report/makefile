.PHONY: all
all: project-3.pdf

%.pdf: %.roff
	groff -ms -T pdf -P -pletter $< >$@
