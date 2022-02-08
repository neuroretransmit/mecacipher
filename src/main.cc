#include <chrono>
#include <iomanip>
#include <iostream>

#include "mecacipher.h"
#include "util.h"

using namespace std;
using namespace chrono;

template<typename T> ostream& operator<<(ostream& output, vector<T> const& values)
{
    for (auto const& value : values) {
        output << setfill('0') << setw(16) << hex << value << " ";
    }
    return output;
}

int main()
{
    MECACipher<uint64_t, 15> cipher;
    string key = "My crime is that of curiosity";

    vector<uint64_t> plaintext{0x1337, 0x1337, 0x1337, 0x1337};
    vector<uint64_t> key_words = key_to_words<uint64_t>(key);
    vector<uint64_t> key_schedule = cipher.schedule_keys(key_words);

    // Encrypt
    auto enc_start = high_resolution_clock::now();
    vector<uint64_t> encrypted = cipher.encrypt(plaintext, key_schedule);
    auto enc_stop = high_resolution_clock::now();
    auto enc_duration = duration_cast<microseconds>(enc_stop - enc_start);

    // Decrypt
    auto dec_start = high_resolution_clock::now();
    vector<uint64_t> decrypted = cipher.decrypt(encrypted, key_schedule);
    auto dec_stop = high_resolution_clock::now();
    auto dec_duration = duration_cast<microseconds>(dec_stop - dec_start);

    cout << "plaintext: " << plaintext << endl;
    cout << "key      : " << key_words << endl;
    cout << "encrypted: " << encrypted << endl;
    cout << "decrypted: " << decrypted << endl;

    cout.unsetf(ios::hex);
    cout << "enc time : " << enc_duration.count() << " microseconds" << endl;
    cout << "dec time : " << dec_duration.count() << " microseconds" << endl;
}
