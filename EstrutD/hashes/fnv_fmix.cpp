
#include <cstdint>
#include "../hashes.h"

unsigned int hashFNV(const char *nome) {
    const unsigned char *p =
        (const unsigned char *)nome;

    uint64_t h = 14695981039346656037ULL;

    unsigned int c;

    while (*p != '\0') {
        c = *p++;

        if (c >= 'a' && c <= 'z') {
            c -= 'a' - 'A';
        }

        h ^= c;
        h *= 1099511628211ULL;
    }

    // Finalizador MurmurHash3 fmix64
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;

    // Reducao para o intervalo 0..TAM_TABELA-1
    return (unsigned int)(
        ((h >> 32) * TAM_TABELA) >> 32
    );
}
