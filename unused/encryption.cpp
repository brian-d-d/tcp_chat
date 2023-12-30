// Sample.cpp
//

#include <cryptopp/rsa.h>
using CryptoPP::RSA;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;

#include <cryptopp/sha.h>
using CryptoPP::SHA1;

#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/secblock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
using CryptoPP::DecodingResult;

#include <string>
using std::string;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <assert.h>

template <typename Key>
const Key loadKey(const std::string& filename)
{
  Key key;
  CryptoPP::ByteQueue queue;
  CryptoPP::FileSource file(filename.c_str(), true);
  file.TransferTo(queue);
  queue.MessageEnd();

  key.Load(queue);
  return key;
}

using namespace CryptoPP;

int main(int argc, char* argv[]) {
    std::string privateKeyName = "priv.pem";
    std::string publicKeyName = "pub.pub";
    std::string plainText = "hello_testing";

    AutoSeededRandomPool prng;

  auto privateKey = loadKey<CryptoPP::RSA::PrivateKey>(privateKeyName);
  auto publicKey = loadKey<CryptoPP::RSA::PublicKey>(publicKeyName);

  std::string encrypted, decrypted;
  RSAES_OAEP_SHA_Encryptor e(publicKey);

  StringSource(plainText, true,
	       new PK_EncryptorFilter(prng, e,
				      new StringSink(encrypted)));

  RSAES_OAEP_SHA_Decryptor d(privateKey);

  StringSource(encrypted, true,
	       new PK_DecryptorFilter(prng, d,
				      new StringSink(decrypted)));
}