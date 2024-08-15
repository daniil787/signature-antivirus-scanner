#include "sha256hash.h"
#include <wincrypt.h>

void getSHA256(const BYTE* data, DWORD size, BYTE* hash) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;

    if (CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
            if (CryptHashData(hHash, data, size, 0)) {
                DWORD hashSize = 32;
                CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0);
            }
            CryptDestroyHash(hHash);
        }
        CryptReleaseContext(hProv, 0);
    }
}
