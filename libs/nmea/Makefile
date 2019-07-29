CC = gcc 
 
BIN = build/libnmea.a 
MODULES = generate generator parse parser tok context nmeatime info gmath sentence  
 
OBJ = $(MODULES:%=build/%.o) 
LINKOBJ = $(OBJ) $(RES)


INCS = -I include 
LIBS = -lm -Llib -lnmea
 
.PHONY: all all-before all-after clean clean-custom doc
 
all: all-before $(BIN) all-after 
	mv libnmea.a ../../build/

all-before:
	mkdir -p build

clean: clean-custom 
	rm -f $(LINKOBJ) $(BIN) $(SMPLOBJ) $(SMPLS)

doc:
	$(MAKE) -C doc
	
remake: clean all

$(BIN): $(LINKOBJ)
	ar rsc $@ $^
	ranlib $@
	mv $@ ../../build/$@

build/%.o: src/%.c 
	$(CC) $(INCS) -c $< -o $@


