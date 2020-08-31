#############################################################################
# Makefile for: Operating Systems: Project 2			            #
#############################################################################


SUBDIRS = root splitNmerge leaves

.PHONY: allmake
allmake:
	$(MAKE) -C root
	$(MAKE) -C splitNmerge
	$(MAKE) -C leaves

.PHONY: clean
clean:
	$(MAKE) -C root clean
	$(MAKE) -C splitNmerge clean
	$(MAKE) -C leaves clean
