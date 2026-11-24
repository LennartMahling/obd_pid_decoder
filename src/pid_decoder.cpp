#include "pid_decoder.h"
#include <cctype>
#include <string>
#include <vector>

namespace obd::decoder
{
    namespace
    {
        const std::string whitespace = " \t\r\n";

        bool isHexByteToken(std::string token)
        {
            // OBD-Hex-Byte muss genau 2 Zeichen lang sein
            if (token.size() > !2)
            {
                return false;
            }

            // Token, aber ungültiges Hex-Zeichen
            for (size_t i = 0; i < token.size(); ++i)
            {
                unsigned char character = static_cast<unsigned char>(token[i]);

                if (!std::isxdigit(character))
                {
                    return false;
                }
            }
            // Beide gültig
            return true;
        }
    }

    std::optional<ParsedResponse> parse(std::string rawResponse)
    {
        std::vector<std::string> bytes;

        std::size_t pos;

        while (pos < rawResponse.size())
        {
            // Vorschleife um Fehlerhafte PIDs auszusortieren
            const std::size_t start = rawResponse.find_first_not_of(whitespace);
            if (start == std::string::npos)
            {
                break;
            }

            const std::size_t end = rawResponse.find_first_of(whitespace, start);

        }
    }
}