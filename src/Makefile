CROSS_COMPILE ?=

CM160 = cm160
DB_IMPORT = db_import

CC	:= $(CROSS_COMPILE)gcc
LD	:= $(CROSS_COMPILE)gcc
CFLAGS	:= -W -Wall -Wno-multichar -O2 #-Werror
CFLAGS	+= -Isqlite-amalgamation-3230100 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_DECLTYPE -DSQLITE_OMIT_DECLTYPE -DSQLITE_OMIT_DEPRECATED -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_SHARED_CACHE
LDFLAGS :=
LIBS	:= -lusb -lpthread -ldl

OBJ_CM160 := cm160.o usb_utils.o db.o demonize.o sqlite-amalgamation-3230100/sqlite3.o
OBJ_DBIMP := db_import.o db.o sqlite-amalgamation-3230100/sqlite3.o

all: $(CM160) $(DB_IMPORT)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(CM160): $(OBJ_CM160)
	$(LD) $(LDFLAGS) -o $@ $(OBJ_CM160) $(LIBS)

$(DB_IMPORT): $(OBJ_DBIMP)
	$(LD) $(LDFLAGS) -o $@ $(OBJ_DBIMP) $(LIBS)

clean:
	-$(RM) *.o sqlite-amalgamation-3230100/*.o
	-$(RM) $(CM160)
	-$(RM) $(DB_IMPORT)
