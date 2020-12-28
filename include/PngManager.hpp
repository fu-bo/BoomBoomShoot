#ifndef PNGMANAGER_HPP
#define PNGMANAGER_HPP

#include <vector>
#include <string>
#include "yspng.h"


class PngManager
{
private:
	PngManager(const PngManager& another);
    PngManager & operator=(const PngManager & another);
protected:
    // array of YsRawPngDecoder
    YsRawPngDecoder* png_decoder_;
    // array of GLuint
    GLuint* png_texture_;
    // total image number
    int png_num_;
    // flag for first render
    bool flag_first_render_;
public:
	PngManager(const std::vector<std::string>& png_files);
	~PngManager();
	void CleanUp();
	void Draw(int png_id, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double alpha = 1.0);
	inline int Wid(int png_id) const { return png_decoder_[png_id].wid; }
	inline int Hei(int png_id) const { return png_decoder_[png_id].hei; }
	inline int Num() const { return png_num_; }
};



#endif