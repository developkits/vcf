#ifndef _VCF_IMAGE_H__
#define _VCF_IMAGE_H__
//Image.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


/* Generated by Together */



namespace VCF{


class ImageBits;

class GraphicsContext;


/**
\class Image Image.h "vcf/GraphicsKit/Image.h"
The Image class is an abstract class that defines how you can work with and 
manipulate an image.

An image provides access to information such as the pixel layout, the number 
of color channels in the image, the number of bits in each channel, and whether 
or not the channel data is floating point or integer based. You can get or set 
the height or width of an image. You can manipulate the bits of the image as well. 
Setting the size will destroy any existing data in the image.

Image bits are manipulated through the ImageBits class. Each image has an ImageBits 
instance - the ImageBits holds a pointer to the buffer of data for the image. 
Depending on the system the buffer may be allocated by the VCF or it may be 
allocated by the platform. In other words, *don't* reallocate or delete this 
memory - it's not your's to play with! Feel to modify the pixel values to your 
heart's content though. 

\code
Image* img = //... get the image from somewhere

SysPixelType* pixels = img->getImageBits()->pixels_;
//assume a 4 color channel image
int sz = img->getHeight() * img->getWidth();
for (int i=0;i<sz;i++ ) {
	pixels[i].b = //...determine some cool blue value
	pixels[i].g = //...determine some cool green value
	pixels[i].r = //...determine some cool red value
	pixels[i].a = //...determine some cool alpha value
}
\endcode


Each platform defines a specific type of SysPixelType. For example on Win32 
it's defined as BGRAPixel<unsigned char>, which means that each pixel type 
is laid out in blue, green, red, alpha order, and each unit (of b,g,r,a) is 
an unsigned char (8 bits). By default, the ImageBits data uses this as it unit 
of storage in it's buffer.


To support AGG integration, the ImageBits has a renderBuffer_ member. You can 
initialize this like so :
\code
img->getImageBits()->attachRenderBuffer( img->getWidth(), img->getHeight() );
\endcode


Once initialized, you can use the render buffer in AGG operations 
(see <a href="http://www.antigrain.com/doc/index.html">http://www.antigrain.com/doc/index.html</a>
 for more on AGG).


Unlike many of the other classes in the VCF, the Image class does not use a 
peer. Instead a concrete platform implementation is provided for each platform 
the VCF is ported to. So what you have is (on Win32, for example):
\verbatim

 Image
  |
  +--- AbstractImage
               |
               +--- Win32Image
\endverbatim

Thus any instance of an Image that you deal with on the Win32 platform is 
ultimately a Win32Image instance. The AbstractImage is used to implement the 
basic housekeeping methods that are almost certainly guaranteed to be the 
same no matter the platform. The rest are then implemented in the concrete 
platform class.

Part of the idea behind the ImageBits class is to support a number of 
different image types. The idea being that we would have 4 channel color images, 
or 1 channel grey scale images, with potentially different channel sizes and 
pixel layout order. This is currently accomplished through the use of templates.

\note
Currently while we have the potential to support grey scale images, we do not in 
practice actually do so yet. Nor do we fully support alpha blending - we do have 
the data there, we just don't make any use of it yet.

We do support a *sort* of transparency. The image has ability to set a transparency 
color, and then toggle whether or not this should be used (see 
Image::setTransparencyColor() and Image::setIsTransparent() ) .

Drawing images is done by creating an image and then calling the various image 
drawing methods on the GraphicsContext.

Drawing an image (on Win32) means we take a couple of things into consideration:
\li 1) do we have a current transform in the GraphicsContext that is not an identity 
set? An identity set means that nothing needs to be transformed.

\li 2) is the image transparent (Image::isTransparent() returns true)?

If the current transform is *not* "default" i.e. it's an identity set, then we do 
the more complex code that uses AGG to transform the image and then blend it with 
the underlying bits that belong to the graphics context HDC.

If the transform is default then we create a sub image and transfer whatever pixels 
we need that are within our image bounds rect. Then if the image is transparent 
we execute some insane code that uses a 1 bit mask to clear out, or render invisible 
(using extra HBITMAP's and ROP code magic) and pixels in the image that correspond 
to the image's transparent color. So if an image has it's transparent color set to 
"green" then any green pixels in the image will become invisible and the underlying 
pixels of the graphics context will show through.

*/
class GRAPHICSKIT_API Image {
public:

	/**
	This indicates the type of image. An image may be either
	Color (this is the default) or Grayscale. Color images
	have four channels, grayscale images have 1. The precise
	value of the enum is the number of channels the image
	has. Thus itGrayscale is 1 for one channel, and itColor
	is 4 for 4 channels.
	*/
	enum ImageType{
		/**
		Indicates a Color image with four channels
		*/
		itColor = 0x4,

		/**
		Indicates a Grayscale image with one channel
		*/
		itGrayscale = 1
	};

	/**
	This indicates the size of each channel
	*/
	enum ImageChannelSize {
		ics8Bit = 0x08,
		ics16Bit = 0x10,
		ics32Bit = 0x20
	};

	enum ImageChannelType {
		ictInteger = 0,
		ictFloatingPoint = 1
	};

	enum PixelLayoutOrder {
		ploRGBA,
		ploBGRA,
		ploARGB,
		ploABGR
	};


	typedef unsigned long ImageDescriptor;

	virtual ~Image(){};

	/**
	returns the type of image that this Image instance represents.
	The integer value also indicates the number of color channels
	the Image has. Currently there are only 2 types, full color
	4 channel RGBA images, and 1 channel grayscale images.
	@return ImageType the value that represent the images type. The
	values have the follwing meanings:
	
		\li Image::itColor - indicate a full color image with red, green, blue, and alpha
		channels present. The integer value of this is 4 and can be used in operations
		to determine the total data size of the image, for example.
		\li Image::itGrayscale - indicates a grayscale image with a single channel. The
		numerical value of this is 1.
	

	*/
	virtual ImageType getType() const = 0;

	/**
	returns the number of bits each channel value represents. For example, by default, on
	Win32 systems, an Image is a full color RGBA image, with each value of a channel
	taking 8 bits, thus a single of pixel of this type of image takes up 32 bits - 4 channels
	with each channel component 8 bits in size.
	@return the channel's component size. Can be one of the following values:
	
		\li Image::ics8Bit - each channel component is 8 bits in sizem, thus the numerical
		value of this enum is 8.

		\li Image::ics16Bit - each channel component is 8 bits in sizem, thus the numerical
		value of this enum is 16.

		\li Image::ics32Bit - each channel component is 8 bits in sizem, thus the numerical
		value of this enum is 32.
	
	The last two types (Image::ics16Bit and Image::ics32Bit) are typically used for high end imaging.
	*/
	virtual ImageChannelSize getChannelSize() const = 0;

	/**
	returns whether the values for a channel are integer based or floating point based.
	@return ImageChannelType the image channel's value type. Can be one of
	
		\li  Image::ictInteger - the value of a channel is integer based. It's min and max values
		can be expressed by ImageBits::Traits::minVal() and ImageBits::Traits::maxVal() respectively.
		Typically this is a value between 0 and 2<sup>getChannelSize()</sup> (inclusive).
		\li  Image::ictFloatingPoint - the value of a channel is integer based. It's min and max values
		can be expressed by ImageBits::Traits::minVal() and ImageBits::Traits::maxVal() respectively.
		Typically this is a value in the range of 0.0 and 1.0 inclusive.
	
	*/
	virtual ImageChannelType getChannelType() const = 0;

	/**
	returns the pixel layout order. This explains how the individual color components
	of each of the color channels are laid out.
	@return PixelLayoutOrder the binary layout order of a single pixel value. Can be one of
	
		\li  Image::ploRGBA - indicates the Red value is in the MSB position, followed
		by Green, Blue and finally Alpha values. In code it might
		look like this for an Image with integer based 8 bit color channels
		\code
			ulong32 pixelColor = (redVal &lt;&lt; 24) | (greenVal &lt;&lt; 16) | (blueVal &lt;&lt; 8) | (alphaVal);
		\endcode
		Extracting the values from a single pixel color would be:
		\code
			redVal	= (pixelColor &amp; 0xFF000000) &gt;&gt; 24;
			greenVal = (pixelColor &amp; 0x00FF0000) &gt;&gt; 16;
			blueVal = (pixelColor &amp; 0x0000FF00) &gt;&gt; 8;
			alphaVal = (pixelColor &amp; 0x000000FF);
		\endcode
		

		\li  Image::ploBGRA - indicates the Blue value is in the MSB position, followed
		by Green, Red and finally Alpha values. In code it might look like this for an
		Image with integer based 8 bit color channels
		\code
			ulong32 pixelColor = (blueVal &lt;&lt; 24) | (greenVal &lt;&lt; 16) | (redVal &lt;&lt; 8) | (alphaVal);
		\endcode
		Extracting the values from a single pixel color would be:
		\code
			blueVal	= (pixelColor &amp; 0xFF000000) &gt;&gt; 24;
			greenVal = (pixelColor &amp; 0x00FF0000) &gt;&gt; 16;
			redVal = (pixelColor &amp; 0x0000FF00) &gt;&gt; 8;
			alphaVal = (pixelColor &amp; 0x000000FF);
		\endcode
		

		\li  Image::ploARGB - indicates the Alpha value is in the MSB position, followed
		by Red, Green and finally Blue values. In code it might look like this for an
		Image with integer based 8 bit color channels
		\code
			ulong32 pixelColor = (alphaVal &lt;&lt; 24) | (redVal &lt;&lt; 16) | (greenVal &lt;&lt; 8) | (blueVal);
		\endcode
		Extracting the values from a single pixel color would be:
		\code
			alphaVal	= (pixelColor &amp; 0xFF000000) &gt;&gt; 24;
			redVal = (pixelColor &amp; 0x00FF0000) &gt;&gt; 16;
			greenVal = (pixelColor &amp; 0x0000FF00) &gt;&gt; 8;
			blueVal = (pixelColor &amp; 0x000000FF);
		\endcode
		This is typically the way it would be stored on a linux based port of VCF.
		


		\li  Image::ploABGR - indicates the Alpha value is in the MSB position, followed
		by Blue, Green and finally Red values. In code it might look like this for an
		Image with integer based 8 bit color channels
		\code
			ulong32 pixelColor = (alphaVal &lt;&lt; 24) | (blueVal &lt;&lt; 16) | (greenVal &lt;&lt; 8) | (redVal);
		\endcode
		Extracting the values from a single pixel color would be:
		\code
			alphaVal	= (pixelColor &amp; 0xFF000000) &gt;&gt; 24;
			blueVal = (pixelColor &amp; 0x00FF0000) &gt;&gt; 16;
			greenVal = (pixelColor &amp; 0x0000FF00) &gt;&gt; 8;
			redVal = (pixelColor &amp; 0x000000FF);
		\endcode
		This is typically the way it would be stored on a Win32 based port of VCF.
		

		
	*/
	virtual PixelLayoutOrder getPixelLayoutOrder() const = 0;

    virtual void setSize( const unsigned long & width, const unsigned long & height )=0;

    virtual unsigned long getWidth()=0;

    virtual unsigned long getHeight()=0;

    virtual void addImageSizeChangedHandler( EventHandler* handler ) = 0;

    virtual void removeImageSizeChangedHandler( EventHandler* handler ) = 0;

	virtual ImageBits* getImageBits() = 0;

	/**
	Call this method before calling getImageContext() to "lock" the
	images pixels and ensure that the GraphicsContext returns is properly
	sycnhed with the image's data.
	*/
	virtual void beginDrawing() = 0;

	virtual void finishedDrawing() = 0;

	/**
	This retreives a graphics context for drawing on. Any drawing performed
	on the graphics context will be reflected in the internal pixel data of the
	image. On some platforms this may be "instantaneous" because the pixel
	data of the image is directly linked to the GraphicsContext (i.e. Win32),
	while on other platforms the drawing on the GraphicsContext needs to
	be "flushed" back to the images pixels. Because of this, you <b>must</b>
	call beginDrawing() <i>before</i> calling getImageContext(), and call
	finishedDrawing() when you're done with the GraphicsContext. Concrete
	implemententations of this class will transfer the image's contents to
	the GraphicsContext for beginDrawing(), finishedDrawing() will update
	the image's data due to any changes in the GraphicsContext. An example:
	\code
	Image* image = getImage(); //get an image from somewhere
	image->beginDrawing();

	GraphicsContext* gc = image->getImageContext();
	gc->rectangle( 20, 20, 400, 60 );
	gc->strokePath();

	image->finishedDrawing();

	\endcode

	*/
	virtual GraphicsContext* getImageContext() = 0;

	/**
	*returns the color that is used to blend with the contents of
	*a GraphicsContext when the Image is drawn. Only used when the
	*Image is set to Transparent
	*/
	virtual Color* getTransparencyColor() = 0;

	virtual void setTransparencyColor( Color* transparencyColor ) = 0;

	/**
	*Indicates whether or not the Image is using a transparent
	*color.
	*@return bool if this is true then the Image is transparent
	*and the contents of the underlying GraphicsContext will show through
	*wherever a pixel in the image is found that is the transparency color
	*/
	virtual bool isTransparent() = 0;

	virtual void setIsTransparent( const bool& transparent ) = 0;

	virtual void* getData() = 0;
};

};


#endif // _VCF_IMAGE_H__

/**
$Id$
*/
