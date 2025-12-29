#include "stb_image.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    char do_flip = *argv[5];
    stbi_set_flip_vertically_on_load(do_flip == 'y');
    int width, height, nrChannels;
    const char* varname = argv[1];
    const char* input   = argv[2];
    std::string output;
    output.assign(argv[3], std::strlen(argv[3]));
    int channels = std::atoi(argv[4]);
    unsigned char *data = stbi_load(input, &width, &height, &nrChannels, channels);

    if (!data) {
            std::cout
    << "unable to load image: "
    << stbi_failure_reason() 
    << "\n";
    //throw;
        return 1;
    }

    std::cout << input << "\n";
    std::cout << output << "\n";
    std::cout << "'" << data << "'" << "\n";
    std::cout << width << "\n";
    std::cout << height << "\n";
    std::cout << nrChannels << "\n";
    std::cout << channels << "\n";

    int amountOfBytes = width * height * 4;
    unsigned char* data_end = &data[amountOfBytes];

    std::stringstream sb;
    char tmp[4];
    for (unsigned char* ptr = data; ptr < data_end; ptr++) {
        snprintf(tmp, 3, "%0.2x", *ptr);
        sb << "\\x" << tmp;
    }
    
    std::string headerpath = output + ".hpp";
    std::string cppath = output + ".cpp";
    // std::ofstream header(headerpath);
    // header << \
    // "#include \"Texture2d.hpp\"\n"\
    // "extern RR::image_data " << varname << ";\0";

    std::FILE* header = std::fopen(headerpath.c_str(), "w");
    std::fwrite(header,
        "#include \"texture2d.hpp\"\n"\
        "extern RR::image_data %s;\0",
        varname
    );
    std::fclose(header);

    // std::ofstream cfile(cpath);
    // cfile << \
    // "#include \"" << headerpath << "\"\n" << \
    // "RR::image_data " << varname << " = {"\
    // << width << ", " << height << ", " << nrChannels << ", "\
    // << "(unsigned char*)\"" << sb.str() << "\"};\n\0";

    std::FILE* cppfile = std::fopen(cppath.c_str(), "w");
    std::fwrite(cppfile,
        "#include \"texture2d.hpp\"\n"\
        "extern RR::image_data %s;\0",
        varname
    );
    std::fclose(cppfile);
	stbi_image_free(data);
}
