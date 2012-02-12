#ifndef __ScriptCutscene_h__
#define __ScriptCutscene_h__

/*
 *  Script.h
 *  Nuvie
 *
 *  Created by Eric Fry on Tue Sep 20 2011.
 *  Copyright (c) 2011. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include "GUI.h"
#include "GUI_widget.h"
#include "U6Shape.h"
#include "Configuration.h"

class SoundManager;
class Font;

extern "C"
{
#include "lauxlib.h"
}

struct CSImage {
	U6Shape *shp;
	uint16 refcount;

	CSImage(U6Shape *shape)
	{
		shp = shape;
		refcount = 0;
	}
};

struct CSSprite {
	sint16 x;
	sint16 y;
	uint8 opacity;
	CSImage *image;
	bool visible;
	SDL_Rect clip_rect;
	std::string text;

	CSSprite()
	{
		x = 0;
		y = 0;
		opacity = 255;
		image = NULL;
		visible = false;
		clip_rect.x = 0;
		clip_rect.y = 0;
		clip_rect.w = 0;
		clip_rect.h = 0;
		text = "";
	}
};

void nscript_init_cutscene(lua_State *L, Configuration *cfg, GUI *gui, SoundManager *sm);


class ScriptCutscene : public GUI_Widget
{
private:
	Configuration *config;
	GUI *gui;
	std::list<CSSprite *> sprite_list; // in paint order
	Screen *screen;
	uint8 *palette;
	SoundManager *sound_manager;
	Font *font;
	SDL_Rect clip_rect;
	uint16 x_off, y_off;
	uint32 next_time;
	uint32 loop_interval;
	uint8 screen_opacity;

public:
	ScriptCutscene(GUI *g, Configuration *cfg, SoundManager *sm);
	CSImage *load_image(const char *filename, int idx);
	std::vector<CSImage *> load_all_images(const char *filename);
	void add_sprite(CSSprite *s) { sprite_list.push_back(s); }
	void remove_sprite(CSSprite *s) { sprite_list.remove(s); }

	void load_palette(const char *filename, int idx);
	void set_palette_entry(uint8 idx, uint8 r, uint8 g, uint8 b);
	void rotate_palette(uint8 idx, uint8 length);
	void set_screen_opacity(uint8 new_opacity);

	void set_update_interval(uint16 interval);
	void update();

	void wait();
	void Display(bool full_redraw);

	void print_text(CSImage *image, const char *string, uint16 *x, uint16 *y, uint16 startx, uint16 width, uint8 color);

	SoundManager *get_sound_manager() { return sound_manager; }

	uint16 get_x_off() { return x_off; }
	uint16 get_y_off() { return y_off; }

	Font *get_font() { return font; }
	Configuration *get_config() { return config; }

	void hide_sprites();

};

#endif /* __ScriptCutscene_h__ */
