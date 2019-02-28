// main.cc
//   main programm of PointPattern2Voronoi3d, calls everything...

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


#include <iostream>
#include "PointPattern2Voronoi3d.h"
#include "FileUtil.h"


int main(int argc, char**  argv)
{
  // handle parameters
  CommandLineParser cr;
  Pointpattern2Voronoi::addDefaultArgument(cr);

  // add parameters for main function
  cr.addDefaultArgument('i', "infile", false, "");
  cr.addDefaultArgument('o', "outfile", false, "");
  cr.addDefaultArgument(0, "outfile-saveLabels", false, "");
  cr.addDefaultArgument(0, "outfile-saveLabelsExtraInfo", false, "");

  cr.parseArgumentList(argc, argv);
  if (cr.processHelp(std::cerr))
    return 1;
  const bool isDebug=cr.getBool('D', "DEBUG");

  if (cr.getFileList().size()>0)
  {
    std::cerr << "Please use arguments -i for specifying infile and "
              << "-o for optionally specifying outfile" << std::endl;
    return 1;
  }
  
  if (isDebug)
    std::cerr << "DEBUG version" << std::endl;

  // inputfilename via -i / --infile
  std::string infilename=cr.getString('i', "--infile");
  if (infilename.size()==0)
  {
    std::cerr << "Please use argument -i for specifying infile" << std::endl;
    return -1;
  }
  
  // outputfilename via -o / --outfile or empty=add .poly to infile
  std::string outfilename=cr.getString('o', "--outfile");
  if (outfilename.size()==0)
  {
    outfilename=infilename;
    if (FileUtil::ending(outfilename)=="xyz")
      outfilename=outfilename.substr(0,outfilename.size()-4)+".poly";
    else
      outfilename+=".poly";
  }
  std::string seedfilename=outfilename+"-seeds.xyz";
  if (!cr.isDefault(0, "outfile-saveLabels"))
    seedfilename=cr.getString(0, "outfile-saveLabels");
  std::string seedeifilename=outfilename+"-seeds-ei.xyz";
  if (!cr.isDefault(0, "outfile-saveLabelsExtraInfo"))
    seedeifilename=cr.getString(0, "outfile-saveLabelsExtraInfo");
  
  
  {  
    Pointpattern2Voronoi ob;
    ob.readConfig(cr);

    std::cout << "processing \"" << infilename << "\" to \"" << outfilename << "\"...";
    std::cout.flush();
    {
      std::ifstream infile;
      infile.open(infilename.c_str());
      if (!infile.good())
      {
        std::cerr << "infile \"" << infilename << "\" could not be opened" << std::endl;
        return -1;
      }
      if (isDebug)
        std::cerr << "infile opened, loading sites" << std::endl;
      if (!ob.loadSites(infile))
      {
        std::cerr << "infile \"" << infilename << "\" could not be loaded" << std::endl;
        return -1;
      }
      infile.close();
    }


    if (!ob.createCopies())
    {
      std::cerr << "error creating copies" << std::endl;
      return -1;
    }
    if (isDebug)
      std::cerr << "processing Qhull" << std::endl;

    if (!ob.processQhull())
    {
      std::cerr << "error processing with qhull" << std::endl;
      return -1;
    }

    {  
      std::ofstream outfile;
      outfile.open(outfilename.c_str());
      if (!outfile.good())
      {
        std::cerr << "could not open " << outfilename << std::endl;
        return -1;
      }
      ob.writePoly(outfile);
      outfile.close();
    }
    
    if (ob.shouldSaveLabels())
    {
      std::ofstream outfile;
      outfile.open(seedfilename.c_str());
      if (!outfile.good())
      {
        std::cerr << "could not open " << seedfilename << std::endl;
        return -1;
      }
      ob.saveLabels(outfile);
      outfile.close();
    }
    if (ob.shouldSaveLabelsExtraInfo())
    {
      std::ofstream outfile;
      outfile.open(seedeifilename.c_str());
      if (!outfile.good())
      {
        std::cerr << "could not open " << seedeifilename << std::endl;
        return -1;
      }
      ob.saveLabelsExtraInfo(outfile);
      outfile.close();
    }
    //ob.writePolys(outfilename);
  }
  std::cout << "   done" << std::endl;
}
