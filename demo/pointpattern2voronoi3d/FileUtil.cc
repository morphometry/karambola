// FileUtil.cc
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

#include "FileUtil.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include "../../lib/Kmath.h"

#include <unistd.h> // for gettmp()


namespace FileUtil
{
  std::string path(std::string s)
  {
    size_t pathend=s.rfind("/");
    if (pathend==std::string::npos)
      return "";
    s.erase(pathend+1);
    return s;
  } 
  std::string namewoend(std::string s)
  {
    // cut everything up to the .
    size_t pathend=s.rfind("/");
    if (pathend!=std::string::npos)
      s.erase(0, pathend+1);
    size_t lastdot=s.rfind(".");
    if (lastdot!=std::string::npos)
      s.erase(lastdot);
    return s;
  }

  std::string ending(std::string s)
  {
    // cut everything up to the .
    size_t pathend=s.rfind("/");
    if (pathend!=std::string::npos)
      s.erase(0, pathend+1);
    size_t lastdot=s.rfind(".");
    if (lastdot==std::string::npos)
      return "";
    s.erase(0,lastdot+1);
    return s;
  }
  static unsigned int seedvar=0;

  void seed(unsigned int argc, char** argv)
  {
    seedvar=0;
    for (unsigned int arg=0; arg<argc; arg++)
      for (unsigned int i=0; argv[arg][i]; i++)
        seedvar+=argv[arg][i];
  }

  std::string mktemp(std::string base, unsigned int N)
  {
    std::string validchars="abcdefghijklmnopqrstuvwxyz0123456789";
    static bool initialized=false;
    if (!initialized)
    {
      unsigned int s=time(NULL)+clock()+seedvar;
      srand(s);
      initialized=true;
    }
    for (unsigned int i=0; i<N; i++)
      base+=validchars[rand()%validchars.length()];
    return base;
  }
  
  bool opensectemp(std::string& base, std::ofstream& os)
  {
    base += "XXXXXX";
    std::vector<char> dest(base.begin(), base.end());
    dest.push_back('\0');

    int fd = mkstemp(&dest[0]);
    if(fd==-1)
      return false;
    base.assign(dest.begin(), dest.end() - 1);
    os.open(base.c_str(), std::ios_base::trunc | std::ios_base::out);
    close(fd);
    return true;
  }
  
  bool remove(std::string filename)
  {
    if (!fileExists(filename))
      return false;
    if (unlink(filename.c_str())==0)
      return true;
    return false;
  }
  
  bool fileExists(std::string strFilename) {
    std::ifstream infile;
    infile.open(strFilename.c_str());
    bool fex=infile.good();
    infile.close();
    return fex;
  }
  
  inline int max(int a, int b)
  {
    return (a>b) ? a : b;
  }
 
  std::string num(unsigned int i, unsigned int n)
  {
    std::ostringstream os;
    unsigned int fillnr=(unsigned int)(max(0,(int)(log(n)/log(10))))+1;
    os << std::setfill('0') << std::setw(fillnr) << i;
    return os.str();
  }

  std::string gettmp()
  {
    char* TMP=getenv("TMP");
    char* TMPDIR=getenv("TMPDIR");
    if (!TMP  && !TMPDIR)
      return "/tmp";
    if (!TMP)
      return TMPDIR;
    return TMP;
  }

}

