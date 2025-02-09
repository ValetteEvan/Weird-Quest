#pragma once
#include "tools.h"

typedef struct
{
	sfVector2i defaultVideoMode;
	sfVideoMode defaultDesktopMode;
	sfVector2f ratioResolution;
	sfFloatRect rectViewPort;
	sfView* view;
	sfVector2f viewPos;
	sfFloatRect rectView;
	sfVector2f initialSize; 
	sfVector2f size;
}View;

View* mainView;
View* playerView;

/// <summary>
/// initialise the view
/// </summary>
/// <param name="_defaultVideoMode">the video mode</param>
/// <param name="_rectViewPort">the rect of the view port</param>
/// <param name="_defaultDesktopMode">the video mode</param>
/// <returns></returns>
View* setupView(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode);

/// <summary>
/// initialise the view with more settings
/// </summary>
/// <param name="_defaultVideoMode">the video mode</param>
/// <param name="_rectViewPort">the rect of the view port</param>
/// <param name="_defaultDesktopMode">the video mode</param>
/// <param name="_rectView">the rect view</param>
/// <returns></returns>
View* setupViewCustom(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode, sfFloatRect _rectView);

/// <summary>
/// set the size of the view
/// </summary>
/// <param name="_view"></param>
/// <param name="_rectview"></param>
void setRectView(View* _view, sfFloatRect _rectview);

/// <summary>
/// get the current view position
/// </summary>
/// <param name="_view">the current view</param>
/// <returns>the current position of the center of the view</returns>
sfVector2f getViewPosition(View* _view);

/// <summary>
/// set the view pos center to a given position
/// </summary>
/// <param name="_view">the current view</param>
/// <param name="_position">the next center position of the view</param>
void setViewPosition(View* _view, sfVector2f _position);

/// shaking view
void shakingView(float _timer);

/// Zooms in the given view.
void viewZoomIn(View* _view);

/// Zooms in the given view.
void viewZoomOut(View* _view);

void updateEditorView(float _valSpeed, sfRenderWindow* _window);

/// Dynamically updates the viewport, for instance when resizing the window, keeping the original aspect ratio.
/// \param _w - Window object based on which the new view should be computed
/// \param _v - View object to update
void UpdateView(struct Window* _w, View* _v);

void initializeView(View* _view, sfFloatRect viewport); 
void resetView(View* _view);