
#include <cstdint>
#include <cstddef>
#include <string>

// ============================================================
// Wyhash - Final version 4.3
// Implementacao autocontida, sem bibliotecas externas.
// ============================================================

static const uint64_t WY_SECRET[4] = {
    0x2d358dccaa6c78a5ULL,
    0x8bb84b93962eacc9ULL,
    0x4b33a62ed433d4a3ULL,
    0x4d5a2da51de1aa47ULL
};

// Leitura de 4 bytes em little-endian.
static uint32_t wy_read4(const uint8_t *p) {
    return (uint32_t)p[0]
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

// Leitura de 8 bytes em little-endian.
static uint64_t wy_read8(const uint8_t *p) {
    return (uint64_t)wy_read4(p)
         | ((uint64_t)wy_read4(p + 4) << 32);
}

// Leitura de 1 a 3 bytes.
static uint64_t wy_read3(const uint8_t *p, size_t len) {
    return ((uint64_t)p[0] << 16)
         | ((uint64_t)p[len >> 1] << 8)
         | p[len - 1];
}

// Multiplicacao de 128 bits.
// A metade inferior fica em A e a superior em B.
static void wy_mum(uint64_t *A, uint64_t *B) {
    __uint128_t resultado = (__uint128_t)(*A) * (*B);

    *A = (uint64_t)resultado;
    *B = (uint64_t)(resultado >> 64);
}

// Multiplicacao seguida de XOR.
static uint64_t wy_mix(uint64_t A, uint64_t B) {
    wy_mum(&A, &B);
    return A ^ B;
}

// ============================================================
// Funcao principal do wyhash 4.3
// ============================================================

static uint64_t wy_hash(
    const void *key,
    size_t len,
    uint64_t seed
) {
    const uint8_t *p = (const uint8_t *)key;

    seed ^= wy_mix(
        seed ^ WY_SECRET[0],
        WY_SECRET[1]
    );

    uint64_t a, b;

    if (len <= 16) {

        if (len >= 4) {
            a = ((uint64_t)wy_read4(p) << 32)
              | wy_read4(p + ((len >> 3) << 2));

            b = ((uint64_t)wy_read4(p + len - 4) << 32)
              | wy_read4(p + len - 4 - ((len >> 3) << 2));
        }
        else if (len > 0) {
            a = wy_read3(p, len);
            b = 0;
        }
        else {
            a = b = 0;
        }
    }
    else {
        size_t i = len;

        if (i >= 48) {
            uint64_t see1 = seed;
            uint64_t see2 = seed;

            do {
                seed = wy_mix(
                    wy_read8(p) ^ WY_SECRET[1],
                    wy_read8(p + 8) ^ seed
                );

                see1 = wy_mix(
                    wy_read8(p + 16) ^ WY_SECRET[2],
                    wy_read8(p + 24) ^ see1
                );

                see2 = wy_mix(
                    wy_read8(p + 32) ^ WY_SECRET[3],
                    wy_read8(p + 40) ^ see2
                );

                p += 48;
                i -= 48;

            } while (i >= 48);

            seed ^= see1 ^ see2;
        }

        while (i > 16) {
            seed = wy_mix(
                wy_read8(p) ^ WY_SECRET[1],
                wy_read8(p + 8) ^ seed
            );

            i -= 16;
            p += 16;
        }

        a = wy_read8(p + i - 16);
        b = wy_read8(p + i - 8);
    }

    a ^= WY_SECRET[1];
    b ^= seed;

    wy_mum(&a, &b);

    return wy_mix(
        a ^ WY_SECRET[0] ^ len,
        b ^ WY_SECRET[1]
    );
}

// ============================================================
// Interface do projeto
// ============================================================

unsigned int hashWyhash(const char *nome) {

    std::string normalizado(nome);

    // Normalizacao ASCII: minusculas para maiusculas.
    for (char &c : normalizado) {
        if (c >= 'a' && c <= 'z') {
            c -= 'a' - 'A';
        }
    }

    uint64_t h = wy_hash(
        normalizado.data(),
        normalizado.size(),
        0
    );

    // Reducao para o intervalo [0, 52].
    return (unsigned int)(((h >> 32) * 53ULL) >> 32);
}
