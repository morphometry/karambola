// CommentParser.h
//   parse comment of form "a=b, c=d, e=f"

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


#include <map>
#include <string>

class CommentParser
{
 public:
  CommentParser() {};
  CommentParser(std::string s) {parse(s);};
  // read in string, store internally
  void parse(std::string);  
  
  // get values with specific id
  std::string getString(std::string);
  double getDouble(std::string);
  int getInt(std::string);
  // see if value is defined or not
  bool isDefined(std::string id);
 private:
  std::map<std::string, std::string> values;
};
