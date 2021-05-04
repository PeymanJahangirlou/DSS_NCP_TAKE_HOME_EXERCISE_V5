/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Simple Movie Image class
 */

#ifndef MovieImage_H
#define MovieImage_H

#include <memory>

/** @brief Movie image class **/
class MovieImage  {
public:

	/** @brief constructor init Movie image feilds **/
	MovieImage( unsigned char* movieImage,
				const int image_width,
				const int image_height)
			:  M_imageWidth(image_width), M_imageHeight(image_height)
	{
		M_movieImage = movieImage;
	}

	~MovieImage() { if (M_movieImage) delete M_movieImage; }

	/** Getters **/
	inline int width()  const { return M_imageWidth; }
	inline int height() const { return M_imageHeight; }
	inline unsigned char * image() const { return M_movieImage; }

	/** Setters **/
	void setWidth(const int w)  { M_imageWidth = w; }
	void setHeight(const int h) { M_imageHeight = h; }
	void setImage(unsigned char  * image) { M_movieImage = image; }

private:
	unsigned char * M_movieImage;
	int M_imageWidth;
	int M_imageHeight;
};

#endif
