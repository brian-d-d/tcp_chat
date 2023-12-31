#include "rsa_enc_dec.h"

rsa_enc_dec::rsa_enc_dec() :
    _rng(),
    _private_key(),
    _public_key(),
    _their_public_key(),
    _encryption_enabled(false) {
}

void rsa_enc_dec::generate_keys(unsigned int key_bits) {
    _private_key.GenerateRandomWithKeySize(_rng, key_bits);
    _public_key.Initialize(_private_key.GetModulus(), _private_key.GetPublicExponent());
    _encryption_enabled = true;
}

std::string rsa_enc_dec::encrypt_text(std::string plain_text, CryptoPP::RSA::PublicKey& public_key) {
    std::string cipher_text;
    
    CryptoPP::RSAES_OAEP_SHA256_Encryptor encryptor(public_key);

    CryptoPP::StringSource encrypt(plain_text, true,
    new CryptoPP::PK_EncryptorFilter(_rng, encryptor,
    new CryptoPP::StringSink(cipher_text)));

    return cipher_text;
}

std::string rsa_enc_dec::decrypt_text(std::string cipher_text, CryptoPP::RSA::PrivateKey& private_key) {
    std::string plain_text;

    CryptoPP::RSAES_OAEP_SHA256_Decryptor decryptor(private_key);

    CryptoPP::StringSource decrypt(cipher_text, true,
    new CryptoPP::PK_DecryptorFilter(_rng, decryptor,
    new CryptoPP::StringSink(plain_text)));
    
    return plain_text;
}

bool rsa_enc_dec::getStatus() {
    return _encryption_enabled;
}

std::string rsa_enc_dec::getPublicKey() {
    std::string public_key;
    CryptoPP::StringSink sink(public_key);
    _public_key.DEREncode(sink);
    return public_key;
}

std::string rsa_enc_dec::getTheirPublicKey() {
    std::string public_key;
    CryptoPP::StringSink sink(public_key);
    _their_public_key.DEREncode(sink);
    return public_key;
}

void rsa_enc_dec::setTheirPublicKey(std::string& their_public_key) {
    CryptoPP::StringSource source(their_public_key, true);
    _their_public_key.Load(source);
}