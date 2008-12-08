#ifndef ACTION_ENABLE_HIGHLIGHT_MODEL_H
#define ACTION_ENABLE_HIGHLIGHT_MODEL_H

#include "EventHandler.h"
#include "color.h"

enum HighlightMode {
	HighlightDisable,
	HighlightOutline,
	HighlightFlash
};

/** Message to request that the model be highlighted */
class ActionEnableModelHighlight : public Action {
public:
	ActionEnableModelHighlight(HighlightMode _mode,
	                           float _time,
	                           color _colora = yellow,
	                           color _colorb = white) {
		mode = _mode;
		time = _time;
		colora = _colora;
		colorb = _colorb;
	}
	
public:
	HighlightMode mode;
	float time;
	color colora;
	color colorb;
};

#endif
