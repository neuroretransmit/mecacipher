#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chrono>

#include "../src/mecacipher.h"
#include "../src/util.h"

using namespace chrono;

using WordSizes = ::testing::Types<uint16_t, uint32_t, uint64_t>;

template<typename T> ostream& operator<<(ostream& output, vector<T> const& values)
{
    for (auto const& value : values)
        output << setfill('0') << setw(numeric_limits<T>::digits / 4) << hex << value << " ";
    return output;
}

/* Fixtures */
template <typename T>
class SmokeTest : public ::testing::Test {
 public:
  static T shared_;
  T value_;
};

/* Suites */
TYPED_TEST_SUITE(SmokeTest, WordSizes, );

/* Parameterized test for different word sizes */
TYPED_TEST(SmokeTest, EncryptDecrypt)
{
    MECACipher<TypeParam, 15> cipher;
    string key = "My crime is that of curiosity";

    vector<TypeParam> plaintext{0x1337, 0x1337, 0x1337, 0x1337};
    vector<TypeParam> key_words = key_to_words<TypeParam>(key);
    vector<TypeParam> key_schedule = cipher.schedule_keys(key_words);

    // Encrypt
    auto enc_start = high_resolution_clock::now();
    vector<TypeParam> encrypted = cipher.encrypt(plaintext, key_schedule);
    auto enc_stop = high_resolution_clock::now();
    auto enc_duration = duration_cast<microseconds>(enc_stop - enc_start);

    // Decrypt
    auto dec_start = high_resolution_clock::now();
    vector<TypeParam> decrypted = cipher.decrypt(encrypted, key_schedule);
    auto dec_stop = high_resolution_clock::now();
    auto dec_duration = duration_cast<microseconds>(dec_stop - dec_start);

    cout << "PLAINTEXT: " << plaintext << endl;
    cout << "KEY      : " << key_words << endl;
    cout << "ENCRYPTED: " << encrypted << endl;
    cout << "DECRYPTED: " << decrypted << endl;

    cout.unsetf(ios::hex);
    cout << "ENC TIME : " << enc_duration.count() << " microseconds" << endl;
    cout << "DEC TIME : " << dec_duration.count() << " microseconds" << endl;
    ASSERT_EQ(plaintext, decrypted);
}
