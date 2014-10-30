# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug
endif
export config

PROJECTS := BulletLua Test

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

BulletLua: 
	@echo "==== Building BulletLua ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f BulletLua.make

Test: 
	@echo "==== Building Test ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f Test.make

clean:
	@${MAKE} --no-print-directory -C . -f BulletLua.make clean
	@${MAKE} --no-print-directory -C . -f Test.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   release"
	@echo "   debug32"
	@echo "   release32"
	@echo "   debug64"
	@echo "   release64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   BulletLua"
	@echo "   Test"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
