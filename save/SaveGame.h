#ifndef __SaveGame_h__
#define __SaveGame_h__

/*
 *  SaveGame.h
 *  Nuvie
 *
 *  Created by Eric Fry on Sat May 22 2004.
 *  Copyright (c) 2004. All rights reserved.
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

#define NUVIE_SAVE_VERSION_MAJOR 0
#define NUVIE_SAVE_VERSION_MINOR 1

#define NUVIE_SAVE_VERSION       NUVIE_SAVE_VERSION_MAJOR * 256 + NUVIE_SAVE_VERSION_MINOR

#include <string>


class Configuration;
class ActorManager;
class Actor;
class Map;
class NuvieIO;

class SaveGame
{
 Configuration *config;
 
 uint16 num_saves;
 uint32 actual_play_time; //total play time for this save game in minutes.
 uint32 game_play_time; //total play time for this save game measured in game turns.
 
 std::string save_description;
 
 NuvieIOBuffer objlist;
 
 public:
  
 SaveGame(Configuration *cfg);
 ~SaveGame();

 bool load_new();
 bool load_original();
 bool load(const char *filename);

 bool save(const char *filename);
 
 uint16 get_num_saves() { return num_saves; };
 
 protected:

 bool load_objlist();
 char *get_objblk_path(char *path);
};

#endif /* __SaveGame_h__ */