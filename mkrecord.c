
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int getFileSize(const char* path) {
    struct stat stat_result;
    if (stat(path,&stat_result)<0) {
        return -1;
    }
    return stat_result.st_size;
}

int main(int argc, char** argv) {
    if (argc<4) {
        printf("Please enter the correct amount of arguments\n");
        return -1;
    }

    int size = getFileSize(argv[1]);
    if (size<0) {
        printf("Unknown file!\n");
        return -1;
    }

    FILE* flip = fopen("record.out", "w");
    FILE* src = fopen(argv[1], "r");
    if (src==NULL || flip==NULL) {
        printf("Error, not proceeding!\n");
    }

    // Buffer
    // Byte[0] = 'R'
    // Byte[1] = 'E'
    // Byte[2] = 'C'
    // Byte[3] = '/
    // Byte[7:4] = Size
    // Byte[11:8] = Target Address
    // Byte[15:12] = options
    const char* header = "REC/";
    fwrite(header, 1, 4, flip);
    fwrite(&size, 4, 1, flip);
    unsigned int address = strtoul(argv[2], NULL, 16);
    fwrite(&address, 4, 1, flip);
    unsigned int options = strtoul(argv[3], NULL, 16);
    fwrite(&options, 4, 1, flip);
    char c;
    size_t readBytes = 1;
    size_t count = 0;
    while ((readBytes = fread(&c, 1, 1, src))>0) {
        count++;
        printf("\x1b[2K");
        printf("\rAdding %zu bytes into a flash record", count);
        fwrite(&c, 1, 1, flip);
    }
    printf("\n");
    fclose(flip);
    fclose(src);
    return 0;
}
