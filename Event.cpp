/*
 *  Event.cpp
 *  Nuive
 *
 *  Created by Eric Fry on Wed Mar 26 2003.
 *  Copyright (c) 2003. All rights reserved.
 *
 */

#include <SDL.h>


#include "U6def.h"
#include "Configuration.h"

#include "MapWindow.h"
#include "Player.h"
#include "Book.h"
#include "Event.h"

uint32 nuvieGameCounter;

Event::Event(Configuration *cfg)
{
 config = cfg;
 clear_alt_code();
 active_alt_code = 0;
 alt_code_input_num = 0;
}

Event::~Event()
{
 delete book;
}

bool Event::init(ObjManager *om, MapWindow *mw, MsgScroll *ms, Player *p,
                 Converse *c)
{
 obj_manager = om;
 map_window = mw;
 scroll = ms;
 player = p;
 converse = c;

 mode = MOVE_MODE;

 book = new Book(config);
 if(book->init() == false)
   return false;

 return true;
}

bool Event::update()
{
  while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYUP:
                                        if(event.key.keysym.sym == SDLK_RALT
                                           || event.key.keysym.sym == SDLK_LALT)
                                        {
                                            clear_alt_code();
                                        }
                                        break;
				case SDL_KEYDOWN:
           if(scroll->handle_input(&event.key.keysym.sym))
             break; // break switch
           // alt-code input
           if((event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
              && (SDL_GetModState() & KMOD_ALT))
           {
               alt_code_str[alt_code_len++] = (char)event.key.keysym.sym;
               alt_code_str[alt_code_len] = '\0';
               if(alt_code_len == 3)
               {
                   alt_code(alt_code_str);
                   clear_alt_code();
               }
               break;
           }
           switch(event.key.keysym.sym)
            {
             case SDLK_UP    :
                               move(0,-1);
                               break;
             case SDLK_DOWN  :
                               move(0,1);
                               break;
             case SDLK_LEFT  :
                               move(-1,0);
                               break;
             case SDLK_RIGHT :
                               move(1,0);
                               break; 
             case SDLK_q     :
                               return false;
                               break;
             case SDLK_l     :
                               mode = LOOK_MODE;
                               scroll->display_string("Look-");
                               map_window->centerCursor();
                               map_window->set_show_cursor(true);
                               break;
             case SDLK_p     :
                               scroll->display_string("\nYou say:");
                               scroll->set_input_mode(true);
                               break;
             case SDLK_t     :
                               mode = TALK_MODE;
                               scroll->display_string("Talk-");
                               map_window->centerCursor();
                               map_window->set_show_cursor(true);
                               break;
             case SDLK_u     : 
                               mode = USE_MODE;
                               scroll->display_string("Use-");
                               map_window->centerCursor();
                               map_window->set_show_use_cursor(true);
                               break;
             case SDLK_RETURN  :
                               if(mode == LOOK_MODE)
                                 {
                                  mode = MOVE_MODE;
                                  look();
                                  map_window->set_show_cursor(false);
                                 }
                               else if(mode == TALK_MODE)
                                 {
                                  mode = MOVE_MODE;
                                  if(talk())
                                    scroll->set_talking(true);
                                  map_window->set_show_cursor(false);
                                 }
                               else if(mode == USE_MODE)
                                 {
                                  mode = MOVE_MODE;
                                  use(0,0);
                                  map_window->set_show_use_cursor(false);
                                 }
                               else
                                 {
                                  scroll->display_string("what?\n\n");
                                  scroll->display_prompt();
                                 }
                               break;
             case SDLK_ESCAPE:
                               if(mode == MOVE_MODE)
                                 {
                                  scroll->display_string("Pass!\n\n");
                                  scroll->display_prompt();
                                 }
                               else
                                 {
                                  mode = MOVE_MODE;
                                  map_window->set_show_use_cursor(false);
                                  map_window->set_show_cursor(false);
                                  scroll->display_string("what?\n\n");
                                  scroll->display_prompt();
                                 }
                               break;
             case SDLK_SPACE :
                               scroll->display_string("Pass!\n\n");
                               scroll->display_prompt();
                               break;
             default :
                               if(event.key.keysym.sym != SDLK_LALT
                                  && event.key.keysym.sym != SDLK_RALT)
                               {
                                scroll->display_string("what?\n\n");
                                scroll->display_prompt();
                               }
                               break;
            }
          break;
             
        case SDL_QUIT :
                       return false; //time to quit.
                       break;
        default:
        break;
			}
		}
    if(active_alt_code && scroll->get_input())
        alt_code_input(scroll->get_input());
 return true;
}


/* Get ID of Actor at cursor and check to see if him/her/it is willing to talk
 * to the player character.
 * Returns true if conversation starts, false if they don't talk, or if there
 * is no Actor at the requested location.
 */
bool Event::talk()
{
    Actor *npc = map_window->get_actorAtCursor();
    // load npc script
    if(npc && converse->start(npc))
        return(true);
    scroll->display_string("nothing!\n\n");
    scroll->display_prompt();
    return(false);
}


bool Event::move(sint16 rel_x, sint16 rel_y)
{
 if(mode == LOOK_MODE || mode == TALK_MODE)
    map_window->moveCursorRelative(rel_x,rel_y);
 else
  {
   if(mode == USE_MODE)
     {
      use(rel_x,rel_y);
     }
   else
      player->moveRelative(rel_x,rel_y);
  }
 return true;
}

bool Event::use(sint16 rel_x, sint16 rel_y)
{
 Obj *obj;
 uint16 x,y;
 uint8 level;
 
 player->get_location(&x,&y,&level);
 

 
 obj = obj_manager->get_obj((uint16)(x+rel_x), (uint16)(y+rel_y), level);
 
 if(obj)
 {
  scroll->display_string(obj_manager->look_obj(obj));
  scroll->display_string("\n");
  
  if(obj->obj_n == OBJ_U6_LADDER || obj->obj_n == OBJ_U6_HOLE)
   {
    if(obj->frame_n == 0) // DOWN
      {
       if(level == 0)
        {
         player->move(((obj->x & 0x07) | (obj->x >> 2 & 0xF8)), ((obj->y & 0x07) | (obj->y >> 2 & 0xF8)), level+1);
        }
       else
          player->move(obj->x,obj->y,level+1);
      }
    else //UP
      {
       if(level == 1)
         {
        //FIX clean his up a bit. :)
         player->move(
         obj->x / 8 * 8 * 4 + ((obj->quality & 0x03) * 8) + (obj->x - obj->x / 8 * 8),
         obj->y / 8 * 8 * 4 + ((obj->quality >> 2 & 0x03) * 8) + (obj->y - obj->y / 8 * 8),
         level-1);
         
         }
       else
         player->move(obj->x,obj->y,level-1);
      }
   }
  else
   {   
    obj_manager->use_obj(obj);
   }
 }
 
 scroll->display_string("\n");
 scroll->display_prompt();
  
 map_window->set_show_use_cursor(false);
 map_window->updateBlacking();
 mode = MOVE_MODE;

 return true;
}

bool Event::look()
{
 Obj *obj;
 char *data;
 
 scroll->display_string("Thou dost see ");
 scroll->display_string(map_window->lookAtCursor());
 obj = map_window->get_objAtCursor();
 if(obj && (obj->obj_n == OBJ_U6_SIGN || obj->obj_n == OBJ_U6_BOOK || obj->obj_n == OBJ_U6_SCROLL || obj->obj_n == OBJ_U6_PICTURE))
   {
    if(obj->quality != 0)
      {
       scroll->display_string(":\n\n");
       data = book->get_book_data(obj->quality-1);
       scroll->display_string(data);
       free(data);
      }
   }

 scroll->display_string("\n\n");
 scroll->display_prompt();

 return true;
}


/* Send input to active alt-code.
 */
void Event::alt_code_input(const char *in)
{
    switch(active_alt_code)
    {
        case 400: // talk to NPC (FIXME: get portrait and inventory too)
            if(!converse->start((uint8)strtol(in, NULL, 10)))
            {
                scroll->display_string("nobody!\n\n");
                scroll->display_prompt();
            }
            active_alt_code = 0;
            break;
    }
}


/* Get an alt-code from `cs' and use it.
 */
void Event::alt_code(const char *cs)
{
    uint16 c = (uint16)strtol(cs, NULL, 10);
    switch(c)
    {
        case 400: // talk to anyone (FIXME: get portrait and inventory too)
            scroll->display_string("Npc number? ");
            scroll->set_input_mode(true);
            active_alt_code = c;
    }
}


void Event::wait()
{
 SDL_Delay(TimeLeft());
}

//Protected

inline Uint32 Event::TimeLeft()
{
    static Uint32 next_time = 0;
    Uint32 now;

    now = SDL_GetTicks();
    if ( next_time <= now ) {
        next_time = now+NUVIE_INTERVAL;
        return(0);
    }
    return(next_time-now);
}
