include makevars

check_defined = $(foreach 1,$1,$(__check_defined))
__check_defined = $(if $(value $1),, \
	$(error Undefined implementation (impl=<file>)$(if $(value 2), ($(strip $2)))))

help:
	@echo "Cucco Fighting!'s makefile"
	@echo "All:"
	@echo "  'make all impl=<sockets/pipes>'"
	@echo "Server/Client:"
	@echo "  'make <option> impl=<sockets/pipes>'"
	@echo "  - Options: 'server', 'client'"
	@echo "Others:"
	@echo "  make <option>'"
	@echo "  - Options: 'database', 'logger', 'libs'"
	@echo "Clean:"
	@echo "  'make clean'"

all: clean build

build: libs server client

libs:
	$(MAKE) -C $@ all

server: libs
	$(call check_defined, impl)
	$(MAKE) -C $@ all impl=$(impl)

client: libs
	$(call check_defined, impl)
	$(MAKE) -C $@ all impl=$(impl)

clean:
	$(MAKE) -C libs clean
	$(MAKE) -C server clean
	$(MAKE) -C client clean

.PHONY: help all build libs server client clean
