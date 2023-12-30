#include <iostream>
#include <string>
#include <cstdlib>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

int main() {

    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::RSA::PrivateKey priv_key;
    priv_key.GenerateRandomWithKeySize(rng, 2048);
    
    CryptoPP::RSA::PublicKey pub_key(priv_key);


    std::cout << priv_key.GetModulus() << std::endl << std::endl;
    std::cout << priv_key.GetPrivateExponent() << std::endl << std::endl;
    

    std::string plain_text = "123456789101";
    std::string cipher;
    std::string recovered;

    CryptoPP::RSAES_OAEP_SHA256_Encryptor encryptor(pub_key);
    CryptoPP::RSAES_OAEP_SHA256_Decryptor decryptor(priv_key);

    CryptoPP::StringSource ss1(plain_text, true,
    new CryptoPP::PK_EncryptorFilter(rng, encryptor,
        new CryptoPP::StringSink(cipher)
    ));

   CryptoPP::StringSource ss2(cipher, true,
    new CryptoPP::PK_DecryptorFilter(rng, decryptor,
        new CryptoPP::StringSink(recovered)
    )); 

   std::cout << cipher << std::endl;
   std::cout << recovered << std::endl;


    return 0;
}