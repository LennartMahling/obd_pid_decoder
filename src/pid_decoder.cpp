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
            if (token.size() != 2)
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
        std::vector<std::string> tokens;
        std::size_t pos = 0;

        // Solange ein Zeichen noch im String enthalten ist, wird nach dem nächsten Token gesucht
        while (pos < rawResponse.size())
        {
            // Sucht nach dem ersten Zeichen, welches nicht 'whitespace' ist
            const std::size_t start = rawResponse.find_first_not_of(whitespace, pos);
            // Endet, wenn String leer ist
            if (start == std::string::npos)
            {
                break;
            }

            // Sucht ab 'start' nach dem ersten 'whitespace' -> Also bis zum Ende des Tokens
            const std::size_t end = rawResponse.find_first_of(whitespace, start);

            std::string token;
            // Prueft, ob am Ende angekommen
            if (end == std::string::npos)
            {
                // Falls kein Leerzeichen mehr gefunden wird -> Alles bis zum Ende genommen
                token = rawResponse.substr(start);
            }
            else
            {
                // Falls ein Leezeichen gefunden wurde -> Genau die Zeichen zwischen 'start' und 'end' nehmen
                const std::size_t length = end - start;
                token = rawResponse.substr(start, length);
            }

            // Prüft ob das Token wie ein 2-stelliges HexByte aussieht
            // Falls nicht -> komplettes Parsen abbrechen
            if (!isHexByteToken(token))
            {
                return std::nullopt;
            }

            // Gültiges Token in die Liste 'token' einfügen
            tokens.emplace_back(token);

            // Endet, falls es das letzte Token bis zum String-Ende war
            if (end == std::string::npos)
            {
                break;
            }

            // Suchposition wird hinter das zuletzt gefundene 'whitespace' gesetzt
            pos = end + 1;
        }

        // Gültige OBD-II Antwort benötigt mindestens Mode, PID und 1 Datenbyte -> 3 Tokens
        if (tokens.size() < 3)
        {
            return std::nullopt;
        }

        // Rückgabewert und Tokens verschieben
        ParsedResponse result;
        result.mode = std::move(tokens[0]);
        result.pid = std::move(tokens[1]);
        // Alle restlichen Tokens ab Index 2 als Daten in dataBytes übernehmen
        result.dataBytes.assign(
            std::make_move_iterator(tokens.begin() + 2),
            std::make_move_iterator(tokens.end())
        );

        return result;
    }

    std::optional<uint8_t> hexToDec(std::string hexByte)
    {
        if (!isHexByteToken(hexByte))
        {
            return std::nullopt;
        }
        else
        {
            int decValue = 0;
            int base = 1; //Represents 16^0

            //Traverse the hexadecimal string from right to left
            for (int i = hexByte.length() - 1; i >= 0; i--)
            {
                int digit;

                //Covert characters '0' - '9' to values 0-9
                if (hexByte[i] >= '0' && hexByte[i] <= '9')
                {
                    digit = hexByte[i] - '0';
                }
                //Convert characters 'A' - 'F' to values 10-15
                else if (hexByte[i] >= 'A' && hexByte[i] <= 'F')
                {
                    digit = hexByte[i] - 'A' + 10;
                }
                //Ignore invalid hexadecimal characters
                else
                {
                    continue;
                }

                //Add digit x corresponding power of 16
                decValue += digit * base;

                //Move to the next power of 16
                base *= 16;
            }
            return decValue;
        }
    }
}