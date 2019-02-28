// PointPattern2Voronoi3d.h
//   constructs a voronoi tessellation out of xyz coordinates and stores to poly

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
#ifndef POINTPATTER2VORONOI_H
#define POINTPATTER2VORONOI_H

#include "CommandLineParser.h"
#include <vector>
#include <list>
#include <map>
//#include <blitz/tinyvec.h>
//#include <blitz/tinyvec-et.h>
#include "../../lib/Vector.h"

//typedef blitz::TinyVector<double,3> Vector;
#define DIM 3
#define DIMSTRING std::string("3")
const unsigned int outputPrecision=14;

class Pointpattern2Voronoi
{
public:
  Pointpattern2Voronoi();
  
  bool readConfig(const CommandLineParser& cr);
  static void addDefaultArgument(CommandLineParser& cr)
  {
    cr.addDefaultArgument('s', "shiftLabelToOrigin", true, "1");
    cr.addDefaultArgument('l', "testSiteLocations", true, "1");
    cr.addDefaultArgument('1', "writeFirstCellOnly", true, "0");
    cr.addDefaultArgument('t', "triangulateOutput", true, "0");
    cr.addDefaultArgument('D', "DEBUG", true, "0");
    cr.addDefaultArgument(0, "saveLabels", true, "0");
    cr.addDefaultArgument(0, "saveLabelsExtraInfo", true, "0"); 
    cr.addDefaultArgument('p', "padding", false, "0.1");
    cr.addDefaultArgument('m', "margin", false, "0.1");
    cr.addDefaultArgument('r', "margin_radius", false, "0.1");
    cr.addExtraHelpInfo("padding must be greater than 0, margin and margin_radius must lie within (0-0.5).");
    cr.addExtraHelpInfo("if no output filename is specified input filename is copied, if ending with .xyz .poly is substituted, appended otherwise");
  }
  bool loadSites(std::istream&);
  bool createCopies();
  bool processQhull();
  bool writePoly(std::ostream&);
  //bool writePoly(std::ostream&, unsigned int s);
  //bool writePolys(std::string outfilenamebase);
  
  unsigned int getNumSites();
  unsigned int getNumVertices();
  unsigned int getNumFaces();
  unsigned int getNumFaces(unsigned int siteNr);
  
  bool saveLabels(std::ostream& os);
  bool saveLabelsExtraInfo(std::ostream& os);
  bool shouldSaveLabels();
  bool shouldSaveLabelsExtraInfo();
  
  struct
  {
    bool shiftLabelToOrigin;
    bool testSiteLocations;
    bool triangulateOutput;
    bool DEBUG;
    bool saveLabels;
    bool saveLabelsExtraInfo;
    double padding; // ranging from 0 to 1
    bool paddingIsDefault;
    double margin; // ranging from 0 to 0.5
    double margin_radius;
    bool writeFirstCellOnly;
    bool marginIsDefault;
  } Config;
private:
  std::string writeQhullInputfile();
  std::string createSecureQhullOutputfile();
  //bool writePolyToStream(unsigned int sitenr, std::ostream& os);

  // is loaded after loadSites:
  std::vector<Vector> sites;
  std::vector<bool> labelIsInside;

  enum BoundaryCondition { bcUndef, bcPeriodicCuboidal, bcMinusSampleCuboidal, bcMinusSampleCylindrical};
  struct
  {
    BoundaryCondition bc;
    unsigned int seedsPerLabel;
    Vector boxSize;
    double cyl_height;
    double cyl_radius;
  } fileInfo;  
  
  // is loaded after processQhull
  std::vector<Vector> Vertex;
  std::vector<std::list<std::vector<unsigned int> > > labelFaceVertexMap;

  std::string tempfilebase;
};

#endif
