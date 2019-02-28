// CommentParser.cc
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

#ifndef COMMENTPARSER_H
#define COMMENTPARSER_H
#include "CommentParser.h"

#include <sstream>
#include <stdexcept>

std::string trim(const std::string s)
{
  unsigned int begin=0;
  unsigned int end=s.length()-1;
  while ((s[begin]==' ' || s[begin]=='\t' || s[begin]=='\n') && begin<end)
    begin++;
  while ((s[end]==' ' || s[end]=='\t' || s[end]=='\n') && begin<end)
    end--;
  return s.substr(begin, end-begin+1);
}

void CommentParser::parse(std::string s) 
{
  size_t currentstart=0;
  while (true)
  {
    size_t currentend=s.find(',', currentstart);
    std::string temp;
    if (currentend!=std::string::npos)
      temp=s.substr(currentstart, currentend-currentstart);
    else
      temp=s.substr(currentstart);
    size_t delim=temp.find('=');
    if (delim==std::string::npos)
      throw std::runtime_error(std::string("CommentParser: unable to parse, invalid entry: ")+temp);
    std::string id=trim(temp.substr(0,delim));
    std::string value=trim(temp.substr(delim+1));
    if (values.find(id)!=values.end())
      throw std::runtime_error(std::string("CommentParser: ")+id+" defined twice");
    values[id]=value;
    if (currentend==std::string::npos)
      break;
    currentstart=currentend+1;
  }
}
std::string CommentParser::getString(std::string id)
{
  std::map<std::string,std::string>::iterator it=values.find(id);
  if (it==values.end())
    throw std::runtime_error(std::string("CommentParser: ")+id+" not found");
  return it->second;
}

double CommentParser::getDouble(std::string id)
{
  std::map<std::string,std::string>::iterator it=values.find(id);
  if (it==values.end())
    throw std::runtime_error(std::string("CommentParser: ")+id+" not found");
  double d;
  std::istringstream is(it->second);
  is >> d;
  return d;
}
int CommentParser::getInt(std::string id)
{
  std::map<std::string,std::string>::iterator it=values.find(id);
  if (it==values.end())
    throw std::runtime_error(std::string("CommentParser: ")+id+" not found");
  int i;
  std::istringstream is(it->second);
  is >> i;
  return i;
}

bool CommentParser::isDefined(std::string id)
{
  std::map<std::string,std::string>::iterator it=values.find(id);
  if (it==values.end())
    return false;
  return true;
}
#endif
