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

#include "scaleup.h"

namespace KWin
{

KWIN_EFFECT( scaleup, ScaleUpEffect )

ScaleUpEffect::ScaleUpEffect()
    {
    reconfigure( ReconfigureAll );
    }

void ScaleUpEffect::reconfigure( ReconfigureFlags )
    {
    KConfigGroup conf = EffectsHandler::effectConfig("ScaleUp");
    mDuration = conf.readEntry("Duration", 250);
    QString disableForWindowClass = conf.readEntry("DisableForWindowClass", "yakuake;plasma-desktop");
    foreach ( QString i, disableForWindowClass.split(";") )
        mDisableForWindowClass.insert(i);
    }

void ScaleUpEffect::prePaintScreen( ScreenPrePaintData& data, int time )
    {
    if( !mTimeLineWindows.isEmpty() )
        data.mask |= PAINT_SCREEN_WITH_TRANSFORMED_WINDOWS;
    effects->prePaintScreen( data, time );
    }

void ScaleUpEffect::prePaintWindow( EffectWindow* w, WindowPrePaintData& data, int time )
    {
    if( mTimeLineWindows.contains( w ) )
        {
        mTimeLineWindows[ w ].addTime( time );
        if( mTimeLineWindows[ w ].value() < 1 )
            data.setTransformed();
        else
            mTimeLineWindows.remove( w );
        }
    effects->prePaintWindow( w, data, time );
    }

void ScaleUpEffect::paintWindow( EffectWindow* w, int mask, QRegion region, WindowPaintData& data )
    {
    if( mTimeLineWindows.contains( w ) && mWindowGeometryCache.contains( w ) )
        {
        const QRect& old = mWindowGeometryCache[ w ];
        if ( old.width() > 0 && old.height() > 0 )
            {
            double scale = mTimeLineWindows[ w ].value();
            if ( w->width() > 0 )
                data.xScale *= interpolate( (double)old.width() / w->width(), 1, scale );
            if ( w->height() > 0 )
                data.yScale *= interpolate( (double)old.height() / w->height(), 1, scale );
            data.xTranslate += interpolate( old.x() - w->x(), 0, scale );
            data.yTranslate += interpolate( old.y() - w->y(), 0, scale );
            effects->paintWindow( w, mask, region, data );
            }
        }
    effects->paintWindow( w, mask, region, data );
    }

void ScaleUpEffect::postPaintWindow( EffectWindow* w )
    {
    if( mTimeLineWindows.contains( w ) )
        w->addRepaintFull(); // trigger next animation repaint
    effects->postPaintWindow( w );
    }

bool ScaleUpEffect::isScaleUpWindow( EffectWindow* w )
    {
    if ( w->isManaged() && ( w->isNormalWindow() || w->isDialog() ) )
        {
        foreach ( QString i , w->windowClass().split(" ") )
            if ( mDisableForWindowClass.contains( i ) )
                return false;
        return true;
        }
    else
        return false;
    }

void ScaleUpEffect::windowUserMovedResized( EffectWindow* w, bool first, bool last )
    {
    if ( first && last && w->isOnCurrentDesktop() && isScaleUpWindow(w) )
        {
        TimeLine& t = mTimeLineWindows[ w ];
        t.setCurveShape( TimeLine::EaseInOutCurve );
        t.setDuration( animationTime( mDuration ) );
        t.setProgress( 0.0 );
        w->addRepaintFull();
        }
    }

void ScaleUpEffect::windowGeometryShapeChanged( EffectWindow* w, const QRect& old )
    {
    mWindowGeometryCache[ w ] = old;
    }

void ScaleUpEffect::windowClosed( EffectWindow* w )
    {
    mTimeLineWindows.remove( w );
    mWindowGeometryCache.remove( w );
    }

} // namespace
