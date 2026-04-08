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

// WRITE A FUNCTION TO WALK THE FILE TREES TO FIND ALL THE NECESSARY FILES
std::string getTTFPath(const std::string &family, const std::string &style) {
    if (!FcInit()) {
        std::cerr << "Could not init fontconfig" << std::endl;
    }

    FcPattern *pat = FcNameParse((const FcChar8 *)(family + ":" + style).c_str());
    FcConfigSubstitute(NULL, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern *font = FcFontMatch(NULL, pat, &result);

    if (font) {
        FcChar8 *file = NULL;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch) {
            std::cout << "Font File: " << file << std::endl;
            return std::string((const char *)file);
        } else {
            // TODO: Display this error and default to a font
            std::cerr << "Could not get FC_FILE" << std::endl;
        }
        FcPatternDestroy(font);
    } else {
        // TODO: Display this error and default to a font
        std::cerr << "No match found" << std::endl;
        return std::string("");
    }

    FcPatternDestroy(pat);
    FcFini();
    return std::string("");
}
}
