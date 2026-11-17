#ifndef HASHES_H
#define HASHES_H
#define TAM_TABELA 53          /* M: numero de chaves */

unsigned int hashFNV(const char *nome);
unsigned int hashXXH3(const char *nome);
unsigned int hashWyhash(const char *nome);
unsigned int hashMurmur3(const char *nome);

#endif