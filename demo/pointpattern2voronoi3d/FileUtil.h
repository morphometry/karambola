// FileUtil.h
//   some commonly used functions for file interactions (tested only for linux)

/*  Copyright (C):
    2010  Matthias Hoffmann <Matthias.Hoffmann@physik.uni-erlangen.de>
 
    This file is part of PointPattern2Voronoi3d.

    PointPattern2Voronoi3d is free software: you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as published 
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PointPattern2Voronoi3d is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along 
    with PointPattern2Voronoi3d.  If not, see <http://www.gnu.org/licenses/>.   */
    
#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <fstream>
namespace FileUtil
{
  std::string path(std::string s);
  std::string namewoend(std::string s);
  std::string ending(std::string s);
  void seed(unsigned int argc, char** argv);
  bool opensectemp(std::string& base, std::ofstream& os);
  bool remove(std::string filename);
  std::string mktemp(std::string base, unsigned int N=10);
  bool fileExists(std::string);\
  std::string num(unsigned int i, unsigned int n);
  std::string gettmp();
}
#endif
