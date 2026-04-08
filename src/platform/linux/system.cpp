#include <blip/platform/system.hpp>
#include <fontconfig/fontconfig.h>
#include <fstream>
#include <iostream>

namespace platform {
void readFile(const char *fname, std::string &lines) {
    std::string filename{fname};
    std::fstream f{filename};

    if (!f.is_open()) {
        std::cout << "Could not open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line = "";
    while (std::getline(f, line)) {
        lines += line + "\n";
    }
}

std::string getTTFPath(const std::string &family, const std::string &style) {
    static bool initialized = false;
    if (!initialized) {
        if (!FcInit()) {
            std::cerr << "Could not init fontconfig\n";
            return "";
        }
        initialized = true;
    }

    std::string patternStr = family;
    if (!style.empty())
        patternStr += ":" + style;

    FcPattern *pat = FcNameParse((const FcChar8 *)patternStr.c_str());

    // Add fallback monospace family
    FcPatternAddString(pat, FC_FAMILY, (const FcChar8 *)"monospace");

    FcConfigSubstitute(nullptr, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern *font = FcFontMatch(nullptr, pat, &result);

    std::string path;

    if (font) {
        FcChar8 *file = nullptr;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch) {
            path = reinterpret_cast<char *>(file);
        } else {
            std::cerr << "Could not get FC_FILE\n";
        }
        FcPatternDestroy(font);
    } else {
        std::cerr << "No match found — this should never happen with fallback\n";
    }

    FcPatternDestroy(pat);

    if (path.empty()) {
        FcPattern *fallbackPat = FcNameParse((const FcChar8 *)"monospace");
        FcConfigSubstitute(nullptr, fallbackPat, FcMatchPattern);
        FcDefaultSubstitute(fallbackPat);

        FcPattern *fallbackFont = FcFontMatch(nullptr, fallbackPat, &result);
        if (fallbackFont) {
            FcChar8 *file = nullptr;
            if (FcPatternGetString(fallbackFont, FC_FILE, 0, &file) == FcResultMatch) {
                path = reinterpret_cast<char *>(file);
            }
            FcPatternDestroy(fallbackFont);
        }
        FcPatternDestroy(fallbackPat);
    }

    return path;
}
}
