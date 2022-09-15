all: fifo_seqnum_server fifo_seqnum_client_normal
fifo_seqnum_server: tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c fifo_seqnum_server.c fifo_seqnum.h
	gcc tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c fifo_seqnum_server.c fifo_seqnum.h -o fifo_seqnum_server
fifo_seqnum_client_normal: tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c fifo_seqnum_client_normal.c fifo_seqnum.h
	gcc tlpi_hdr.h get_num.h get_num.c error_functions.h error_functions.c fifo_seqnum_client_normal.c fifo_seqnum.h -o fifo_seqnum_client_normal