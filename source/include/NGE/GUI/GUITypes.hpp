/* 
 * File:   GUITypes.h
 * Author: tku
 *
 * Created on 5 lipiec 2012, 01:17
 */

#ifndef GUITYPES_HPP
#define	GUITYPES_HPP

namespace NGE
{
    namespace GUI
    {

        enum WidgetEvents
        {
            CLICKED = 1,
            DOUBLE_CLICKED,
            PRESSED,
            DRAGGED,
            RELEASED,
            MOVED
        };

        enum AnchorPoints
        {
            CENTER,
            CORNERLU,
            CORNERRU,
            CORNERLD,
            CORNERRD
        };

        enum Orientation
        {
            HORIZONTAL,
            VERTICAL
        };

        enum WidgetTypes
        {
            SLIDER = 1,
            BUTTON,
            CHECK_BOX,
            CHECK_BOX_MARK,
            RADIO_BUTTON,
            CHECK_RB_MARK,
            LABEL,
            TEXT_AREA,
            MATERIAL_SURFACE,
            PANEL,
            SEPARATOR,
            TEXT_BOX,
            COMBO_BOX,
            TABBED_PANEL,
            UNKNOWN
        };
        
        enum Layout
        {
            FREE_LAYOUT,
            YAXIS_LAYOUT,
            YAXIS_CEN_LAYOUT,
            XAXIS_LAYOUT,
            GRID_LAYOUT
        };
    }
}

#endif	/* GUITYPES_HPP */

