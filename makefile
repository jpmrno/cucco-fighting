include makevars

check_defined = $(foreach 1,$1,$(__check_defined))
__check_defined = $(if $(value $1),, \
	$(error Undefined implementation (impl=<file>)$(if $(value 2), ($(strip $2)))))

help:
	@echo "Cucco Fighting!'s makefile"
	@echo "Clean & Build: 'make all impl=<sockets/pipes>'"
	@echo "Build: 'make <option> impl=<sockets/pipes>'"
	@echo "  - Options: 'server', 'client', 'database', 'logger', 'comapi', 'shared'"
	@echo "Clean: 'make clean'"

all: clean build

build: shared comapi server client

shared:
	$(MAKE) -C $@ all

comapi:
	$(call check_defined, impl)
	$(MAKE) -C $@ all impl=$(impl)

server: shared comapi
	$(MAKE) -C $@ all

client: shared comapi
	$(MAKE) -C $@ all

clean:
	$(MAKE) -C shared clean
	$(MAKE) -C comapi clean
	$(MAKE) -C server clean
	$(MAKE) -C client clean

.PHONY: help all build shared server client comapi clean
