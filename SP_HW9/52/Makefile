all: svmsg_file_server svmsg_file_client
svmsg_file_server: tlpi_hdr.h svmsg_file.h svmsg_file_server.c get_num.h get_num.c error_functions.h error_functions.c
	gcc tlpi_hdr.h svmsg_file.h svmsg_file_server.c get_num.h get_num.c error_functions.h error_functions.c -o svmsg_file_server -lrt
svmsg_file_client: tlpi_hdr.h svmsg_file.h svmsg_file_client.c get_num.h get_num.c error_functions.h error_functions.c
	gcc tlpi_hdr.h svmsg_file.h svmsg_file_client.c get_num.h get_num.c error_functions.h error_functions.c -o svmsg_file_client -lrt