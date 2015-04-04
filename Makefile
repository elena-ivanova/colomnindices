# Modify Makefile for zlib
# Copyright (C) 1995-2013 Jean-loup Gailly, Mark Adler
# For conditions of distribution and use, see copyright notice in zlib.h

PROGNAME=slave
CC=mpicc
DIR=./zlib/
CFLAGS= -mmic -openmp -D_LARGEFILE64_SOURCE=1 -DHAVE_HIDDEN
EXE=.mic

OBJCI = slave.o common.o init.o execute.o finalize.o print.o domain.o cilocate.o cigenerate.o ciload.o db.o cicompress.o
OBJZ = $(DIR)adler32.o $(DIR)crc32.o $(DIR)deflate.o $(DIR)infback.o $(DIR)inffast.o $(DIR)inflate.o $(DIR)inftrees.o $(DIR)trees.o $(DIR)zutil.o
OBJG = $(DIR)compress.o $(DIR)uncompr.o $(DIR)gzclose.o $(DIR)gzlib.o $(DIR)gzread.o $(DIR)gzwrite.o
OBJS = $(OBJCI) $(OBJZ) $(OBJG)

all: $(PROGNAME)$(EXE)

$(PROGNAME)$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o *~ $(DIR)*.o \
	   $(PROGNAME)$(EXE) \
	   infcover \
	   foo.gz so_locations \
	   _match.s maketree contrib/infback9/*.o
	rm -rf objs
	rm -f *.gcda *.gcno *.gcov
	rm -f contrib/infback9/*.gcda contrib/infback9/*.gcno contrib/infback9/*.gcov

# DO NOT DELETE THIS LINE -- make depend depends on it.

adler32.o zutil.o: zutil.h zlib.h zconf.h
gzclose.o gzlib.o gzread.o gzwrite.o: zlib.h zconf.h gzguts.h
compress.o example.o minigzip.o uncompr.o: zlib.h zconf.h
crc32.o: zutil.h zlib.h zconf.h crc32.h
deflate.o: deflate.h zutil.h zlib.h zconf.h
infback.o inflate.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h inffixed.h
inffast.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inftrees.o: zutil.h zlib.h zconf.h inftrees.h
trees.o: deflate.h zutil.h zlib.h zconf.h trees.h
