all: convert file_lookup
convert: convert.c dict.h
	gcc convert.c dict.h -o convert
file_lookup: lookup1.c main.c dict.h
	gcc lookup1.c main.c dict.h -o file_lookup