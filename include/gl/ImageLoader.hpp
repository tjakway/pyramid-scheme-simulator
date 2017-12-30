#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <lodepng.h>

#include "Util/Strcat.hpp"
#include "Util/NewExceptionType.hpp"

/**
 * thin layer over lodepng that throws exceptions on error
 */
class ImageLoader
{
protected:
    ImageLoader() = delete;

public:
    NEW_EXCEPTION_TYPE(ImageLoaderException);
    NEW_EXCEPTION_TYPE_WITH_BASE(LoadFileException, ImageLoaderException);
    NEW_EXCEPTION_TYPE_WITH_BASE(DecodeException, ImageLoaderException);

    /**
     * holds raw image data and dimensions in RGBA format (4 bytes per pixel)
     */
    class Image
    {
        friend ImageLoader;

    public:
        const unsigned int width, height;
        const std::vector<unsigned char> pixels;

    protected:
        Image(const unsigned int _width, 
                const unsigned int _height, 
                const std::vector<unsigned char> _pixels)
            : width(_width), height(_height), pixels(_pixels)
        {}

    public:
        Image(const Image& other)
            : width(other.width),
            height(other.height),
            pixels(other.pixels)
        {}

        virtual ~Image() {}
    };


    static Image loadPNG(const char* filename)
    {
        //closely follows example 2
        std::vector<unsigned char> pngData;

        //load the PNG
        unsigned error = lodepng::load_file(pngData, filename);
        if(error)
        {
            throw LoadFileException(STRCAT("Error while loading png data from ",
                        filename, ": ", lodepng_error_text(error)));
        }
        else
        {
            //decode it into an RGBA vector
            unsigned width, height;
            std::vector<unsigned char> rawPixels;

            const std::string errorHeader = 
                STRCAT("Error while decoding png data from ", filename, ": ");

            error = lodepng::decode(rawPixels, width, height, pngData);
            if(error)
            {
                throw DecodeException(STRCAT(errorHeader, lodepng_error_text(error)));
            }
            else if(width <= 0 || height <= 0)
            {
                throw DecodeException(STRCAT(errorHeader, "nonsensical dimensions ",
                            "(width: ", width, "height: ", height));
            }
            else
            {
                return Image(width, height, rawPixels);
            }
        }
    }

    static Image loadPNG(const std::string& filename)
    {
        return loadPNG(filename.c_str());
    }
};
