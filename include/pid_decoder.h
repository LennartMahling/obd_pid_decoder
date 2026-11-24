#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>


namespace obd::decoder
{
    struct ParsedResponse
    {
        std::string mode;
        std::string pid;
        std::vector<std::string> dataBytes;
    };

    //Funktionen
    std::optional<ParsedResponse> parse(std::string rawResponse);
    std::optional<uint8_t> hexToDec(std::string hexByte);
    std::optional<double> decodeRPM(std::string rawResponse);
}
