#include "viewManager.h"
#include "player.h"
#include "windowManager.h"
#include "stateManager.h"
#include "math.h"
#include "map.h"

float view_TimerShake;

View* setupView(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode)
{
	View* tmp = (View*)malloc(sizeof(View));
	if (!tmp) return NULL;

	tmp->defaultDesktopMode = _defaultDesktopMode;
	tmp->defaultVideoMode = _defaultVideoMode;
	tmp->ratioResolution = (sfVector2f){ (float)tmp->defaultDesktopMode.width / (float)tmp->defaultVideoMode.x, (float)tmp->defaultDesktopMode.height / (float)tmp->defaultVideoMode.y };
	tmp->rectViewPort = _rectViewPort;
	tmp->view = sfView_create();
	tmp->viewPos = (sfVector2f){ tmp->defaultVideoMode.x / 2 * tmp->rectViewPort.width, tmp->defaultVideoMode.y / 2 * tmp->rectViewPort.height };
	tmp->rectView = (sfFloatRect){ (float)tmp->defaultDesktopMode.width / 2 , (float)tmp->defaultDesktopMode.height / 2, (float)tmp->defaultDesktopMode.width / tmp->ratioResolution.x * tmp->rectViewPort.width, (float)tmp->defaultDesktopMode.height / tmp->ratioResolution.y * tmp->rectViewPort.height };
	
	sfView_reset(tmp->view, tmp->rectView); 
	sfView_setViewport(tmp->view, tmp->rectViewPort);
	tmp->size = sfView_getSize(tmp->view);
	return tmp;
}


View* setupViewCustom(sfVector2i _defaultVideoMode, sfFloatRect _rectViewPort, sfVideoMode _defaultDesktopMode, sfFloatRect _rectView)
{
	View* tmp = (View*)malloc(sizeof(View));
	if (!tmp) return NULL;

	tmp->defaultDesktopMode = _defaultDesktopMode;
	tmp->defaultVideoMode = _defaultVideoMode;
	tmp->ratioResolution = (sfVector2f){ (float)tmp->defaultDesktopMode.width / (float)tmp->defaultVideoMode.x, (float)tmp->defaultDesktopMode.height / (float)tmp->defaultVideoMode.y };
	tmp->rectViewPort = _rectViewPort;
	tmp->view = sfView_create();
	tmp->viewPos = (sfVector2f){ tmp->defaultVideoMode.x / 2 * tmp->rectViewPort.width, tmp->defaultVideoMode.y / 2 * tmp->rectViewPort.height };
	tmp->rectView = _rectView;
	sfView_reset(tmp->view, tmp->rectView);
	sfView_setViewport(tmp->view, tmp->rectViewPort);
	tmp->size = sfView_getSize(tmp->view);
	return tmp;
}

void setRectView(View* _view, sfFloatRect _rectview)
{
	_view->rectView = _rectview;
	sfView_reset(_view->view, _view->rectView);
	_view->size = sfView_getSize(_view->view);
}

sfVector2f getViewPosition(View* _view)
{
	return _view->viewPos;
}

void setViewPosition(View* _view, sfVector2f _position)
{
	_view->viewPos = _position;
	sfView_setCenter(_view->view, _view->viewPos);
}

void viewZoomIn(View* _view) {
	sfView_zoom(_view->view, 0.997);
	
	_view->size = sfView_getSize(_view->view); 
}

void viewZoomOut(View* _view) {
	sfView_zoom(_view->view, 1.003);

	_view->size = sfView_getSize(_view->view);
}


//! Mettre un temps de tremblement 
void shakingView(float _timer) {
	view_TimerShake = _timer;
}

void updateEditorView(float _valSpeed, sfRenderWindow* _window)
{
	float dt = getDeltaTime();
	float viewSpeed = _valSpeed;

	if (customKeyboardIsKeyPressed(sfKeyZ, _window))
	{
		mainView->viewPos.y -= dt * viewSpeed;
		setViewPosition(mainView, mainView->viewPos);
	}
	else if (customKeyboardIsKeyPressed(sfKeyS, _window))
	{
		mainView->viewPos.y += dt * viewSpeed;
		setViewPosition(mainView, mainView->viewPos);
	}
	if (customKeyboardIsKeyPressed(sfKeyQ, _window))
	{
		mainView->viewPos.x -= dt * viewSpeed;
		setViewPosition(mainView, mainView->viewPos);
	}
	else if (customKeyboardIsKeyPressed(sfKeyD, _window))
	{
		mainView->viewPos.x += dt * viewSpeed;
		setViewPosition(mainView, mainView->viewPos);
	}

	if (customKeyboardIsKeyPressed(sfKeyO, _window)) viewZoomIn(mainView);
	else if (customKeyboardIsKeyPressed(sfKeyL, _window)) viewZoomOut(mainView);
}

void UpdateView(Window* _w, View* _v) {
	sfVector2u windowSize = sfRenderWindow_getSize(_w->renderWindow);
	float ratio1 = windowSize.x / (float)_v->defaultVideoMode.x;
	float ratio2 = windowSize.y / (float)_v->defaultVideoMode.y;
	float minRatio = fminf(ratio1, ratio2);
	_v->ratioResolution = (sfVector2f){ minRatio, minRatio };
	_v->rectView = (sfFloatRect){
		windowSize.x / 2.0f,
		windowSize.y / 2.0f,
		(float)windowSize.x / _v->ratioResolution.x * _v->rectViewPort.width,
		(float)windowSize.y / _v->ratioResolution.y * _v->rectViewPort.height
	};
	sfView_setViewport(_v->view, _v->rectViewPort);

	sfVector2f vecShake = NULLVECTF;
	if (view_TimerShake > 0.f) {
		view_TimerShake -= getDeltaTime();
		vecShake = multiplyVectorsrf(normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f))), view_TimerShake * 10.f);
		setViewPosition(mainView, addVectorsrf(mainView->viewPos, vecShake));
	}
	sfView_setCenter(_v->view, addVectorsrf(_v->viewPos, vecShake));

	if (getState() == GAME) {
		if (getGameState() != GAME_FISH) {
			sfVector2f max = { map.mapSize.x * 64.f - 960.f, map.mapSize.y * 64.f - 540.f };
			sfVector2f playerPos = plr_GetFeetPos();

			if (playerPos.x < max.x && playerPos.x > 960.f) {
				_v->viewPos.x = playerPos.x;
			}
			else {
				if (playerPos.x >= max.x)
					_v->viewPos.x = max.x;
				else
					_v->viewPos.x = 960.f;
			}


			if (playerPos.y > 540.f && playerPos.y < max.y) {
				_v->viewPos.y = playerPos.y;
			}
			else {
				if (playerPos.y >= max.y)
					_v->viewPos.y = max.y;
				else
					_v->viewPos.y = 540.f;
			}

		}
		else {
			_v->viewPos = (sfVector2f){ 960.f, 540.f };
		}
	}
	else if (getState() == MENU) {
		resetView(mainView);
		mainView->viewPos = vector2f(960.f, 540.f);
		sfView_setCenter(mainView->view, mainView->viewPos);
	}

}


void initializeView(View* _view, sfFloatRect viewport) {
	_view->view = sfView_createFromRect(viewport); 
	_view->size = sfView_getSize(_view->view); 
	_view->initialSize = _view->size; 
} 

void resetView(View* _view) {
	sfView_setSize(_view->view, _view->initialSize);
	_view->size = _view->initialSize;
}
