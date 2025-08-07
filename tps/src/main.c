#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

int main() {
    DIR *dir_ptr;
    struct dirent *read_dir;

    dir_ptr = opendir("/proc");

    if (dir_ptr == NULL){
        perror("Can't open the proc directory\n");
        return 1;
    }

    char *str1 = "PID";
    char *str2 = "CMD";

    printf("%5s %s\n", str1, str2);
    //keep reading sub-dir PID & name until reached end of main dir
    while((read_dir = readdir(dir_ptr)) != NULL){
        char* pid = read_dir -> d_name;

        bool is_pid = true;
        for (int i = 0; pid[i] != '\0'; i++){
            if (!isdigit(pid[i])){
                is_pid = false;
                break;
            }
        }

        if (is_pid){
            char file_path[21] = "/proc/";
            strcat(file_path, pid);
            strcat(file_path, "/status");

            int file_id;
            ssize_t read_file;
            char buffer[1000];
            
            file_id = open(file_path, O_RDONLY);
            if (file_id < 0){
                perror("Can't open the file\n");
                continue;
            }

            read_file = read(file_id, buffer, 100);
            if (read_file == -1) {
                perror("Can't read the file\n");
            }

            bool start_collecting = false;
            char filename[50];
            int findex = 0;
            for (int i = 0; buffer[i] != '\n'; i++){
                if (buffer[i] == '\t'){
                    start_collecting = true;
                    continue;
                }

                if (start_collecting == true){
                    filename[findex] = buffer[i];
                    findex++;
                }
            }

            filename[findex] = '\0';  // Null-terminate the string
            printf("%5s %s\n",pid, filename);

            if (close(file_id) == -1){
                perror("Can't close the directory");
                return 1;
            }

        }
        
    }

    if (dir_ptr == NULL){
        perror("Can't open the file\n");
        return 1;
    }

    if (closedir(dir_ptr) == -1){
        perror("Can't close the directory");
        return 1;
    }

    return 0;
}

