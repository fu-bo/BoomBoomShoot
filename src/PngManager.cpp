#include "yspng.h"
#include "fssimplewindow.h"
#include "PngManager.hpp"
#include "MyDraw.hpp"

#include <vector>
#include <string>

PngManager::PngManager(const std::vector<std::string>& png_files){
	flag_first_render_ = true;
	png_num_ = png_files.size();
	png_decoder_ = new YsRawPngDecoder[png_num_];
	png_texture_ = new GLuint[png_num_];
	for(int i = 0; i < png_num_; i++) {
	    png_decoder_[i].Decode(png_files[i].c_str());
	}
}

PngManager::~PngManager(){
	CleanUp();
}

void PngManager::CleanUp(){
	delete[] png_decoder_;
	delete[] png_texture_;
}

void PngManager::Draw(int png_id, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double alpha) {
	if(flag_first_render_) {
		flag_first_render_ = false;
		for(int i= 0; i < png_num_; i++){
			glGenTextures(1,&png_texture_[i]);  // Reserve one texture identifier
	        glBindTexture(GL_TEXTURE_2D,png_texture_[i]);  // Making the texture identifier current (or bring it to the deck)

	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	        glTexImage2D(GL_TEXTURE_2D,
	             0,    // Level of detail
	             GL_RGBA,
	             png_decoder_[i].wid,
	             png_decoder_[i].hei,
	             0,    // Border width, but not supported and needs to be 0.
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             png_decoder_[i].rgba);
		}
	}

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4d(1.0,1.0,1.0, alpha);

    glEnable(GL_TEXTURE_2D);  // Begin using texture mapping

    glBindTexture(GL_TEXTURE_2D,png_texture_[png_id]);

    glBegin(GL_QUADS);

    glTexCoord2d(0.0,0.0);
    MyDraw::myglVertex2d(x1,y1);

    glTexCoord2d(1.0,0.0);
    MyDraw::myglVertex2d(x2,y2);

    glTexCoord2d(1.0,1.0);
    MyDraw::myglVertex2d(x3,y3);

    glTexCoord2d(0.0,1.0);
    MyDraw::myglVertex2d(x4,y4);

    glEnd();
    glDisable(GL_BLEND);

    glDisable(GL_TEXTURE_2D);  // End using texture mapping
}