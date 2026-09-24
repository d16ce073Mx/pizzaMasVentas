#include "config/DotEnv.h"

#include <cstdlib>
#include <fstream>
#include <string>

namespace pizzaMas
{
    void loadDotEnv(const char* filename)
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            return;
        }

        std::string line;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            const auto separator = line.find('=');

            if (separator == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, separator);
            std::string value = line.substr(separator + 1);

            if (!key.empty())
            {
                setenv(key.c_str(), value.c_str(), 1);
            }
        }
    }
}