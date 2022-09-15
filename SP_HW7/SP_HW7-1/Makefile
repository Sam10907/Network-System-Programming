all: sig_speed_sigsuspend sig_speed_sigwaitinfo
sig_speed_sigsuspend: tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c sig_speed_sigsuspend.c
	gcc tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c sig_speed_sigsuspend.c -o sig_speed_sigsuspend
sig_speed_sigwaitinfo: tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c sig_speed_sigwaitinfo.c
	gcc tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c sig_speed_sigwaitinfo.c -o sig_speed_sigwaitinfo