#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

// Вспомогательные функции F, G, H и циклический сдвиг
// Если x, то y; иначе z. (x & y) | (~x & z)
#define F(x, y, z) ((x & y) | ((~x) & z))
// Мажоритарная функция. (x & y) | (x & z) | (y & z)
#define G(x, y, z) ((x & y) | (x & z) | (y & z))
// Побитовый XOR. x ^ y ^ z
#define H(x, y, z) (x ^ y ^ z)
// Циклический сдвиг влево
#define left_rotate(x, n) ((x << n) | (x >> (32 - n)))
// Преобразование 32 битного регистра в little-endian запись
#define le(x) (((x & 0xFF) << 24) | (((x >> 8) & 0xFF) << 16) | \
    (((x >> 16) & 0xFF) << 8) | ((x >> 24) & 0xFF))


char* md4(const char* message){
    char* hash = malloc(33); // Финальный хэш

    uint32_t msg_len = (uint32_t)strlen(message); // Длина сообщения в байтах

    uint32_t data_len = (msg_len + 64 - (msg_len % 64)); // Длина массива байт
    uint8_t* data = calloc(data_len, sizeof(uint8_t)); // Массив заполненный нулями
    if (!data) { // Не удалось выделить память
        fprintf(stderr, "Sorry, can't allocate memory for you!");
        exit(1);
    }

    memcpy(data, message, msg_len); // Копируем оригинальную строку в начало
    data[msg_len] = 0x80; // Единица и 7 нулей
    uint64_t bit_msg_len = msg_len * 8; // Длина сообщения в битах

    for (int i = 0; i < 8; i++) {
        data[data_len - 8 + i] = (uint8_t)(bit_msg_len >> (i * 8));
    }

    // Регистры ABCD (записаны в Little-endian)
    uint32_t A = 0x67452301; // 01 23 45 67
    uint32_t B = 0xefcdab89; // 89 ab cd ef
    uint32_t C = 0x98badcfe; // fe dc ba 98
    uint32_t D = 0x10325476; // 76 54 32 10

    // Сдвиги раундов
    const uint8_t s1[4] = {3, 7, 11, 19};
    const uint8_t s2[4] = {3, 5, 9, 13};
    const uint8_t s3[4] = {3, 9, 11, 15};

    // Константы раундов
    const uint32_t c2 = 0x5A827999;
    const uint32_t c3 = 0x6ED9EBA1;

    // Порядки слову в раундах
    const uint8_t ord2[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
    const uint8_t ord3[16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

    // Массив под 4 байтовые слова из 16 элеменентов
    uint32_t words[16];

    for (uint32_t ofs=0; ofs < data_len; ofs += 64) {
        // Сохраняем текущие значения регистров
        uint32_t AA = A;
        uint32_t BB = B;
        uint32_t CC = C;
        uint32_t DD = D;

        // Получение текущего 64 байтового блока
        const uint8_t* block = data + ofs;
        
        // Заполнение массива 4 байтовых слов
        for (int i = 0; i < 16; i++) {
            words[i] = (uint32_t)block[i*4] |
            ((uint32_t)block[i*4 + 1] << 8) |
            ((uint32_t)block[i*4 + 2] << 16) |
            ((uint32_t)block[i*4 + 3] << 24);
        }

        uint32_t temp;
        // Раунды 1
        for (uint8_t i=0; i < 16; i++) {
            A = left_rotate(
                (A + F(B, C, D) + words[i]),
                s1[i%4]);
            // Циклически сдвигаем (A,B,C,D) -> (D,A,B,C)
            temp = D;
            D = C;
            C = B;
            B = A;
            A = temp;
            
        }

        // Раунд 2
        for (uint8_t i=0; i < 16; i++) {
            A = left_rotate(
                (A + G(B, C, D) + words[ord2[i]] + c2),
                s2[i%4]);
            // Циклически сдвигаем (A,B,C,D) -> (D,A,B,C)
            temp = D;
            D = C;
            C = B;
            B = A;
            A = temp;
        }

        // Раунд 3
        for (uint8_t i=0; i < 16; i++) {
            A = left_rotate(
                (A + H(B, C, D) + words[ord3[i]] + c3),
                s3[i%4]);
            // Циклически сдвигаем (A,B,C,D) -> (D,A,B,C)
            temp = D;
            D = C;
            C = B;
            B = A;
            A = temp;
        }

        // Обновляем регистры для следующего блока
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    free(data); // Освобождение памяти
    // Формируем хэш как ABCD записанные в little endian
    snprintf(hash, 33, "%08x%08x%08x%08x", le(A), le(B), le(C), le(D));
    return hash; // Хэш
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [-h] message [message ...]\n", argv[0]);
        return 1;
    }
    int opt;
    const char* usage = "usage: md4.py [-h] message [message ...]\n";

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
            case 'h':
                fprintf(stdout, "%s", usage);
                fprintf(stdout, "\nProgrames create MD4 hash for string(s).\n\npositional arguments:\n" 
                    "  message   The message or messages for which the hash will be calculated.\n\n"
                    "options:\n  -h,   show this help message and exit\n");
                return 0;
            default:
                fprintf(stdout, "%s", usage);
                fprintf(stderr, "%s: error: the following arguments are required: message", argv[0]);
                return 1;
        }
    }

    char* hash;
    for (int i = optind; i < argc; i++) {
        hash = md4(argv[i]);
        fprintf(stdout, "%s\n", hash);
        free(hash);
    }
    return 0;
}