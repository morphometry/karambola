// CommandLineParser.h
//   read and parse argc/argv

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

#ifndef CONFIGREAD_H
#define CONFIGREAD_H
#include <ostream>
#include <vector>
#include <string>

class CommandLineParser
{
public:
  CommandLineParser();
  
  // add Default Argument, if value="" it has no argument
  bool addDefaultArgument(char shortoption, std::string option, bool isFlag, std::string value);
  void addExtraHelpInfo(std::string s);
  
  // parse Commandline Arguments -a --aa -noaa --aa b or -a b or --aa=b or -a=b or filename
  void parseArgumentList(int argc_, char** argv_);
  
  // return true if -a or --aa or false if not given or -noaa
  bool getBool(char shortoption, std::string option) const;
  
  // following is -a bb or --aa bb
  int getInt(char shortoption, std::string option) const;
  double getDouble(char shortoption, std::string option) const;
  std::string getString(char shortoption, std::string option) const;
  
  bool isDefault(char shortoption, std::string option) const;
  
  // return ever argument a not being -a or -b a
  std::vector<std::string> getFileList() const;
  
  // print all arguments for help, without any sense before addDefaultArgument was called
  std::string getHelp() const;

  // if -h or --help parameter was given process here:
  bool processHelp(std::ostream& os) const;


private:
  // ignore this:
  class ConfigEntry
  {
   public:
    ConfigEntry(char shortoption_=0, std::string option_="", bool isFlag_=true, std::string value_="", bool isDefault_=true)
     : shortoption(shortoption_), option(option_), isFlag(isFlag_), value(value_), isDefault(isDefault_){};
    char shortoption;
    std::string option;
    bool isFlag;
    std::string value;
    bool isDefault;
  };

  unsigned int getEntry(char shortoption, std::string option) const;

  std::string extraHelpInfo;
  std::vector<ConfigEntry> config;
  std::vector<std::string> fileList;
  std::string programmfilename;
};

#endif
