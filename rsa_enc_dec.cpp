#include "rsa_enc_dec.h"

rsa_enc_dec::rsa_enc_dec(unsigned int key_bits) :
    _rng(),
    _private_key(),
    _public_key() {
    generate_keys(key_bits);
}

void rsa_enc_dec::generate_keys(unsigned int key_bits) {
    _private_key.GenerateRandomWithKeySize(_rng, key_bits);
    _public_key.Initialize(_private_key.GetModulus(), _private_key.GetPublicExponent());
}

std::string rsa_enc_dec::encrypt_text(std::string plain_text) {
    std::string cipher_text;
    
    CryptoPP::RSAES_OAEP_SHA256_Encryptor encryptor(_public_key);

    CryptoPP::StringSource encrypt(plain_text, true,
    new CryptoPP::PK_EncryptorFilter(_rng, encryptor,
    new CryptoPP::StringSink(cipher_text)));

    return cipher_text;
}

std::string rsa_enc_dec::decrypt_text(std::string cipher_text) {
    std::string plain_text;

    CryptoPP::RSAES_OAEP_SHA256_Decryptor decryptor(_private_key);

    CryptoPP::StringSource decrypt(cipher_text, true,
    new CryptoPP::PK_DecryptorFilter(_rng, decryptor,
    new CryptoPP::StringSink(plain_text)));
    
    return plain_text;
}