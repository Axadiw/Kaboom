#pragma once

#include <hash_map>
#include "../Singleton.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

/*! 
 *  \brief     Tekstura
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Texture
{
public:
	/// œcie¿ka do tekstury na dysku
	const char* name;

	/// identyfikator tekstury (wykorzystywany przez OpenGL)
	GLuint texture;	

	/// format tekstury (wykorzystywany przez OpenGL)
	GLenum texture_format;

	Texture():texture(-1){};
	Texture(Texture& t) { name = t.name; texture = t.texture; texture_format = t.texture_format; }

	/// sprawdza, czy tekstura jest pusta
	inline bool isDummy() { return texture == -1; }

	/// binduje teksture
	inline void bind() { glBindTexture( GL_TEXTURE_2D, texture ); }

	void initOpenCVTexture()
	{
		glGenTextures(1, &texture);

		glBindTexture( GL_TEXTURE_2D, texture );

		// zaklêcia od OpenCV
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void updateFromOpenCvmat(cv::Mat frame)
	{		
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows,0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);

	}

	void releaseOpenCVTexture()
	{
		glDeleteTextures( 1, &texture ); 
	}

};

