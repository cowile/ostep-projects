REPORTS=project-3.pdf project-4.pdf

.PHONY: all
all: ${REPORTS}

%.pdf: %.roff
	groff -ms -T pdf -P -pletter $< >$@
