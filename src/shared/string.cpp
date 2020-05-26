//
// Created by lzx on 5/15/20.
//
#include <shared/string.h>
#include <cassert>
#include <vector>
#include <LiVM/LiVM.h>
#include <codecvt>
#include <locale>

namespace LiVM {
    namespace strings {
        static std::wstring_convert<std::codecvt_utf8<wchar_t>> CONVERT;
        static bool isOneByteType(const u1 *bytes, size_t length, int position) {
            assert(position + 1 <= length);
            return (bytes[position] & 0x80) == 0;
        }
        static bool isTwoBytesType(const u1 *bytes, size_t length, int position){
            assert(position + 2 <= length);
            return (bytes[position] & 0xE0) == 0xC0
                    && (bytes[position + 1] & 0xC0) == 0x80;
        }
        static bool isThreeBytesType(const u1 *bytes, size_t length, int position){
            assert(position + 3 <= length);
            return (bytes[position] & 0xF0) == 0xE0
                   && (bytes[position + 1] & 0xC0) == 0x80
                   && (bytes[position + 2] & 0xC0) == 0x80;
        }
        static bool isSixBytesType(const u1 *bytes, size_t length, int position){
            assert(position + 6 <= length);
            return (bytes[position] == 0xED)
                    && (bytes[position+1] & 0xF0) == 0xA0
                    && (bytes[position+2] & 0xC0) == 0x80
                    && (bytes[position + 3] == 0xED)
                    && (bytes[position + 4] & 0xF0) == 0xB0
                    && (bytes[position + 5] & 0xC0) == 0x80;
        }
        static u2 getOneByteType(const u1* bytes, int position){
            return bytes[position];
        }
        static u2 getTwoBytesType(const u1* bytes, int position){
            return static_cast<u2>(((bytes[position] & 0x1f) << 6)
                   + (bytes[position+1] & 0x3f));
        }
        static u2 getThreeBytesType(const u1* bytes, int position){
            return static_cast<u2>(((bytes[position] & 0xf) << 12)
                    + ((bytes[position+1] & 0x3f) << 6)
                    + (bytes[position+2] & 0x3f));
        }
        static u2 getSixBytesType(const u1* bytes, int position){
            return static_cast<u2>(0x10000
                   + ((bytes[position+1] & 0x0f) << 16)
                   + ((bytes[position+2] & 0x3f) << 10)
                   + ((bytes[position+4] & 0x0f) << 6)
                   + ((bytes[position+5] & 0x3f)));
        }
        String fromBytes(u1 *bytes, size_t length){
            std::vector<u2> buffer;
            for(int pos = 0; pos < length;) {
                if(isOneByteType(bytes, length, pos)){
                    buffer.push_back(getOneByteType(bytes, pos));
                    pos += 1;
                } else if (isTwoBytesType(bytes,length, pos)){
                    buffer.push_back(getTwoBytesType(bytes, pos));
                    pos += 2;
                } else if(isThreeBytesType(bytes, length, pos)){
                    buffer.push_back(getThreeBytesType(bytes, pos));
                    pos += 3;
                } else if(isSixBytesType(bytes, length, pos)){
                    buffer.push_back(getSixBytesType(bytes, pos));
                    pos += 6;
                } else {
                    PANIC("Unexpected bytes type");
                }
            }
            return String(buffer.begin(), buffer.end());
        }
        std::string toStdString(const String &str) {
            return CONVERT.to_bytes(str);
        }
        String fromStdString(const std::string &str) {
            return CONVERT.from_bytes(str);
        }
        String replaceAll(const String &string, const String &oldValue, const String &newValue) {
            auto oldSize =  oldValue.size();
            auto newSize =  newValue.size();
            String newString = string;
            auto pos = newString.find(oldValue);
            while(pos != std::string::npos){
                newString.replace(pos, oldSize, newValue);
                pos = newString.find(oldValue, pos + newSize);
            }
            return newString;
        }
        std::vector<String> split(const String &string, const String &delimiter) {
            size_t  start = 0;
            size_t  end;
            size_t delimiterLength =  delimiter.length();
            std::vector<String> results;

            while((end = string.find(delimiter, start)) != String::npos){
                results.push_back(string.substr(start, end - start));
                start = end + delimiterLength;
            }
            results.push_back(string.substr(start));
            return results;
        }
    }
}

