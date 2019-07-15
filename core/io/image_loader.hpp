#pragma once

#include <string>

enum file_format{
    BMP = 0,
    JPG,
    TGA,
    PNG,
    DDS
};

class ImageLoader {
    public:
        ImageLoader() = default;

        void Load(std::string);

        void LoadFromMemory(uint8_t*);

        uint8_t* Data();

        void Destroy();

    private:
        uint8_t* mData;
        std::string mName;
        file_format mFormat;
    protected:
};