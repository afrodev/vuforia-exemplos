//
//  NFreeImage.h
//  Created by DrNeurosurg on 30.01.13.
//
//


#ifndef _NFreeImage_h
#define _NFreeImage_h

#include "FreeImage.h"
#define BYTE unsigned char
#define MAX_SIZE 2048

// ############### Class for loading an image file from file-system (FreeImage) #######################################
// see: http://freeimage.sourceforge.net
//#####################################################################################################################
class NFreeImage
{
public:
    // Initialise image pointer.
    NFreeImage()
    : m_pImage( 0 )
    {
    }
    //HELPER FUNCTIONS FOR "NPOT"
    bool isPowerOfTwo(int value)
    {
        return value ? ((value & -value) == value) : false;
    }
    
    bool isSizeValid(int value)
    {
        
        return value >= 1 && value <= MAX_SIZE;
    }
    
    int bestPowerOfTwo(int value)
    {
        unsigned int p =1;
        while (p < value)
            p <<= 1;
        
        if (p > MAX_SIZE) p = MAX_SIZE;
        return p;
    }
    
    // Load a new image file.
    bool Load( const std::string & i_rFilename )
    {
        // Unload first.
        Unload();
        
        // Check the file signature and deduce its format.
        FREE_IMAGE_FORMAT FreeImageFormat = FreeImage_GetFileType( i_rFilename.c_str(), 0 );
        
        
        // If still unknown, try to guess the file format from the file extension.
        if( FreeImageFormat == FIF_UNKNOWN )
        {
            FreeImageFormat = FreeImage_GetFIFFromFilename( i_rFilename.c_str() );
        }
        
        // If still unkown, return failure.
        if( FreeImageFormat == FIF_UNKNOWN )
        {
            return false;
        }
        
        // Check that the plugin has reading capabilities and load the file.
        if( FreeImage_FIFSupportsReading( FreeImageFormat ) )
        {
            FIBITMAP *tmp = FreeImage_Load( FreeImageFormat, i_rFilename.c_str() );
            m_pImage = FreeImage_ConvertTo32Bits(tmp);
            FreeImage_Unload(tmp);
            
        }
        
        if(m_pImage != 0)
        {
            
            //CHANGE CHANNEL ORDER - SWAP R<->B (IOS ONLY ??)
            FIBITMAP * r = FreeImage_GetChannel(m_pImage, FICC_RED);
            FIBITMAP * b = FreeImage_GetChannel(m_pImage, FICC_BLUE);
            
            FreeImage_SetChannel(m_pImage, b, FICC_RED);
            FreeImage_SetChannel(m_pImage, r, FICC_BLUE);
            FreeImage_Unload(r);
            FreeImage_Unload(b);
            
            
            //RESIZE IF NECESSARY
            int w = FreeImage_GetWidth(m_pImage);
            int h = FreeImage_GetHeight(m_pImage);
            
            if(isSizeValid(w) && !isPowerOfTwo(w))
                w = bestPowerOfTwo(w);
            if(isSizeValid(h) && !isPowerOfTwo(h))
                h = bestPowerOfTwo(h);
            
            m_pImage = FreeImage_Rescale(m_pImage, w, h, FILTER_BICUBIC);
            
        }
        
        // Return success.
        return m_pImage != 0;
    }
    
    // Get loaded image width.
    unsigned int Width() const
    {
        return m_pImage ? FreeImage_GetWidth( m_pImage ) : 0;
    }
    
    // Height.
    unsigned int Height() const
    {
        return m_pImage ? FreeImage_GetHeight( m_pImage ) : 0;
    }
    
    // Bits per pixel.
    unsigned int BitsPerPixel() const
    {
        return m_pImage ? FreeImage_GetBPP( m_pImage ) : 0;
    }
    
    // Image bits.
    const BYTE * Bits() const
    {
        return m_pImage ? FreeImage_GetBits( m_pImage ) : 0;
    }
    
    // Unload image.
    void Unload()
    {
        if( m_pImage )
        {
            FreeImage_Unload( m_pImage );
            m_pImage = 0;
        }
    }
    
    // Cleanup.
    ~NFreeImage()
    {
        Unload();
    }
private:
    FIBITMAP * m_pImage;
};
//##########################################################################################################



#endif
