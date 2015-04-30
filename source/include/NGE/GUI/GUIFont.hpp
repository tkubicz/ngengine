/* 
 * File:   GUIFont.hpp
 * Author: tku
 *
 * Created on 25 luty 2013, 22:15
 */

#ifndef GUIFONT_HPP
#define	GUIFONT_HPP

#include "NGE/Media/Fonts/FreeTypeFont.hpp"

namespace NGE
{
    namespace GUI
    {
        class GUIFont
        {
        public:
            GUIFont(const std::string& path = NULL);
            ~GUIFont();
            
            Media::Fonts::FreeTypeFont* GetFont();
        };
    }
}

#endif	/* GUIFONT_HPP */

