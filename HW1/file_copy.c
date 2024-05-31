#include <stdio.h>

#define BUFFER_SIZE 1024

int main() {
    FILE *sourceFile, *destinationFile;
    char buffer[BUFFER_SIZE];
    char sourceFileName[100], destinationFileName[100];
    size_t bytesRead;

    // 获取源文件名
    printf("Enter source file name: ");
    scanf("%s", sourceFileName);

    // 打开源文件
    sourceFile = fopen(sourceFileName, "r");
    if (sourceFile == NULL) {
        printf("Error opening source file.\n");
        return 1;
    }

    // 获取目标文件名
    printf("Enter destination file name: ");
    scanf("%s", destinationFileName);

    // 打开目标文件
    destinationFile = fopen(destinationFileName, "w");
    if (destinationFile == NULL) {
        printf("Error opening destination file.\n");
        fclose(sourceFile);
        return 1;
    }

    // 复制文件内容
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    // 关闭文件
    fclose(sourceFile);
    fclose(destinationFile);

    printf("File copied successfully.\n");

    return 0;
}
