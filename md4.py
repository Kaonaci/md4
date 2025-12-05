#!/usr/bin/python3

def md4(message: str|int|float) -> str:
    # Превращаем message в строку
    if not isinstance(message, str):
        message = str(message)
    # Превращаем строку с массив байтов
    message_bytes = message.encode("utf-8")

    length = len(message) # Длина изначального сообщения в байтах

    # Подготовка строки
    data = bytearray(length + 64 - (length % 64)) # Формируем буфер нужного размера
    data[:length] = message_bytes # Записываем в него байты строки
    data[length] = 0x80 # 1000 0000 в Little-endian - единичный бит и нули после него
    data[-8:] = (length*8).to_bytes(8, byteorder="little") # Последние 8 байт - длинa строки

    # Регистры ABCD (записаны в Little-endian)
    A = 0x67452301 # 01 23 45 67
    B = 0xefcdab89 # 89 ab cd ef
    C = 0x98badcfe # fe dc ba 98
    D = 0x10325476 # 76 54 32 10
    MASK = 0xFFFFFFFF # Для приведения к 32 битам (некоторые операции в python выходят за 32 бита)

    # Вспомогательные функции F, G, H и циклический сдвиг
    def F(x, y, z):
        """Если x, то y; иначе z. (x & y) | (~x & z)"""
        return ((x & y) | ((~x) & z)) & MASK
    
    def G(x, y, z):
        """Мажоритарная функция. (x & y) | (x & z) | (y & z)"""
        return ((x & y) | (x & z) | (y & z)) & MASK
    
    def H(x, y, z):
        """Побитовый XOR. x ^ y ^ z"""
        return (x ^ y ^ z) & MASK
    
    def left_rotate(x, n):
        """Циклический сдвиг влево на n бит для 32-битного числа"""
        return ((x << n) | (x >> (32 - n))) & MASK

    # Применение алгоритма к строкам по 512 бит ()
    for ofs in range(0, len(data), 64):
        block = data[ofs: ofs + 64]
        # Список из 16 слов размером 4 байта (32 битные слова)
        words = [int.from_bytes(block[4*i:4*i+4], "little")
                 for i in range(16)]
        # Сохраняем текущие значения регистров
        AA = A
        BB = B
        CC = C
        DD = D

        # Раунд 1 (функция F) - работа с 32-битными словами (4 байта)
        s1 = [3, 7, 11, 19] # Сдвиги раунда
        for i in range(16):
            # A = (A + F(B, C, D) + W[i]) << s1[i]
            A = left_rotate(
                ((A + F(B, C, D) + words[i]) & MASK),
                s1[i%4]);
            A, B, C, D = D, A, B, C # Циклически сдвигаем (A,B,C,D) -> (D,A,B,C)

        # Раунд 2 (функция G) - работа с 32-битными словами (4 байта)
        const_r2 = 0x5A827999 # Константа раунда = 0x5A827999
        order2 = [0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15] # Порядок слов на раунд
        s2 = [3, 5, 9, 13] # Сдвиги раунда
        for i in range(16):
            # A = (A + G(B, C, D) + W[order2[i]] + C2) << s2[i]
            A = left_rotate(
                ((A + G(B, C, D) + words[order2[i]] + const_r2) & MASK),
                s2[i%4]);
            A, B, C, D = D, A, B, C # Циклически сдвигаем (A,B,C,D) -> (D,A,B,C)

        # Раунд 3
        const_r3 = 0x6ED9EBA1 # Константа раунда = 0x6ED9EBA1
        order3 = [0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15] # Порядок слов на раунд
        s3 = [3, 9, 11, 15] # Сдвиги раунда
        for i in range(16):
            # A = (A + G(B, C, D) + W[order2[i]] + C3) << s3[i]
            A = left_rotate(
                ((A + H(B, C, D) + words[order3[i]] + const_r3) & MASK),
                s3[i%4]);
            A, B, C, D = D, A, B, C # Циклически сдвигаем (A,B,C,D) -> (D,A,B,C)

        # Обновляем регистры для следующего блока
        A = (A + AA) & MASK
        B = (B + BB) & MASK
        C = (C + CC) & MASK
        D = (D + DD) & MASK

    # Формирование хэша (хэш сформирован как ABCD в little-endian)
    hashed_message = b"".join([
        A.to_bytes(4, byteorder="little"),
        B.to_bytes(4, byteorder="little"),
        C.to_bytes(4, byteorder="little"),
        D.to_bytes(4, byteorder="little"),
    ])
    return hashed_message.hex()


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        description="Programes create MD4 hash for string(s).",
        )
    parser.add_argument(
        "message",
        type=str,
        nargs="+",
        help="The message or messages for which the hash will be calculated."
                        )
    args = parser.parse_args()
    for message in args.message:
        print(md4(message))
# 