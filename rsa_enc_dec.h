#ifndef __RSA_ENC_DEC__H
#define __RSA_ENC_DEC__H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

class rsa_enc_dec {
    public:
        rsa_enc_dec(unsigned int key_bits);

        void generate_keys(unsigned int key_bits);

        std::string encrypt_text(std::string text);

        std::string decrypt_text(std::string text);

    private:
        CryptoPP::AutoSeededRandomPool _rng;
        CryptoPP::RSA::PrivateKey _private_key;
        CryptoPP::RSA::PublicKey _public_key;
};

#endif