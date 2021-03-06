// -----------------------------------------------------------------------------
// |    ___  ___  _  _ _     ___                                        _      |
// |   / __>| . \| || \ |   | __>_ _  ___ ._ _ _  ___  _ _ _  ___  _ _ | |__   |
// |   \__ \|  _/| ||   |   | _>| '_><_> || ' ' |/ ._>| | | |/ . \| '_>| / /   |
// |   <___/|_|  |_||_\_|   |_| |_|  <___||_|_|_|\___.|__/_/ \___/|_|  |_\_\   |
// |                                                                           |
// |---------------------------------------------------------------------------|
//
// http://spinframework.sourceforge.net
// Copyright (C) 2009 Mike Wozniewski, Zack Settel
//
// Developed/Maintained by:
//    Mike Wozniewski (http://www.mikewoz.com)
//    Zack Settel (http://www.sheefa.net/zack)
// 
// Principle Partners:
//    Shared Reality Lab, McGill University (http://www.cim.mcgill.ca/sre)
//    La Societe des Arts Technologiques (http://www.sat.qc.ca)
//
// Funding by:
//    NSERC/Canada Council for the Arts - New Media Initiative
//    Heritage Canada
//    Ministere du Developpement economique, de l'Innovation et de l'Exportation
//
// -----------------------------------------------------------------------------
//  This file is part of the SPIN Framework.
//
//  SPIN Framework is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SPIN Framework is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with SPIN Framework. If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#ifndef __MediaManager_H
#define __MediaManager_H

#include <string>

namespace spin
{

/**
 * \brief Provides an interface for finding media locations (models, textures,
 *        sounds, etc.)
 * 
 * The idea is that different mechanisms may exist for managing a project's
 * files. Examples include a networked media database, or direct access to the
 * filesystem. This class allows for a simple programmatic interface that is 
 * agnostic to the actual media storage method.
 */
class MediaManager
{
    public:
        
        MediaManager(const std::string &dataPath);
        ~MediaManager();
        
        std::string getDataPath() const { return dataPath; }
        
        std::string getImagePath(const std::string &s) const;
        std::string getModelPath(const std::string &s) const;
        std::string getSoundPath(const std::string &s) const;
        
        std::string getImagePath(int id) const;
        std::string getModelPath(int id) const;
        std::string getSoundPath(int id) const;
        
        std::string getImageName(int id) const;
        std::string getModelName(int id) const;
        std::string getSoundName(int id) const;

    private:
        std::string dataPath;
};

} // end of namespace spin

#endif /*MediaManager_H_*/
