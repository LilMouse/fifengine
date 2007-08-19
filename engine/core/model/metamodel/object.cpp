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
#include "object.h"
#include "action.h"


namespace FIFE { namespace model { 
	Action* Object::addAction(const std::string& action_name) {
		Action* a = getAction(action_name);
		if (!a) {
			a = new Action();
			m_actions[action_name] = a;
		}
		return a;
	}

	Action* Object::getAction(const std::string& action_name) {
		std::map<std::string, Action*>::const_iterator i = m_actions.find(action_name);
		if (i != m_actions.end()) {
			if (m_inherited) {
				return m_inherited->getAction(action_name);
			}
			return NULL;
		}
		return i->second;
	}
}}
