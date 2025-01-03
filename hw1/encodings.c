#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define UTF8_MAX_BYTES 4
#define LAST_ASCII_SYMBOL 127

void transform(FILE *fp, const char *encoding, char *result);
void cp1251ToUtf8(int ch, char *result, int * i);
void koi8ToUtf8(int ch, char *result, int * i);
void iso8859_5ToUtf8(int ch, char *result, int * i);
void checkEncoding(const char *encoding);

int main(const int argc, const char * argv[])
{
    FILE* fin;
    FILE* fout;
    if (argc != 4) {
        printf("Usage: %s input_file encoding (iso8859-5, koi8, cp1251) output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((fin = fopen(argv[1], "r")) == NULL) {
        printf("Error opening file named %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if ((fout = fopen(argv[3], "w")) == NULL) {
        printf("Error creating file named %s\n", argv[3]);
        fclose(fin);
        exit(EXIT_FAILURE);
    }
    checkEncoding(argv[2]);
    fseek(fin, 0, SEEK_END);
    const long size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    char *result = malloc(size*sizeof(char) * UTF8_MAX_BYTES);
    transform(fin, argv[2], result);
    fprintf(fout, "%s", result);
    free(result);
    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}

void transform(FILE *fp, const char *encoding, char *result)
{
    if (result == NULL) {
        printf("Result string pointer is null\n");
        exit(EXIT_FAILURE);
    }
    int ch;
    int index = 0;
    if (strcmp("cp1251", encoding) == 0) { //cp-1251
        while ((ch = getc(fp)) != EOF) {
            if (ch <= LAST_ASCII_SYMBOL) {
                result[index++] = (char) ch;
                continue;
            }
            cp1251ToUtf8(ch, result, &index);
        }
    } else if (strcmp("iso8859_5", encoding) == 0) { //iso8859_5
        while ((ch = getc(fp)) != EOF) {
            if (ch <= LAST_ASCII_SYMBOL) {
                result[index++] = (char) ch;
                continue;
            }
            iso8859_5ToUtf8(ch, result, &index);
        }
    } else if (strcmp("koi8", encoding) == 0) { //koi8
        while ((ch = getc(fp)) != EOF) {
            if (ch <= LAST_ASCII_SYMBOL) {
                result[index++] = (char) ch;
                continue;
            }
            koi8ToUtf8(ch, result, &index);
        }
    }

    result[index] = '\0';
}

void cp1251ToUtf8(const int ch, char *result, int * i)
{
    if (ch == 168) { //Ё
        result[(*i)++] = '\320';
        result[(*i)++] = '\201';
    } else if (ch == 184) { //ё
        result[(*i)++] = '\320';
        result[(*i)++] = '\221';
    } else if (ch >= 192 && ch < 240) {
        result[(*i)++] = '\320';
        result[(*i)++] = (char) (ch - 48);
    } else if (ch >= 240 && ch <= 255) {
        result[(*i)++] = '\321';
        result[(*i)++] = (char) (ch - 112);
    }
}

void iso8859_5ToUtf8(const int ch, char *result, int * i)
{
    if (ch == 161) { //Ё
        result[(*i)++] = '\320';
        result[(*i)++] = '\201';
    } else if (ch == 241) { //ё
        result[(*i)++] = '\320';
        result[(*i)++] = '\221';
    } else if (ch >= 176 && ch < 224) {
        result[(*i)++] = '\320';
        result[(*i)++] = (char) (ch - 32);
    } else if (ch >= 224 && ch <= 239) {
        result[(*i)++] = '\321';
        result[(*i)++] = (char) (ch - 96);
    }
}

void koi8ToUtf8(const int ch, char *result, int * i)
{
    static char map[256][2] = {
        [225] = {'\320','\220'},
        [226] = {'\320','\221'},
        [247] = {'\320','\222'},
        [231] = {'\320','\223'},
        [228] = {'\320','\224'},
        [229] = {'\320','\225'},
        [246] = {'\320','\226'},
        [250] = {'\320','\227'},
        [233] = {'\320','\230'},
        [234] = {'\320','\231'},
        [235] = {'\320','\232'},
        [236] = {'\320','\233'},
        [237] = {'\320','\234'},
        [238] = {'\320','\235'},
        [239] = {'\320','\236'},
        [240] = {'\320','\237'},
        [242] = {'\320','\240'},
        [243] = {'\320','\241'},
        [244] = {'\320','\242'},
        [245] = {'\320','\243'},
        [230] = {'\320','\244'},
        [232] = {'\320','\245'},
        [227] = {'\320','\246'},
        [254] = {'\320','\247'},
        [251] = {'\320','\250'},
        [253] = {'\320','\251'},
        [255] = {'\320','\252'},
        [249] = {'\320','\253'},
        [248] = {'\320','\254'},
        [252] = {'\320','\255'},
        [224] = {'\320','\256'},
        [241] = {'\320','\257'},
        [193] = {'\320','\260'},
        [194] = {'\320','\261'},
        [215] = {'\320','\262'},
        [199] = {'\320','\263'},
        [196] = {'\320','\264'},
        [197] = {'\320','\265'},
        [214] = {'\320','\266'},
        [218] = {'\320','\267'},
        [201] = {'\320','\270'},
        [202] = {'\320','\271'},
        [203] = {'\320','\272'},
        [204] = {'\320','\273'},
        [205] = {'\320','\274'},
        [206] = {'\320','\275'},
        [207] = {'\320','\276'},
        [208] = {'\320','\277'},
        [210] = {'\321','\200'},
        [211] = {'\321','\201'},
        [212] = {'\321','\202'},
        [213] = {'\321','\203'},
        [198] = {'\321','\204'},
        [200] = {'\321','\205'},
        [195] = {'\321','\206'},
        [222] = {'\321','\207'},
        [219] = {'\321','\210'},
        [221] = {'\321','\211'},
        [223] = {'\321','\212'},
        [217] = {'\321','\213'},
        [216] = {'\321','\214'},
        [220] = {'\321','\215'},
        [192] = {'\321','\216'},
        [209] = {'\321','\217'},
        [179] = {'\320','\201'},
        [163] = {'\320','\221'},
    };

    if (ch > 255 || ch < 128) { // NOT a koi8 symbol (слабая проверка, но лень делать что-то лучше:))
        return;
    }

    result[(*i)++] = map[ch][0];
    result[(*i)++] = map[ch][1];
}

void checkEncoding(const char *encoding)
{
    static char availableEncodings[][10] = {
        "iso8859-5",
        "koi8",
        "cp1251"
    };
    int i = 0;
    const int size = sizeof(availableEncodings) / sizeof(availableEncodings[0]);
    while (i < size) {
        if (strcmp(encoding, availableEncodings[i++]) == 0) {
            return;
        }
    }

    printf("Unsupported encoding: %s, please provide one of (iso8859-5, koi8, cp1251).\n", encoding);
    exit(EXIT_FAILURE);
}
