
#include <cstdint>
#include <cstddef>

// ============================================================
// MurmurHash3 x64 128-bit
// Implementacao autocontida.
// ============================================================

static uint64_t rotl64(uint64_t x, int r) {
    return (x << r) | (x >> (64 - r));
}

static uint64_t fmix64(uint64_t k) {
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;

    return k;
}

static uint64_t getblock64(const uint8_t *p) {
    return (uint64_t)p[0]
         | ((uint64_t)p[1] << 8)
         | ((uint64_t)p[2] << 16)
         | ((uint64_t)p[3] << 24)
         | ((uint64_t)p[4] << 32)
         | ((uint64_t)p[5] << 40)
         | ((uint64_t)p[6] << 48)
         | ((uint64_t)p[7] << 56);
}

// ============================================================
// MurmurHash3 x64 128
// ============================================================

static void murmur3_x64_128(
    const void *key,
    size_t len,
    uint32_t seed,
    uint64_t &out1,
    uint64_t &out2
) {
    const uint8_t *data = (const uint8_t *)key;

    const int nblocks = (int)(len / 16);

    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = 0x87c37b91114253d5ULL;
    const uint64_t c2 = 0x4cf5ad432745937fULL;

    // Processamento dos blocos de 16 bytes.
    for (int i = 0; i < nblocks; i++) {

        uint64_t k1 = getblock64(data + i * 16);
        uint64_t k2 = getblock64(data + i * 16 + 8);

        k1 *= c1;
        k1 = rotl64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = rotl64(h1, 27);
        h1 += h2;
        h1 = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2 = rotl64(k2, 33);
        k2 *= c1;
        h2 ^= k2;

        h2 = rotl64(h2, 31);
        h2 += h1;
        h2 = h2 * 5 + 0x38495ab5;
    }

    // Processamento dos bytes restantes.
    const uint8_t *tail = data + nblocks * 16;

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch (len & 15) {

        case 15: k2 ^= (uint64_t)tail[14] << 48;
        case 14: k2 ^= (uint64_t)tail[13] << 40;
        case 13: k2 ^= (uint64_t)tail[12] << 32;
        case 12: k2 ^= (uint64_t)tail[11] << 24;
        case 11: k2 ^= (uint64_t)tail[10] << 16;
        case 10: k2 ^= (uint64_t)tail[9] << 8;
        case 9:
            k2 ^= (uint64_t)tail[8];
            k2 *= c2;
            k2 = rotl64(k2, 33);
            k2 *= c1;
            h2 ^= k2;

        case 8: k1 ^= (uint64_t)tail[7] << 56;
        case 7: k1 ^= (uint64_t)tail[6] << 48;
        case 6: k1 ^= (uint64_t)tail[5] << 40;
        case 5: k1 ^= (uint64_t)tail[4] << 32;
        case 4: k1 ^= (uint64_t)tail[3] << 24;
        case 3: k1 ^= (uint64_t)tail[2] << 16;
        case 2: k1 ^= (uint64_t)tail[1] << 8;
        case 1:
            k1 ^= (uint64_t)tail[0];
            k1 *= c1;
            k1 = rotl64(k1, 31);
            k1 *= c2;
            h1 ^= k1;
    }

    // Finalizacao.
    h1 ^= len;
    h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    out1 = h1;
    out2 = h2;
}

// ============================================================
// Interface do projeto
// ============================================================

unsigned int hashMurmur3(const char *nome) {

    // Normalizacao ASCII sem bibliotecas externas.
    // O buffer comporta nomes terminados em '\0'.
    uint8_t normalizado[1024];

    size_t len = 0;

    while (nome[len] != '\0' && len < sizeof(normalizado)) {

        unsigned char c = (unsigned char)nome[len];

        if (c >= 'a' && c <= 'z') {
            c -= 'a' - 'A';
        }

        normalizado[len] = c;
        len++;
    }

    uint64_t h1, h2;

    murmur3_x64_128(
        normalizado,
        len,
        0,
        h1,
        h2
    );

    // Combina as duas metades de 64 bits.
    uint64_t h = h1 ^ h2;

    // Reduz para o intervalo [0, 52].
    return (unsigned int)(((h >> 32) * 53ULL) >> 32);
}
