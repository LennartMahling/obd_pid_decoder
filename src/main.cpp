#include "pid_decoder.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::vector<std::string> testCases = {
        "41 0C 1A F8",          // Standard RPM-Antwort
        "  41\t0c\r\n1a f8  ",   // Whitespace, Kleinbuchstaben, \r\n
        "41 0D 3C",             // 1 Datenbyte (z. B. Geschwindigkeit)
        "41 0C",                // Zu kurz (fehlendes Datenbyte)
        "41 0C 1G F8",          // Ungueltiges Hex-Zeichen ('G')
        "41 0C 1A F80",         // Token mit 3 Zeichen
        "NO DATA",              // ELM327-Fehlermeldung
        ""                      // Leerstring
    };

    for (const std::string& raw : testCases) {
        std::cout << "Input: \"" << raw << "\" -> ";

        const auto parsed = obd::decoder::parse(raw);
        if (!parsed.has_value()) {
            std::cout << "std::nullopt (ungueltig)\n";
            continue;
        }

        std::cout << "OK | mode=" << parsed->mode
                  << ", pid=" << parsed->pid
                  << ", bytes=[";

        for (std::size_t i = 0; i < parsed->dataBytes.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << parsed->dataBytes[i];
        }
        std::cout << "]\n";
    }

    return 0;
}
