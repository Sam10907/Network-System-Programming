all: svshm_xfr_writer svshm_xfr_reader
svshm_xfr_writer: svshm_xfr_writer.c svshm_xfr.h binary_sems.h binary_sems.c semun.h tlpi_hdr.h error_functions.h error_functions.c get_num.h get_num.c
	gcc svshm_xfr_writer.c svshm_xfr.h binary_sems.h binary_sems.c semun.h tlpi_hdr.h error_functions.h error_functions.c get_num.h get_num.c -o svshm_xfr_writer -lrt
svshm_xfr_reader: svshm_xfr_reader.c svshm_xfr.h binary_sems.h binary_sems.c semun.h tlpi_hdr.h error_functions.h error_functions.c get_num.h get_num.c
	gcc svshm_xfr_reader.c svshm_xfr.h binary_sems.h binary_sems.c semun.h tlpi_hdr.h error_functions.h error_functions.c get_num.h get_num.c -o svshm_xfr_reader -lrt