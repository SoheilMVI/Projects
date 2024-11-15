#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iomanip> // Include this for std::hex and std::setfill

// PBOX و SBOX ها
unsigned char P1_BOX(unsigned char byte) {
    // جابجایی بر اساس P1
    unsigned char permuted = 0;
    permuted |= ((byte >> 3) & 1) << 0;
    permuted |= ((byte >> 2) & 1) << 1;
    permuted |= ((byte >> 1) & 1) << 2;
    permuted |= ((byte >> 0) & 1) << 3;
    permuted |= ((byte >> 7) & 1) << 4;
    permuted |= ((byte >> 6) & 1) << 5;
    permuted |= ((byte >> 5) & 1) << 6;
    permuted |= ((byte >> 4) & 1) << 7;
    return permuted;
}

unsigned char P1_INV_BOX(unsigned char byte) {
    // جابجایی معکوس بر اساس P1
    unsigned char permuted = 0;
    permuted |= ((byte >> 0) & 1) << 3; // بیت 0 به بیت 3
    permuted |= ((byte >> 1) & 1) << 2; // بیت 1 به بیت 2
    permuted |= ((byte >> 2) & 1) << 1; // بیت 2 به بیت 1
    permuted |= ((byte >> 3) & 1) << 0; // بیت 3 به بیت 0
    permuted |= ((byte >> 4) & 1) << 7; // بیت 4 به بیت 7
    permuted |= ((byte >> 5) & 1) << 6; // بیت 5 به بیت 6
    permuted |= ((byte >> 6) & 1) << 5; // بیت 6 به بیت 5
    permuted |= ((byte >> 7) & 1) << 4; // بیت 7 به بیت 4
    return permuted;
}

unsigned char S1_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b10; // 00 -> 10
        case 0b01: return 0b01; // 01 -> 01
        case 0b10: return 0b11; // 10 -> 11
        case 0b11: return 0b00; // 11 -> 00
        default: return byte;    // پیش‌فرض
    }
}

unsigned char S1_INV_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b11; // 00 -> 11
        case 0b01: return 0b01; // 01 -> 01
        case 0b10: return 0b00; // 10 -> 00
        case 0b11: return 0b10; // 11 -> 10
        default: return byte;    // پیش‌فرض
    }
}

unsigned char S2_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b01; // 00 -> 01
        case 0b01: return 0b10; // 01 -> 10
        case 0b10: return 0b00; // 10 -> 00
        case 0b11: return 0b11; // 11 -> 11
        default: return byte;    // پیش‌فرض
    }
}

unsigned char S2_INV_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b10; // 00 -> 10
        case 0b01: return 0b00; // 01 -> 00
        case 0b10: return 0b01; // 10 -> 01
        case 0b11: return 0b11; // 11 -> 11
        default: return byte;    // پیش‌فرض
    }
}

unsigned char S3_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b11; // 00 -> 11
        case 0b01: return 0b00; // 01 -> 00
        case 0b10: return 0b10; // 10 -> 10
        case 0b11: return 0b01; // 11 -> 01
        default: return byte;    // پیش‌فرض
    }
}

unsigned char S3_INV_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b01; // بازگشت از (00 -> (01)
        case 0b01: return 0b11; // بازگشت از (01 -> (11)
        case 0b10: return 0b10; // بازگشت از (10 -> (10)
        case 0b11: return 0b00; // بازگشت از (11 -> (00)
        default: return byte;     // پیش‌فرض
    }
}

unsigned char S4_BOX(unsigned char byte) {
    switch (byte & 0b11) {
        case 0b00: return 0b00; // بازگشت به خودش (00)
        case 0b01: return 0b11; // بازگشت از (01 -> (11)
        case 0b10: return 0b01; // بازگشت از (10 -> (01)
        case 0b11: return 0b10; // بازگشت از (11 -> (10)
        default: return byte;     // پیش‌فرض
    }
}

unsigned char S4_INV_BOX(unsigned char byte) {
    switch (byte & 0b11) { 
      case 0b00: return 0b00; 
      case 0b01: return 0b10; 
      case 0b10: return 0b11; 
      case 0b11: return 0b01; 
      default:
          return byte;
   }
}

// تابع برای رمزگذاری فایل
std::string encrypt(const std::string& inputText, const char *key) {
    std::string codedText = "";
    for (size_t i = 0; i < inputText.size(); ++i) {
        int length = strlen(key); // طول رشته
        unsigned char currentChar = inputText[i];

        for (int i = 0; i < length; ++i) {
            char steps = key[i]; // خواندن حرف فعلی
            unsigned char pboxOutput[4];
if (steps == 'p') {
                
                currentChar = P1_BOX(currentChar);

            } else if (steps == 's') {
                
                unsigned char nibble1 = (currentChar & 0xC0) >> 6; // دو بیت اول
                unsigned char nibble2 = (currentChar & 0x30) >> 4; // دو بیت دوم
                unsigned char nibble3 = (currentChar & 0x0C) >> 2; // دو بیت سوم
                unsigned char nibble4 = (currentChar & 0x03);      // دو بیت چهارم

                // اعمال SBOX ها به ترتیب
                nibble1 = S1_BOX(nibble1);
                nibble2 = S2_BOX(nibble2);
                nibble3 = S3_BOX(nibble3);
                nibble4 = S4_BOX(nibble4);

                currentChar = (nibble1 << 6) | (nibble2 << 4) | (nibble3 << 2) | nibble4;
            }
        }
        codedText += static_cast<char>(currentChar);
    }
    return codedText;
}

// تابع برای رمزگشایی فایل
std::string decrypt(const std::string& inputText, const char *key) {
    std::string decodedText = "";
    for (size_t i = 0; i < inputText.size(); ++i) {
        int length = strlen(key); // طول رشته
        unsigned char currentChar = inputText[i];

        for (int i = 0; i < length; ++i) {
            char steps = key[i]; // خواندن حرف فعلی
            unsigned char pboxOutput[4];

            if (steps == 'p') {
                
                currentChar = P1_INV_BOX(currentChar);

            } else if (steps == 's') {
                
                unsigned char nibble1 = (currentChar & 0xC0) >> 6; // دو بیت اول
                unsigned char nibble2 = (currentChar & 0x30) >> 4; // دو بیت دوم
                unsigned char nibble3 = (currentChar & 0x0C) >> 2; // دو بیت سوم
                unsigned char nibble4 = (currentChar & 0x03);      // دو بیت چهارم

                // اعمال SBOX ها به ترتیب
                nibble1 = S1_INV_BOX(nibble1);
                nibble2 = S2_INV_BOX(nibble2);
                nibble3 = S3_INV_BOX(nibble3);
                nibble4 = S4_INV_BOX(nibble4);

                currentChar = (nibble1 << 6) | (nibble2 << 4) | (nibble3 << 2) | nibble4;
            }
        }

        decodedText += static_cast<char>(currentChar);
    }
    return decodedText;
}

void testPbox(){
    std::string str = "Hello, world!";
    const char* byteArray = str.c_str();
    for (size_t i = 0; i < str.size(); ++i) {
        unsigned char byte = P1_BOX(byteArray[i]);
        unsigned char byte_r = P1_INV_BOX(byte);
        std::cout << byte << "\t";
        std::cout << byte_r;
        std::cout << "\t Byte " << i << ": " << byteArray[i] << " (" << static_cast<int>(byteArray[i]) << ")\n";
    }
}

void testSbox(){
    std::string str = "Hello, world!";
    const char* byteArray = str.c_str();
    for (size_t i = 0; i < str.size(); ++i) {
        unsigned char eachChar = byteArray[i];

        unsigned char nibble1 = (eachChar & 0xC0) >> 6; // دو بیت اول
        unsigned char nibble2 = (eachChar & 0x30) >> 4; // دو بیت دوم
        unsigned char nibble3 = (eachChar & 0x0C) >> 2; // دو بیت سوم
        unsigned char nibble4 = (eachChar & 0x03);      // دو بیت چهارم

        std::cout << "Pure:\n";
        std::cout << std::hex << static_cast<int>(nibble1) << "\t";
        std::cout << std::hex << static_cast<int>(nibble2) << "\t";
        std::cout << std::hex << static_cast<int>(nibble3) << "\t";
        std::cout << std::hex << static_cast<int>(nibble4) << std::endl;

        // اعمال SBOX ها به ترتیب
        nibble1 = S1_BOX(nibble1);
        nibble2 = S2_BOX(nibble2);
        nibble3 = S3_BOX(nibble3);
        nibble4 = S4_BOX(nibble4);

        nibble1 = S1_BOX(nibble1);
        nibble2 = S2_BOX(nibble2);
        nibble3 = S3_BOX(nibble3);
        nibble4 = S4_BOX(nibble4);

        nibble1 = S1_BOX(nibble1);
        nibble2 = S2_BOX(nibble2);
        nibble3 = S3_BOX(nibble3);
        nibble4 = S4_BOX(nibble4);
std::cout << "Code:\n";
        std::cout << std::hex << static_cast<int>(nibble1) << "\t";
        std::cout << std::hex << static_cast<int>(nibble2) << "\t";
        std::cout << std::hex << static_cast<int>(nibble3) << "\t";
        std::cout << std::hex << static_cast<int>(nibble4) << std::endl;

        nibble1 = S1_INV_BOX(nibble1);
        nibble2 = S2_INV_BOX(nibble2);
        nibble3 = S3_INV_BOX(nibble3);
        nibble4 = S4_INV_BOX(nibble4);

        nibble1 = S1_INV_BOX(nibble1);
        nibble2 = S2_INV_BOX(nibble2);
        nibble3 = S3_INV_BOX(nibble3);
        nibble4 = S4_INV_BOX(nibble4);

        nibble1 = S1_INV_BOX(nibble1);
        nibble2 = S2_INV_BOX(nibble2);
        nibble3 = S3_INV_BOX(nibble3);
        nibble4 = S4_INV_BOX(nibble4);

        std::cout << "Decode:\n";
        std::cout << std::hex << static_cast<int>(nibble1) << "\t";
        std::cout << std::hex << static_cast<int>(nibble2) << "\t";
        std::cout << std::hex << static_cast<int>(nibble3) << "\t";
        std::cout << std::hex << static_cast<int>(nibble4) << std::endl;

        unsigned char mergedByte = (nibble1 << 6) | (nibble2 << 4) | (nibble3 << 2) | nibble4;
        std::cout << mergedByte;
        std::cout << "\t Byte " << i << ": " << byteArray[i] << " (" << static_cast<int>(byteArray[i]) << ")\n";
    }
}

int main() {
    const char *key = "pspspspsp";

    std::string inputText = "Hello, world!. This text is for test :)";
    std::string codedText = encrypt(inputText , key);
    std::string decodedText = decrypt(codedText , key);

    std::cout << codedText << "\n";
    std::cout << decodedText << "\n";

    // testPbox();
    // testSbox();

    return 0;
}