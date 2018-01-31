
NATIVE_CC ?= gcc

_all: all

all: regDomainGenDb regDomainGenTxt
	./regDomainGenDb
	./regDomainGenTxt > regdb.txt

regDomainGenDb: $(WLANCMN_DIR)/include/regulatory/reg_dbschema.h $(WLANCMN_DIR)/include/regulatory/reg_dbvalues.h
	$(NATIVE_CC) -m32 -Wall -g \
	-I../include \
	-I$(WLANCMN_DIR)/include \
	regDomainGenDb.c -o regDomainGenDb

regDomainGenTxt: $(WLANCMN_DIR)/include/regulatory/reg_dbschema.h $(WLANCMN_DIR)/include/regulatory/reg_dbvalues.h
	$(NATIVE_CC) -m32 -Wall -g \
	-I../include \
	-I$(WLANCMN_DIR)/include \
	regDomainGenTxt.c -o regDomainGenTxt

clean:
	rm -f regDomainGenDb *.bin regulatoryData_*.h regDomainGenTxt regdb.txt

clobber: clean
