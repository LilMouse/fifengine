/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

// Standard C++ library includes

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "util/debugutils.h"
#include "util/log.h"

#include "luagui.h"

namespace luaGui {

	AWidget::~AWidget() {
	}
	int AWidget::l_setSize(lua_State *L) {
		int w = int(luaL_checknumber(L, 1));
		int h = int(luaL_checknumber(L, 2));
		f_setSize(w, h);
		return 0;
	}
	int AWidget::l_getSize(lua_State *L) {
		int w = f_getWidth();
		int h = f_getHeight();
		lua_pushnumber(L, w);
		lua_pushnumber(L, h);
		return 2;
	}
	int AWidget::l_setPosition(lua_State *L) {
		int x = int(luaL_checknumber(L, 1));
		int y = int(luaL_checknumber(L, 2));
		f_setPosition(x, y);
		return 0;
	}
	int AWidget::l_getPosition(lua_State *L) {
		int x = f_getX();
		int y = f_getY();
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		return 2;
	}
	int AWidget::l_getBorderSize(lua_State* L) {
		lua_pushnumber(L, f_getBorderSize());
		return 1;
	}
	int AWidget::l_setBorderSize(lua_State* L) {
		int s = int(luaL_checknumber(L, 1));
		f_setBorderSize(s);
		return 0;
	}
	int AWidget::l_setFocusable(lua_State *L) {
		bool v = lua_toboolean(L, 1);
		f_setFocusable(v);
		return 0;
	}
	int AWidget::l_isFocusable(lua_State *L) {
		lua_pushboolean(L, f_isFocusable());
		return 1;
	}
	int AWidget::l_isFocused(lua_State *L) {
		lua_pushboolean(L, f_isFocused());
		return 1;
	}
	int AWidget::l_setVisible(lua_State *L) {
		bool v = lua_toboolean(L, 1);
		f_setVisible(v);
		return 0;
	}
	int AWidget::l_isVisible(lua_State *L) {
		lua_pushboolean(L, f_isVisible());
		return 1;
	}
	int AWidget::l_setEnabled(lua_State *L) {
		bool v = lua_toboolean(L, 1);
		f_setEnabled(v);
		return 0;
	}
	int AWidget::l_isEnabled(lua_State *L) {
		lua_pushboolean(L, f_isEnabled());
		return 1;
	}
	int AWidget::l_setEventId(lua_State *L) {
		f_setEventId(luaL_checkstring(L, 1));
		return 0;
	}
	int AWidget::l_getEventId(lua_State *L) {
		const std::string & ref = f_getEventId();
		lua_pushstring(L, ref.c_str());
		return 1;
	}
	gcn::Color* AWidget::lua2gcn_color_cast(lua_State *L) {
		typedef struct { luaGui::Color *pT; } ud_Col;
		void *p = NULL;
		gcn::Color * c =NULL;
		if ((p = luaL_checkudata(L, 1, luaGui::Color::className))) {
			ud_Col *u = static_cast<ud_Col*>(p);
			c = dynamic_cast<gcn::Color*>(u->pT);
		}
		return c;
	}

	/** Helper function to check the type of userdata
	 *  Behaves just like luaL_checkudata without
	 *  generating a typeerror and thus a longjmp
	 */
	bool isudata(lua_State* L, int ud, const char* name) {
		void *p = lua_touserdata(L, ud);
		if( p == 0 ) {
			return false;
		}

		// get correct metatable
		lua_getfield(L, LUA_REGISTRYINDEX, name);
		if ( 0 == lua_getmetatable(L, ud) ) {
			lua_pop(L,1);
			return false;
		}

		// compare metatables
		bool equal = lua_rawequal(L,-1,-2);
		lua_pop(L, 2);

		return equal;
	}
	
	gcn::Font* AWidget::lua2font_cast(lua_State *L) {
		// FIXME: Direct dispatch. Rather ugly.
		typedef struct { luaGui::TTFont *pT; }    ud_TTFont;
		typedef struct { luaGui::ImageFont *pT; } ud_ImageFont;
		typedef struct { luaGui::AAFont *pT; }		ud_AAFont;
		

		if( !lua_isuserdata(L,1) ) {
			luaL_typerror(L, 1, "AAFont, ImageFont or TTFont");
		}

		void *p = lua_touserdata(L,1);
		gcn::Font* font = 0;

		if( isudata(L,1,luaGui::TTFont::className) ) {
			ud_TTFont *u = static_cast<ud_TTFont*>(p);
			font = u->pT->getFont();
		} else if( isudata(L,1,luaGui::ImageFont::className) ) {
			ud_TTFont *u = static_cast<ud_TTFont*>(p);
			font = u->pT->getFont();
		}
		else if ( isudata(L, 1, luaGui::AAFont::className) ) {
			ud_AAFont *u = static_cast<ud_AAFont*>(p);
			font = u->pT->getFont();
		}
		if( font == 0 ) {
			luaL_typerror(L, 1, "valid AAFont, ImageFont or TTFont");
		}
		return font;

// 		if ((p = luaL_checkudata(L, 1, luaGui::TTFont::className))) {
// 			ud_TTFont *u = static_cast<ud_TTFont*>(p);
// 			c = u->pT->getFont();
// 		}
// 		return c;
	}

	int AWidget::l_setBaseColor(lua_State *L) {
		gcn::Color *c = lua2gcn_color_cast(L);
		if (c != NULL) {
			gcn::Color & c2 = *c;
			f_setBaseColor(c2);
		}
		else
			FIFE::Log("AWidget", FIFE::Log::LEVEL_WARN) << "Cannot set base-color; invalid color object";
		return 0;
	}
	/*
	int AWidget::l_getBaseColor(lua_State *L) {
		gcn::Color col = f_getBaseColor();
		luaGui::Color *c = reinterpret_cast<luaGui::Color*>(&col);
		Lunar<Color>::push(L, c, false);
		return 1;
	}*/
	int AWidget::l_setForegroundColor(lua_State *L) {
		gcn::Color *c = lua2gcn_color_cast(L);
		if (c != NULL) {
			gcn::Color & c2 = *c;
			f_setForegroundColor(c2);
		}
		else
			FIFE::Log("AWidget", FIFE::Log::LEVEL_WARN) << "Cannot set fg-color; invalid color object";
		return 0;
	}
	/*
	int AWidget::l_getForegroundColor(lua_State *L) {
		return 0;
	}*/
	int AWidget::l_setBackgroundColor(lua_State *L) {
		gcn::Color *c = lua2gcn_color_cast(L);
		if (c != NULL) {
			gcn::Color & c2 = *c;
			f_setBackgroundColor(c2);
		}
		else
			FIFE::Log("AWidget", FIFE::Log::LEVEL_WARN) << "Cannot set bg-color; invalid color object";
		return 0;
	}
	/*
	int AWidget::l_getBackgroundColor(lua_State *L) {
		return 0;
	}*/

	/*
	int AWidget::l_setFont(lua_State *L) {
		gcn::SDLTrueTypeFont* fp = lua2sdlttf_cast(L);
		if (fp != NULL) {
			f_setFont(fp);
		}
		else
			FIFE::Log("AWidget", FIFE::Log::LEVEL_WARN) << "Cannot set font; invalid font object";
		return 0;
	}*/

	int AWidget::l_requestModalFocus(lua_State *L) {
		try {
			f_requestModalFocus();
		}
		catch (const gcn::Exception & e) {
			lua_pushboolean(L, false);
			return 1;
		}
		lua_pushboolean(L, true);
		return 1;
	}

	int AWidget::l_releaseModalFocus(lua_State *L) {
		f_releaseModalFocus();
		return 0;
	}

	// note that in *this* case the following is not done;
	// this means 'AWidget' is unknown to Lua, the magic is
	// done by c++ inheritance.
	
	// For any other class a similiar block is required!
#if 0
	const char AWidget::className[] = "AWidget";
#define method(class, name) {#name, &class::l_ ## name}

	Lunar<AWidget>::RegType AWidget::methods[] = {
		method(AWidget, getSize),
		method(AWidget, getPosition),
		method(AWidget, setSize),
		method(AWidget, setPosition),
		method(AWidget, getBorderSize),
		method(AWidget, setBorderSize),
		method(AWidget, setFocusable),
		method(AWidget, isFocusable),
		method(AWidget, hasFocus),
		method(AWidget, setEnabled),
		method(AWidget, isEnabled),
		method(AWidget, setVisible),
		method(AWidget, isVisible),
		method(AWidget, setEventId),
		method(AWidget, getEventId),
		{0,0}
	};
#endif
}
/* vim: set noexpandtab: set shiftwidth=2: set tabstop=2: */
