#ifndef __RSA_ENC_DEC__H
#define __RSA_ENC_DEC__H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

enum key_size {
    rsa_2048 = 292,
    rsa_4096 = 548
};

class rsa_enc_dec {
    public:
        rsa_enc_dec();

        void generate_keys(unsigned int key_bits);

        std::string encrypt_text(std::string text, CryptoPP::RSA::PublicKey& public_key);

        std::string decrypt_text(std::string text, CryptoPP::RSA::PrivateKey& private_key);

        bool getStatus();

        std::string getPublicKey();

        std::string getTheirPublicKey();

        void setTheirPublicKey(std::string& their_public_key);

    private:
        CryptoPP::AutoSeededRandomPool _rng;
        CryptoPP::RSA::PrivateKey _private_key;
        CryptoPP::RSA::PublicKey _public_key;
        CryptoPP::RSA::PublicKey _their_public_key;
        bool _encryption_enabled;
};

#endif