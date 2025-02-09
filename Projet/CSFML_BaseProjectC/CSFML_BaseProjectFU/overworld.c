#include "overworld.h"
#include "quest.h"
#include "player.h"
#include "dialogBox.h"
#include "viewManager.h"
#include "map.h"

char ow_ShamanWarning = 0;

void overWorld_initOverWorld(Window* _window) {
	ow_ShamanWarning = 0;
}

void overWorld_updateOverWorld(Window* _window) {
	if (map_GetState() == MAP_STATE_OVERWORLD) {
		if (plr_GetUnlockState() < PLRUNL_PIG_HAT) {
			sfVector2f pos = plr_GetFeetPos();
			char* name = lang_GetFromKey("name.shamane");
			if (ow_ShamanWarning == 0 && pos.y >= 5150.f) {
				db_Queue(name, lang_GetFromKey("dialog.shaman.stop_1"), pos);
				ow_ShamanWarning++;
			}
			else if (ow_ShamanWarning == 1 && pos.y >= 5200.f) {
				db_Queue(name, lang_GetFromKey("dialog.shaman.stop_2"), pos);
				ow_ShamanWarning++;
			}
			else if (ow_ShamanWarning == 2 && pos.y >= 5250.f) {
				db_Queue(name, lang_GetFromKey("dialog.shaman.stop_3"), pos);
				shakingView(.5f);
				ow_ShamanWarning++;
			}
			else if (ow_ShamanWarning == 3 && pos.y >= 5300.f) {
				db_Queue(name, lang_GetFromKey("dialog.shaman.stop_4"), vector2f(8800.f, 4070.f));
				plr_SetPos(vector2f(8870.f, 4290.f));
				shakingView(1.f);
			}
		}
	}
}

void overWorld_displayOverWorld(Window* _window) {

}

void overWorld_deinitOverWorld(Window* _window) {

}
