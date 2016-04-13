include makevars

help:
	@echo "Cucco Fighting!'s makefile"
	@echo "Usage: 'make [ all | build | clean | shared | server | client | help ]'"

all: clean build

build: shared server client

shared:
	$(MAKE) -C $@ all

server: shared
	$(MAKE) -C $@ all

client: shared
	$(MAKE) -C $@ all

clean:
	$(MAKE) -C shared clean
	$(MAKE) -C server clean
	$(MAKE) -C client clean

.PHONY: help all build shared server client clean
