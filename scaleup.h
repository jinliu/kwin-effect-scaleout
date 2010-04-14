/********************************************************************
Copyright (C) 2010 Jin Liu <m.liu.jin@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef KWIN_SCALEUP_H
#define KWIN_SCALEUP_H

#include <kwineffects.h>

namespace KWin
{

class ScaleUpEffect
    : public Effect
    {
    public:
        ScaleUpEffect();
        virtual void reconfigure( ReconfigureFlags );
        virtual void prePaintScreen( ScreenPrePaintData& data, int time );
        virtual void prePaintWindow( EffectWindow* w, WindowPrePaintData& data, int time );
        virtual void paintWindow( EffectWindow* w, int mask, QRegion region, WindowPaintData& data );
        virtual void postPaintWindow( EffectWindow* w );
        virtual void windowUserMovedResized( EffectWindow* w, bool first, bool last );
        virtual void windowGeometryShapeChanged( EffectWindow* w, const QRect& old );
        virtual void windowClosed( EffectWindow* w );
    private:
        bool isScaleUpWindow ( EffectWindow* w );
        QHash< const EffectWindow*, TimeLine > mTimeLineWindows;
        QHash< const EffectWindow*, QRect > mWindowGeometryCache;

        int mDuration;
        QSet< QString > mDisableForWindowClass;
    };

} // namespace

#endif
