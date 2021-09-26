#  Initialize the MODULE as the ex3.c file. We can think of this as macro in C as it is useful to save time
#  and allow to change the value of MODULE quickly in one line.
#  ...
MODULE	 = proj2

#  Loadable module goals. (obj-m) specifies object files which are built as loadable kernel modules.
# The module is built from one source since the kbuild makefile adds the compiled output file ex3.o
#  ...
obj-m += $(MODULE).o

#  Note that on any linux distribution, we will find the kernel Makefile and headers in /lib/modules/[version]/build.
#  This is just store the location that has the kernel makefile and headers on our linux kernel.
#  ...
KERNELDIR ?= /lib/modules/$(shell uname -r)/build

#  This is to find out the current working directory in GNU Make. PWD will report the current presented working directory (pwd).
#  ...
PWD := $(shell pwd)

#  all is a conventional name. all target denotes that if you invoke it, make will build all what is needed to make a complete build.
#  So, it build ex3.c
#  ...
all: $(MODULE)


#  %.o: %.c tells make how to obtain an .o file given the corresponding .c file.
#  @echo "  CC      $<": just for print out string/information.
#  @$(CC) -c $< -o $@: specifies one target and one dependency and causes one invocation of $(CC) for each target.
#  ...
%.o: %.c
	@echo "  CC      $<"
	@$(CC) -c $< -o $@

#  Within the MODULE, this command is meant for re-execute the make command, however,
#  the make command first calls the makefile in the path pointed to by the -C option, then
#  execute the module target "modules"
#  ...
$(MODULE):
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

#  This command is meant to delete existing files in PWD before creating a new ones. Similar as above,
#  the command re-execute the make command, however, the make command first calls the make file in the path 
#  path pointed to by the -C option, then execute the module target "modules"
#  ...
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
