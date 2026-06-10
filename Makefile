# ----- MAKE FILE FOR wlcov CODE -----
# Mario A. Rodriguez-Meza, Ciudad de Mexico, 01.05.2026
#
#
# Nothing to do in this file. Make your settings in Makefile_settings file only
#
MACHINES_DIR = ./
# Machine definitions and code settings. Edit this file according to your needs
include $(MACHINES_DIR)Makefile_settings

#
# Nothing to do below
#

EXECPREFIX = wlcov
EXEC = $(EXECPREFIX)

$(info )
$(info =====================================================)
$(info EXEC = [${EXEC}])
$(info External options = [${OPT2}])
$(info =====================================================)
$(info )

MAIN = main.o

ifeq ($(ADDONSON),0)
OBJS = main.o
else
OBJS = main.o startrun.o wlcov.o wlcovio.o abi_check.o
endif

PYTHON_FILES = python/wlcovpy.pyx setup.py python/cwlcovpy.pxd.in

all: $(EXEC) lib$(EXEC).a wlcovpy

lib$(EXEC).a: $(OBJS) $(EXTERNAL)
	$(AR)  $@ $(addprefix build/, $(OBJS) $(TOOLS) $(SOURCE) $(EXTERNAL) $(EXTERNALCXX))

$(EXEC): $(OBJS) $(EXTERNAL) $(MAIN)
	$(CC) $(OPTFLAG) $(OMPFLAG) $(LDFLAG) -o $(EXEC) $(addprefix build/,$(notdir $^)) $(MLIBS)

wlcovpy: lib$(EXEC).a python/wlcovpy.pyx python/cwlcovpy.pxd.in setup.py Makefile_settings addons/Makefile_addons_settings
	export CC=$(CC); \
	output=$$($(PYTHON) -m pip install . 2>&1); \
	status=$$?; \
	echo "$$output"; \
	if [ $$status -ne 0 ]; then \
	    if echo "$$output" | grep -q "ERROR: Cannot uninstall"; then \
	        site_packages=$$($(PYTHON) -c "import sysconfig; print(sysconfig.get_paths()['purelib'])" || $(PYTHON) -c "import site; print(site.getsitepackages()[0])"); \
	        echo "Cleaning up previous installation in: $$site_packages"; \
	        rm -rf "$$site_packages"/wlcovpy*; \
	        $(PYTHON) -m pip install .; \
	    else \
	        exit $$status; \
	    fi; \
	fi

.PHONY : clean
clean: .base
	rm -rf $(WRKDIR);
	rm -f $(EXEC)
	rm -f lib$(EXEC).a
	rm -f $(MDIR)/python/wlcovpy.c
	rm -rf $(MDIR)/python/build
	rm -rf wlcovpy.egg-info
	rm -f $(MDIR)/python/cwlcovpy.pxd
