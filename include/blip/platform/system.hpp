#include <string>

namespace platform {
std::string getTTFPath(std::string &family, std::string &style);
void readFile(const char *filename, std::string &lines);
}
