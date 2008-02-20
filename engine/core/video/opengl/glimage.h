/***************************************************************************
 *   Copyright (C) 2005-2008 by the FIFE team                              *
 *   http://www.fifengine.de                                               *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

#ifndef FIFE_VIDEO_RENDERBACKENDS_OPENGL_GLIMAGE_H
#define FIFE_VIDEO_RENDERBACKENDS_OPENGL_GLIMAGE_H

// Standard C++ library includes
#include <vector>

// Platform specific includes
#include "util/fife_stdint.h"

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "video/image.h"

#include "fife_opengl.h"

namespace FIFE {

	/** Implements an Image using OpenGL.
	 *
	 * This class contains a texture handle bound to the data given to the constructor;
	 * it draws a textured @i Quad to the screen.
	 *
	 * @see Image
	 * @note Width and height are not limited to powers of two; non-power of two
	 * images will be converted internally.
	 * @todo Check the correctness of the generateTexture function on big endian systems (ppc)
	 */
	class GLImage : public Image {
	public:
		GLImage(SDL_Surface* surface);
		GLImage(const uint8_t* data, unsigned int width, unsigned int height);
		virtual ~GLImage();
		void render(const Rect& rect, SDL_Surface* dst, unsigned char alpha = 255);
		void saveImage(const std::string& filename);

	protected:
		void setClipArea(const Rect& cliparea, bool clear);

	private:
		unsigned int m_rows;
		unsigned int m_cols;

		float m_tex_x;
		float m_tex_y;

		unsigned int m_last_col_width;
		unsigned int m_last_row_height;

		GLuint *m_textureid;

		void cleanup();
		void resetGlimage();
		
		/** Generates 256*256 pixel chunks for renders. For reference, see
		 * http://developer.apple.com/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_texturedata/chapter_10_section_4.html
		 */
		void generateTextureChunks();
	};
}

#endif
/* vim: set noexpandtab: set shiftwidth=2: set tabstop=2: */
