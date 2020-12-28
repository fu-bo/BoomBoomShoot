/*
1. Explain the goal of the function
2. Explain the attribute of the class, the parameter and return of the function
3. One space between variable and the operation symbol
4. One space after the comma
5. Write the explanation for some specific or difficult part
*/



#include <stdio.h>
#include <stdlib.h>

 
class CharBitmap {
protected:
    /*
    attr wid width of the bitmap
    attr hei height of the bitmap
    */
    int wid, hei;
    
    // attr pix value at each (x, y) coordinate
    char *pix;

public:
    CharBitmap();
    ~CharBitmap();

    // clean up
    void CleanUp(void);
    // create new bitmap
    void Create(int w, int h);
    // seeting value p to position (x, y)
    void SetPixel(int x, int y, unsigned char p);
    // getting value of position (x, y)
    char GetPixel(int x, int y) const;
};

CharBitmap::CharBitmap() {
    wid = 0;
    hei = 0;
    pix = nullptr;
}

CharBitmap::~CharBitmap() {
    CleanUp();
}

void CharBitmap::CleanUp(void) {
    wid = 0;
    hei = 0;
    if(nullptr != pix) {
        delete [] pix;
    }
    pix = nullptr;
}

/*
para w width of the bitmap
para h height of the bitmap
return None
*/
void CharBitmap::Create(int w, int h) {
    CleanUp();

    pix = new char [w * h];
    wid = w;
    hei = h;

    for(int i = 0; i < wid * hei; ++i) {
        pix[i] = 0;
    }
}

void CharBitmap::SetPixel(int x, int y, unsigned char p) {
    if(0 <= x && x < wid && 0 <= y && y < hei) {
        pix[y * wid + x] = p;
    }
}

/*
para x coodinate
para y coodinate
return value at (x, y)
*/
char CharBitmap::GetPixel(int x, int y) const {
    if(0 <= x && x < wid && 0 <= y && y < hei) {
        return pix[y * wid + x];
    }
    return 0;
}
