#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CentralDirectoryFileHeader {
    // Обязательная сигнатура, равна 0x02014b50
    // uint32_t signature;
    // Версия для создания
    uint16_t versionMadeBy;
    // Минимальная версия для распаковки
    uint16_t versionToExtract;
    // Битовый флаг
    uint16_t generalPurposeBitFlag;
    // Метод сжатия (0 - без сжатия, 8 - deflate)
    uint16_t compressionMethod;
    // Время модификации файла
    uint16_t modificationTime;
    // Дата модификации файла
    uint16_t modificationDate;
    // Контрольная сумма
    uint32_t crc32;
    // Сжатый размер
    uint32_t compressedSize;
    // Несжатый размер
    uint32_t uncompressedSize;
    // Длина название файла
    uint16_t filenameLength;
    // Длина поля с дополнительными данными
    uint16_t extraFieldLength;
    // Длина комментариев к файлу
    uint16_t fileCommentLength;
    // Номер диска
    uint16_t diskNumber;
    // Внутренние аттрибуты файла
    uint16_t internalFileAttributes;
    // Внешние аттрибуты файла
    uint32_t externalFileAttributes;
    // Смещение до структуры LocalFileHeader
    uint32_t localFileHeaderOffset;
    // Имя файла (длиной filenameLength)
    // uint8_t *filename;
    // Дополнительные данные (длиной extraFieldLength)
    // uint8_t *extraField;
    // // Комментарий к файла (длиной fileCommentLength)
    // uint8_t *fileComment;
} __attribute__((packed));

bool isZipArchive(FILE* fin);
void printContent(FILE* fin);

int main(const int argc, const char * argv[])
{
    FILE* fin;

    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((fin = fopen(argv[1], "r")) == NULL) {
        printf("Error opening file named %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (isZipArchive(fin)) {
        printf("It's a Zip archive\n");
        printContent(fin);
    } else {
        printf("It's not a zip archive\n");
    }
    fclose(fin);

    return EXIT_SUCCESS;
}

bool isZipArchive(FILE* fin) {
    char buffer[4];
    int i = 0;
    while (fseek(fin, i, SEEK_SET) == 0) {
        if (fread(buffer, 1,4, fin) != 4) {
            break;
        }
        if (strncmp(buffer, "\x50\x4b\x05\x06", 4) == 0) {
            return true;
        }
        i++;
    }

    return false;
}

void printContent(FILE* fin)
{
    struct CentralDirectoryFileHeader centralDirectoryFileHeader;
    char buffer[4];
    int i = 0;
    while (fseek(fin, i, SEEK_SET) == 0) {
        if (fread(buffer, 1,4, fin) != 4) {
            break;
        }
        if (strncmp(buffer, "\x50\x4b\x01\x02", 4) == 0) {
            fread(&centralDirectoryFileHeader, sizeof(struct CentralDirectoryFileHeader), 1, fin);
            char *filename = malloc(centralDirectoryFileHeader.filenameLength + 1);
            fread(filename, centralDirectoryFileHeader.filenameLength, 1, fin);
            filename[centralDirectoryFileHeader.filenameLength] = '\0';
            printf("%s\n", filename);
            free(filename);
        }
        i++;
    }
}