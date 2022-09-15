#define _XOPEN_SOURCE 600 
#include <sys/inotify.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ftw.h>
#include <stdint.h>
#define BUF_LEN 10*(sizeof(struct inotify_event) + NAME_MAX + 1)
void display_all_event(struct inotify_event*,int,char**);
static int display_file_info(const char *fpath, const struct stat *sb,int flag, struct FTW *ftw);
static int ifd;
int main(int argc,char *argv[]){
    ifd = inotify_init();
    int flags = 0;
    if (nftw((argc > optind) ? argv[optind] : ".", display_file_info, 10, flags) == -1) {
        perror("nftw error");
        exit(1);
    }
    char buf[BUF_LEN] = "";
    ssize_t bytes;
    while(1){
        bytes = read(ifd,buf,BUF_LEN);
        if(bytes <= 0){
            perror("read error");
            exit(1);
        }
        char *ptr;
        for(ptr = buf ; ptr < (buf + BUF_LEN) ; ptr += (sizeof(struct inotify_event) + NAME_MAX + 1)){
            display_all_event((struct inotify_event*) ptr,argc,argv);
        }
        memset(buf,0,sizeof(buf));
    }
}
void display_all_event(struct inotify_event *i, int argc, char *argv[]){
    if(i -> wd > 0 && i -> len > 0){
        printf("wd = %d  ",i -> wd);
        if(i -> cookie > 0) printf("cookie = %d  ",i -> cookie);
        printf("name = %s\n",i -> name);
        printf("mask: ");
        if (i->mask & IN_ACCESS) printf("ACCESS ");
        if (i->mask & IN_ATTRIB) printf("ATTRIB ");
        if (i->mask & IN_CLOSE_NOWRITE) printf("CLOSE_NOWRITE ");
        if (i->mask & IN_CLOSE_WRITE) printf("CLOSE_WRITE ");
        if (i->mask & IN_CREATE){
            printf("CREATE \n");
             int flags = 0;
             if (nftw((argc > optind) ? argv[optind] : ".", display_file_info, 10, flags) == -1) {
                perror("nftw error");
                exit(1);
            }
        }
        if (i->mask & IN_DELETE){ 
            printf("DELETE \n");
            int flags = 0;
             if (nftw((argc > optind) ? argv[optind] : ".", display_file_info, 10, flags) == -1) {
                perror("nftw error");
                exit(1);
            }
        }
        if (i->mask & IN_DELETE_SELF) printf("DELETE_SELF ");
        if (i->mask & IN_IGNORED) printf("IGNORED ");
        if (i->mask & IN_ISDIR) printf("ISDIR ");
        if (i->mask & IN_MODIFY) printf("MODIFY ");
        if (i->mask & IN_MOVE_SELF) printf("MOVE_SELF ");
        if (i->mask & IN_MOVED_FROM){ //file rename
            printf("MOVED_FROM \n");
            int flags = 0;
             if (nftw((argc > optind) ? argv[optind] : ".", display_file_info, 10, flags) == -1) {
                perror("nftw error");
                exit(1);
            }
        }
        if (i->mask & IN_MOVED_TO){ //file rename
            printf("MOVED_TO \n");
            int flags = 0;
             if (nftw((argc > optind) ? argv[optind] : ".", display_file_info, 10, flags) == -1) {
                perror("nftw error");
                exit(1);
            }
        }
        if (i->mask & IN_OPEN) printf("OPEN ");
        if (i->mask & IN_Q_OVERFLOW) printf("Q_OVERFLOW ");
        if (i->mask & IN_UNMOUNT) printf("UNMOUNT ");
        printf("\n");
    }
}
static int display_file_info(const char *fpath, const struct stat *sb,int flag, struct FTW *ftw){
    int wd;
    wd = inotify_add_watch(ifd,fpath,IN_ALL_EVENTS);
    if(wd < 0){
        perror("inotify error");
        exit(1);
    }
    printf("Watching wd:%d\n",wd);
    return 0;
}