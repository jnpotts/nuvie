/*
 *  UseCode.cpp
 *  Nuvie
 *
 *  Created by Eric Fry on Fri May 30 2003.
 *  Copyright (c) 2003. All rights reserved.
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

#include "nuvieDefs.h"

#include "U6LList.h"
#include "MsgScroll.h"
#include "ActorManager.h"
#include "Actor.h"
#include "UseCode.h"

UseCode::UseCode(Game *g, Configuration *cfg)
{
 game = g;
 config = cfg;

 int_ref = 0;
 actor_ref = NULL;
 obj_ref = NULL;
 actor2_ref = NULL;
 mapcoord_ref = NULL;
}

UseCode::~UseCode()
{
}

bool UseCode::init(ObjManager *om, Map *m, Player *p, MsgScroll *ms)
{
 obj_manager = om;
 map = m;
 player = p;
 scroll = ms;

 actor_manager = game->get_actor_manager();
 party = player->get_party();

 return true;
}


// use obj at location with src_obj as object_ref
 bool UseCode::use_obj(uint16 x, uint16 y, uint8 z, Obj *src_obj)
 {
  Obj *obj;
  
  obj = obj_manager->get_obj(x,y,z,true);
  
  if(obj == NULL)
    return false;
  
  return use_obj(obj,src_obj);
 }
 
void UseCode::toggle_frame(Obj *obj)
{
 if(obj->frame_n > 0)
   obj->frame_n--;
 else
   obj->frame_n = 1;
}


/* Print container contents and dump them on top of the container.
 */
bool UseCode::search_container(Obj *obj)
{
 Obj *temp_obj;
 U6Link *obj_link;

 /* Test whether this object has items inside it. */
 if((obj->container != NULL) &&
   ((obj_link = obj->container->end()) != NULL))
    {
     U6LList *obj_list = obj_manager->get_obj_list(obj->x, obj->y, obj->z);

     /* Add objects to obj_list. */
     for(; obj_link != NULL; obj_link = obj_link->prev)
      {
       temp_obj = (Obj*)obj_link->data;
       obj_list->add(temp_obj);
       temp_obj->status |= OBJ_STATUS_OK_TO_TAKE;
       temp_obj->status &= ~OBJ_STATUS_IN_CONTAINER;
       temp_obj->x = obj->x;
       temp_obj->y = obj->y;
       temp_obj->z = obj->z;
       scroll->display_string(obj_manager->look_obj(temp_obj,true));
       if(obj_link->prev) // more objects left
         scroll->display_string(obj_link->prev->prev ? ", " : ", and ");
      }
     /* Remove objects from the container. */
     obj->container->removeAll();
     return true;
    }
 return false;
}


/* Remove last object in container and return a pointer to it.
 */
Obj *UseCode::get_obj_from_container(Obj *obj)
{
    Obj *temp_obj;
    if(obj->container && obj->container->end())
    {
        temp_obj = (Obj *)obj->container->end()->data;
        obj->container->remove(temp_obj); // a pop_back() may be more efficient
        return(temp_obj);
    }
    return(NULL);
}


/* Subtract `count' from object quantity. Destroy the object completely if all
 * stacked objects were removed, or the object is not stackable, or `count' is
 * 0. This means it will be removed from the world or an actor's inventory, and
 * deleted.
 * Returns the original object if it still exists, because the count was smaller
 * than the object stack, or it could not be completely destroyed for whatever
 * reason. Returns NULL if the object was destroyed.
 */
Obj *UseCode::destroy_obj(Obj *obj, uint32 count)
{
    ActorManager *actor_manager = Game::get_game()->get_actor_manager();
    bool removed = false;

    // subtract
    if(count > 0 && obj_manager->is_stackable(obj) && obj->qty > count)
        obj->qty -= count;
    else // destroy
    {
        if(obj->is_in_inventory())
            removed = actor_manager->get_actor_holding_obj(obj)->inventory_remove_obj(obj);
        else if(obj->is_in_container())
//            removed = obj_manager->get_obj_container(obj);
            removed = false; // FIXME
        else
            removed = obj_manager->remove_obj(obj);
        if(removed)
        {
            obj_manager->delete_obj(obj);
            obj = NULL;
        }
    }
    return(obj);
}
