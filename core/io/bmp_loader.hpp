#pragma once

#include <list>
#include <memory>

#include <filesystem.h>

#include "bmp_format.hpp"

class BMP {
    public:
        BMP() = default;

    using File_ptr = std::shared_ptr<File>;
    const uint8_t* Load(const char* path) {
        try {
            File_ptr bmp_file = Filesystem::Open<File_ptr>(path);

            bmp_header_t* header = new bmp_header_t();
            bmp_file->seekg(BMP_HEADER_OFFSET);
            bmp_file->read((uint8_t*) header, BMP_HEADER_SIZE);

            info_header_t* info_header = new info_header_t();
            bmp_file->seekg(BMP_INFOHEADER_OFFSET);
            bmp_file->read((uint8_t*) info_header, BMP_INFOHEADER_SIZE);

            color_table_t* color_table = new color_table_t();
            delete color_table; //don't care about this for now

            mData = new uint8_t[info_header->imagesize];

            delete header;
        } catch(...) {
            std::__throw_runtime_error("Runtime error while loading bmp file.");
        }
    }

    const uint8_t* Data() {
        if(mData != nullptr) {
            return mData;
        }
        else {
            std::__throw_runtime_error("Data member in BMP image object is null");
            return mData;
        }
    }

    bool Destroy() {
        try {
            delete[] mData;
            mData = nullptr;
            return true;
        } catch(...) {
            throw;
        }
        return false;
    }

private:
    uint8_t *mData = nullptr;
};