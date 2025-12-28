#include "stb_image.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    const char* varname = argv[1];
    const char* input   = argv[2];
    std::string output;
    output.assign(argv[3], std::strlen(argv[3]));
    int channels = std::atoi(argv[4]);
    unsigned char *data = stbi_load(input, &width, &height, &nrChannels, channels);

    int amountOfBytes = std::strlen((const char*)data);
    unsigned char* data_end = data + amountOfBytes;

    std::stringstream sb;
    char tmp[4];
    for (unsigned char* ptr = data; ptr < data_end; ptr++) {
        snprintf(tmp, 3, "%0.2x", *ptr);
        sb << "0x" << tmp << ",";
    }
    
    std::string headerpath = output + ".hpp";
    std::string cpath = output + ".cpp";
    std::ofstream header(headerpath);
    header << \
    "#include \"Texture2d.hpp\\n"\
    "extern image_data " << varname << ";\0"

    std::ofstream cfile(cpath);
    cfile << \
    "#include \"" << headerpath << "\"" << \
    "image_data " << varname << " = {"\
    << width << ", " << height << ", " << nrChannels << ", "\
    << "{" << sb.str() << "}};\n\0";
	stbi_image_free(data);
}
