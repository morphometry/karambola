// CommandLineParser.cc
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
    
#include "CommandLineParser.h"
#include <stdexcept>
#include <sstream>

const std::string tabstring="    ";

std::string strArgs(char shortoption, std::string option)
{
  std::ostringstream os;
  if (shortoption!=0)
    os << "-" << shortoption;
  if (shortoption!=0 && option.size()>0)
    os << " or ";
  if (option.size()>0)
    os << "--" << option;
  return os.str();
}


CommandLineParser::CommandLineParser()
{
  // std-option: help
  addDefaultArgument('h',"help",true,"0");
}
// ***** INPUT *****
// add Default Argument, if value="" it has no argument
bool CommandLineParser::addDefaultArgument(char shortoption, std::string option, bool isFlag, std::string value)
{
  unsigned int i;
  if (shortoption==0 && option.size()==0)
    throw std::logic_error("either shortoption or option should be dafined");
  for (i=0; i<config.size(); i++)
  {
    if ((shortoption!=0 && config[i].shortoption==shortoption)|| (option.length()>0 && config[i].option==option))
      break;
  } 
  if (i==config.size())
  {
    config.push_back(ConfigEntry(shortoption, option, isFlag, value));
    return true;
  }
  config[i]=ConfigEntry(shortoption, option, isFlag, value);
  return false;
}


void CommandLineParser::addExtraHelpInfo(std::string s)
{
  std::ostringstream os;
  os << extraHelpInfo;
  os << tabstring << s << std::endl;
  extraHelpInfo=os.str();
}


// parse Commandline Arguments -a --aa -noaa --aa b or -a b or --aa=b or -a=b or filename
void CommandLineParser::parseArgumentList(int argc_, char** argv_)
{
  if (argc_<1)
    throw std::logic_error("argc<1, fail!");
  programmfilename=argv_[0];
  unsigned int argc=argc_;
  char** argv=argv_;
  for (unsigned int i=1; i<argc; i++)
  {
    std::string arg=argv[i];
    if (arg[0]!='-')
    {
      fileList.push_back(arg);
      continue;
    }
    bool isTaken=false;
    if (arg.size()>1)
      for (unsigned int c=0; c<config.size() && !isTaken; c++)
      {
        // is it just a flag?
        if (config[c].isFlag)
        {
          // -a
          if (arg.size()==2 && config[c].shortoption==arg[1])
          {
            config[c].value="1";
            config[c].isDefault=false;
            isTaken=true;
          }
          // -noa
          else if (arg.size()==4 && arg[1]=='n' && arg[2]=='o' && config[c].shortoption==arg[3])
          {
            config[c].value="0";
            config[c].isDefault=false;
            isTaken=true;
          }
          // --aa
          else if (arg==std::string("--")+config[c].option)
          {
            config[c].value="1";
            config[c].isDefault=false;
            isTaken=true;
          }
          // --noaa
          else if (arg==std::string("--no")+config[c].option)
          {
            config[c].value="0";
            config[c].isDefault=false;
            isTaken=true;
          }
        } 
        // -a b || --aa b
        else if ((arg.size()==2 && config[c].shortoption==arg[1])
          || arg==std::string("--")+config[c].option )
        {
          if (i==argc-1) // has no Next
          {
            std::ostringstream os;
            os << "argument " << arg << " needs following option";
            throw std::runtime_error(os.str());
          }
            
          config[c].value=argv[i+1];
          config[c].isDefault=false;
          i++;
          isTaken=true;
        }
      }//for
    if (!isTaken)
    {
      std::ostringstream os;
      os << "No such argument: " << arg << std::endl;
      os << "please try argument -h for help" << std::endl;
      throw std::runtime_error(os.str());
    }
  }
  return;
}

// return true if -a or --aa or false if not given or -noaa
bool CommandLineParser::getBool(char shortoption, std::string option) const
{
  unsigned int c=getEntry(shortoption, option);
  if (!config[c].isFlag)
  {
    std::ostringstream os;
    os << "argument " << strArgs(shortoption, option) << "is no boolean";
    throw std::logic_error(os.str());
  }
  if (config[c].value=="1")
    return true;
  if (config[c].value=="0")
    return false;
    
  std::ostringstream os;
  os << "argument " << strArgs(shortoption, option) << "is no boolean";
  throw std::logic_error(os.str());
}
// following is -a bb or --aa bb
int CommandLineParser::getInt(char shortoption, std::string option) const
{
  unsigned int c=getEntry(shortoption, option);
  if (config[c].isFlag)
  {
    std::ostringstream os;
    os << "argument " << strArgs(shortoption, option) << "is boolean";
    throw std::logic_error(os.str());
  }
  std::istringstream is(config[c].value);
  int i;
  is >> i;
  return i;
}
double CommandLineParser::getDouble(char shortoption, std::string option) const
{
  unsigned int c=getEntry(shortoption, option);
  if (config[c].isFlag)
  {
    std::ostringstream os;
    os << "argument " << strArgs(shortoption, option) << "is boolean";
    throw std::logic_error(os.str());
  }
  std::istringstream is(config[c].value);
  double d;
  is >> d;
  return d;
}
std::string CommandLineParser::getString(char shortoption, std::string option) const
{
  unsigned int c=getEntry(shortoption, option);
  if (config[c].isFlag)
  {
    std::ostringstream os;
    os << "argument " << strArgs(shortoption, option) << "is boolean";
    throw std::logic_error(os.str());
  }
  return config[c].value;
}

bool CommandLineParser::isDefault(char shortoption, std::string option) const
{
  unsigned int i;
  for (i=0; i<config.size(); i++)
  {
    if ((shortoption!=0 && config[i].shortoption==shortoption) || (option.length()>0 && config[i].option==option))
      break;
  }
  if (i==config.size())
    throw std::logic_error(std::string("CommandLineParser: neither ")+shortoption+" nor "+option+" is a valid option");
  return config[i].isDefault;
}


// return ever argument a not being -a or -b a
std::vector<std::string> CommandLineParser::getFileList() const
{
  return fileList;
}

std::string fillw(std::string s, unsigned int length)
{
  while (s.size()<length)
    s+=" ";
  return s;
}

// ***** OTHER *****
// print all arguments for help, without any sense before addDefaultArgument was called
std::string CommandLineParser::getHelp() const
{
  std::ostringstream os;
  os << "usage: " << programmfilename << " { --argument 0 | --flag }" << std::endl;
  os << "note: flags are either set or not, arguments carry values" << std::endl;
  os << "flags:" << std::endl;
  unsigned int longestOptionSize=0;
  for (unsigned int c=0; c<config.size(); c++)
    if (longestOptionSize<config[c].option.size())
        longestOptionSize=config[c].option.size();
  for (unsigned int c=0; c<config.size(); c++)
  {
    if (!config[c].isFlag)
      continue;
    os << tabstring;
    if (config[c].shortoption!=0)
      os << "-" << config[c].shortoption
         << "/-no" << config[c].shortoption;
    os << " ";
    if (config[c].option.size()>0)
      os << "--" << fillw(config[c].option,longestOptionSize);
    os << " ";
    if (config[c].option.size()>0)
      os << "--no" << fillw(config[c].option,longestOptionSize);

    if (config[c].isDefault)
      os << "  per default: ";
    else
      os << "  currently set as: ";
    if (config[c].value=="0")
      os << "unset" << std::endl;
    else if (config[c].value=="1")
      os << "set" << std::endl;
    else
      os << "invalid" << std::endl;
  }
  
  os << "arguments:" << std::endl;
  for (unsigned int c=0; c<config.size(); c++)
  {
    if (config[c].isFlag)
      continue;
    os << tabstring;
    if (config[c].shortoption!=0)
      os << "-" << config[c].shortoption;
    os << " ";
    if (config[c].option.size()>0)
      os << "--" << fillw(config[c].option,longestOptionSize);
    
    if (config[c].isDefault)
      os << "  per default: ";
    else
      os << "  currently set as: ";

    os << config[c].value << std::endl;
  }
  if (fileList.size()>0)
  {
    os << "supplied no-arguments at this call:" << std::endl;
    for (unsigned int i=0; i<fileList.size(); i++)
    {
      os << tabstring << fileList[i] << std::endl;
    }
  }
  if (extraHelpInfo.size()>0)
  {
    os << "additional information:" << std::endl;
    os << extraHelpInfo;
  }
  return os.str();
}
// if -h or --help parameter was given process here:
bool CommandLineParser::processHelp(std::ostream& os) const
{
  if (getBool('h', "help"))
  {
    os << getHelp() << std::endl;
    return true;
  }
  return false;
}

unsigned int CommandLineParser::getEntry(char shortoption, std::string option) const
{
  for (unsigned int c=0; c<config.size(); c++)
  {
    if ((shortoption!=0 && config[c].shortoption==shortoption)
     || (option.size()>0 && config[c].option==option) )
       return c;
  }
  std::ostringstream os;
  os << "No such argument: ";
  if (shortoption!=0)
    os << "-" << shortoption;
  if (shortoption!=0 && option.size()>0)
    os << " or ";
  if (option.size()>0)
    os << "--" << option;
  throw std::logic_error(os.str());

}
