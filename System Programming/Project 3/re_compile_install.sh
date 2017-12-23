#!/bin/sh
{
	fusermount -u /home/coban/Desktop/mnt;
	gcc cityPostalCodes.c libcsv.o -o cityPostalCodes -D_FILE_OFFSET_BITS=64 -lfuse;
	./cityPostalCodes /home/coban/Desktop/mnt;
}
