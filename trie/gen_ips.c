/*
 * gen_ips.c
 *
 *
 *
 * Copyright (C) 2012-10-19 liutos <mat.liutos@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i;
    FILE *fp;

    if (NULL == (fp = fopen("ips.txt", "w"))) {
        printf("Can not write file `ips.txt'.\n");
        exit(1);
    }
    for (i = 0; i < 40; i++) {
        fprintf(fp, "%d.%d.%d.%d\n",
                rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    }
    fclose(fp);

    return 0;
}

