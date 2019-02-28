// PointPattern2Voronoi3d.cc
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <queue>
#include "stdlib.h"

#include "FileUtil.h"
#include "PointPattern2Voronoi3d.h"
#include "CommentParser.h"

bool errorb(std::string what)
{
  std::cerr << what << std::endl;
  return false;
}

std::string dtos(const double& d)
{
  std::ostringstream os;
  os << d;
  return os.str();
}

inline unsigned int labelNumber(const unsigned int& siteNr, const unsigned int& seedsPerLabel)
{
  return static_cast<unsigned int>(static_cast<unsigned int>(siteNr)/seedsPerLabel);
}


Pointpattern2Voronoi::Pointpattern2Voronoi()
{
  tempfilebase=FileUtil::gettmp()+"/voronoitemp";
  fileInfo.bc=bcUndef;
}

bool Pointpattern2Voronoi::readConfig(const CommandLineParser& cr)
{
  Config.shiftLabelToOrigin=cr.getBool('s', "shiftLabelToOrigin");
  Config.testSiteLocations=cr.getBool('l', "testSiteLocations");
  Config.writeFirstCellOnly=cr.getBool('1', "writeFirstCellOnly");

  Config.triangulateOutput=cr.getBool('t', "triangulateOutput");
  Config.DEBUG=cr.getBool('D', "DEBUG");
  Config.saveLabels=cr.getBool(0, "saveLabels");
  if (!Config.saveLabelsExtraInfo && !cr.isDefault(0, "outfile-saveLabels"))
    Config.saveLabels=true;
  Config.saveLabelsExtraInfo=cr.getBool(0, "saveLabelsExtraInfo");
  if (!Config.saveLabelsExtraInfo && !cr.isDefault(0, "outfile-saveLabelsExtraInfo"))
    Config.saveLabelsExtraInfo=true;

  Config.padding=cr.getDouble('p', "padding");
  Config.paddingIsDefault=cr.isDefault('p', "padding");
  Config.margin=cr.getDouble('m', "margin");
  Config.marginIsDefault=cr.isDefault('m', "margin");
  if (cr.isDefault('r', "margin_radius"))
    Config.margin_radius=Config.margin;
  else
    Config.margin_radius=cr.getDouble('r', "margin_radius");


  if (Config.margin<0 || Config.margin>0.5)
    return errorb("Config.margin should be in the range of 0 to 0.5");
  if (Config.margin_radius<0 || Config.margin_radius>0.5)
    return errorb("Config.margin_radius should be in the range of 0 to 0.5");
  if (Config.padding<0 || Config.padding>1)
    return errorb("Config.padding should be in the range of 0 to 1");

  return true;
}

inline void createCopy(const Vector& v, std::queue<Vector>& siteCopies, const Vector& boxSize, const int x, const int y, const int z, const double padding)
{
  Vector n=v+Vector(x*boxSize[0],y*boxSize[1],z*boxSize[2]);
  for (unsigned int i=0; i<DIM; i++)
  {
    const double a=n[i]/boxSize[i];
    if (a<-padding || 1+padding<a)
      return;
    //std::cout << "fail: " << a << " " << b << " ";
  }
  siteCopies.push(n);
}
    
bool Pointpattern2Voronoi::loadSites(std::istream& is)
{
  Vector min; Vector max;
  for (unsigned int n=0; n<DIM; n++)
  {
    min[n]=INFINITY;
    max[n]=-INFINITY;
  }

  is.exceptions(std::ios::badbit | std::ios::failbit);
  // convert sites to qhull format
  try
  {
    // read sites
    unsigned int numsites;
    is >> numsites >> std::ws;
    if (Config.DEBUG)
      std::cerr << "reading xyz: " << numsites << " sites" << std::endl;
    {
      std::string comment;
      getline(is, comment);
      CommentParser cp;
      try
      {
        cp.parse(comment);
      }
      catch (std::runtime_error e)
      {
        return errorb(e.what());
      }

      if (!cp.isDefined("boundary_condition"))
        return errorb("boundary_condition must be defined in file comment");

      std::string bc;
      bc=cp.getString("boundary_condition");

      if (bc=="periodic_cuboidal")
        fileInfo.bc=bcPeriodicCuboidal;
      else if (bc=="minus_sample_cuboidal")
        fileInfo.bc=bcMinusSampleCuboidal;
      else if (bc=="minus_sample_cylindrical")
        fileInfo.bc=bcMinusSampleCylindrical;
      else
        return errorb("boundary_condition must either be periodic_cuboidal/minus_sample_cuboidal/minus_sample_cylindrical");

      if (cp.isDefined("seeds_per_label"))
        fileInfo.seedsPerLabel=cp.getInt("seeds_per_label");
      else
        fileInfo.seedsPerLabel=1;

      if (Config.DEBUG)
        std::cerr << "seedsPerLabel: " << fileInfo.seedsPerLabel << std::endl;

      if (fileInfo.seedsPerLabel!=1 && Config.shiftLabelToOrigin)
        throw std::runtime_error("Use --noshiftLabelToOrigin when seeds_per_label>1");

      if (fileInfo.bc==bcPeriodicCuboidal || fileInfo.bc==bcMinusSampleCuboidal)
      {
        const bool a=cp.isDefined("boxsz");
        const bool x=cp.isDefined("boxsz_x");
        const bool y=cp.isDefined("boxsz_y");
        const bool z=cp.isDefined("boxsz_z");
        const bool b=x && y && z;
        if ((a && b) || (!a && !b))
        {
          return errorb("either boxsz or the triple of boxsz_x to _z must be defined");
        }
        Vector v;
        if (a)
        {
          v[0]=cp.getDouble("boxsz");
          v[1]=cp.getDouble("boxsz");
          v[2]=cp.getDouble("boxsz");
        }
        else if (x && y && z)
        {
          v[0]=cp.getDouble("boxsz_x");
          v[1]=cp.getDouble("boxsz_y");
          v[2]=cp.getDouble("boxsz_z");
        }
        else
          throw std::logic_error("something is wrong(__FILE__, __LINE__)!");
          
        fileInfo.boxSize=v;
      } 
      else if (fileInfo.bc==bcMinusSampleCylindrical)
      {
        if (!cp.isDefined("cyl_height") || !cp.isDefined("cyl_radius"))
          return errorb("input file needs to defined cyl_height and cyl_radius");
        fileInfo.cyl_height=cp.getDouble("cyl_height");
        fileInfo.cyl_radius=cp.getDouble("cyl_radius");
      }
        
      if (Config.DEBUG)
      {
        if (fileInfo.bc==bcPeriodicCuboidal)
          std::cerr << "bc: bcPeriodicCuboidal" << std::endl
                    << "boxSize: " << fileInfo.boxSize << std::endl;
        else if (fileInfo.bc==bcMinusSampleCuboidal)
          std::cerr << "bc: bcMinusSampleCuboidal" << std::endl
                    << "boxSize: " << fileInfo.boxSize << std::endl;
        else if (fileInfo.bc==bcMinusSampleCylindrical)
          std::cerr << "bc: bcMinusSampleCylindrical" << std::endl;
        else if (fileInfo.bc==bcUndef)
          std::cerr << "bc: bcUndef" << std::endl;
      }
    }
    
    sites.resize(numsites);
    for (unsigned int s=0; s<sites.size(); s++)
    {
      std::string temp;
      getline(is, temp);
      std::istringstream is(temp);
      is.exceptions(std::ios::badbit | std::ios::failbit);
      std::string id;
      is >> std::ws >> id;
      // get some more flexible
      //if (c!='P')
      //  return errorb("in contrast to official specs only P is allowed for points!");
      for (unsigned int n=0; n<DIM; n++)
        is >> std::ws >> sites.at(s)[n];
      
      if (!Config.testSiteLocations)
        continue;
      if (fileInfo.bc==bcPeriodicCuboidal || fileInfo.bc==bcMinusSampleCuboidal)
        for (unsigned int n=0; n<DIM; n++)
          if (sites.at(s)[n]<0 || sites.at(s)[n]>fileInfo.boxSize[n])
            return errorb("points must lie inside the box specified by boxsz");
      if (fileInfo.bc==bcMinusSampleCylindrical)
      {
        if (sites.at(s)[2] >fileInfo.cyl_height || 0>sites.at(s)[2])
         {
           if (Config.DEBUG)
             std::cerr << "Point #" << s << ": " << sites[s] << std::endl;
          return errorb(std::string("points must lie inside the cylinder specified by cyl_height ")+dtos(sites[s][2])+std::string(" / ")+dtos(fileInfo.cyl_height));
          }
        const double rsq=sites.at(s)[0]*sites.at(s)[0]+sites.at(s)[1]*sites.at(s)[1];
        if (fileInfo.cyl_radius*fileInfo.cyl_radius<rsq)
        {
          if (Config.DEBUG)
            std::cerr << "Point #" << s << ": " << sites[s] << std::endl;
          return errorb(std::string("points must lie inside the cylinder specified by cyl_radius")+dtos(sqrt(rsq))+" / "+dtos(fileInfo.cyl_radius));
        }
      }
    }

  }
  catch (std::istream::failure e)
  {
    std::cerr << "Exception opening/reading inputfile \"" 
              << "\": " << e.what() << std::endl;
    return false;
  }
 
  return true; 
}
bool Pointpattern2Voronoi::createCopies()
{
  // should we calculate periodic or not??
  if (fileInfo.bc==bcUndef)
    throw std::logic_error("no information about boundary conditions found(__FILE__, __LINE__)!");
  else if (fileInfo.bc==bcMinusSampleCuboidal)
  {
    if (!Config.paddingIsDefault)
      errorb("Please do not supply padding parameter when using bondary_condition minus_sample_cuboidal");

    labelIsInside.resize(labelNumber(sites.size(), fileInfo.seedsPerLabel), true);
    
    if (Config.testSiteLocations)
    {
      for (unsigned int s=0; s<sites.size(); s++)
      {
        for (unsigned int n=0; n<DIM; n++)
          if (sites.at(s)[n] < Config.margin*fileInfo.boxSize[n] || (1-Config.margin)*fileInfo.boxSize[n]< sites.at(s)[n])
          {
            labelIsInside.at(labelNumber(s,fileInfo.seedsPerLabel))=false;
          }
      }
    }
  }
  else if (fileInfo.bc==bcPeriodicCuboidal)
  {
    if (!Config.marginIsDefault)
      errorb("Please do not supply margin parameter when using bondary_condition periodic_cuboidal");
    if (Config.saveLabels)
      errorb("saveLabels does not make sense with boundary_condition periodic_cuboidal");
    
    unsigned int realSize=sites.size();
    std::queue<Vector> siteCopies;
    
    //if (Config.DEBUG)
    //  std::cerr << "inserting some sites(try: " << realSize << ", " << Config.padding << ")" << std::endl;
    for (unsigned int i=0; i<realSize; i++)
    {
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1, -1, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1,  0, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1, +1, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0, -1, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0,  0, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0, +1, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1, -1, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1,  0, +1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1, +1, +1, Config.padding);
      
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1, -1,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1,  0,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1, +1,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0, -1,  0, Config.padding);
      //createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0,  0,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0, +1,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1, -1,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1,  0,  0, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1, +1,  0, Config.padding);
      
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1, -1, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1,  0, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, -1, +1, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0, -1, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0,  0, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize,  0, +1, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1, -1, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1,  0, -1, Config.padding);
      createCopy(sites.at(i), siteCopies, fileInfo.boxSize, +1, +1, -1, Config.padding);
    }
    
    sites.reserve(realSize+siteCopies.size());
    labelIsInside.resize(labelNumber(realSize+siteCopies.size(), fileInfo.seedsPerLabel), true);

    while (!siteCopies.empty())
    {
      sites.push_back(siteCopies.front());
      siteCopies.pop();
      labelIsInside.at(labelNumber(sites.size()-1, fileInfo.seedsPerLabel))=false; // note that site is virtual
    }
    if (Config.DEBUG)
      std::cerr << "virtual sites inserted: " << sites.size()-realSize << std::endl;

  }
  else if (fileInfo.bc==bcMinusSampleCylindrical)
  { 
    if (!Config.paddingIsDefault)
      errorb("Please do not supply padding parameter when using bondary_condition minus_sample_cylindrical");

    labelIsInside.resize(labelNumber(sites.size(), fileInfo.seedsPerLabel), true);

    if (Config.testSiteLocations)
    {
      unsigned int nOfVirtuals=0;
      for (unsigned int s=0; s<sites.size(); s++)
      {
        if (sites.at(s)[2] < Config.margin*fileInfo.cyl_height || (1-Config.margin)*fileInfo.cyl_height < sites.at(s)[2])
        {
          labelIsInside.at(labelNumber(s, fileInfo.seedsPerLabel))=false;
          nOfVirtuals++;
          continue;
        }
        const double rsq=sites.at(s)[0]*sites.at(s)[0]+sites.at(s)[1]*sites.at(s)[1];
        if ((1-Config.margin_radius)*fileInfo.cyl_radius*(1-Config.margin_radius)*fileInfo.cyl_radius < rsq)
        {
          labelIsInside.at(labelNumber(s, fileInfo.seedsPerLabel))=false;
          nOfVirtuals++;
        }
      }
      if (Config.DEBUG)
        std::cerr << "virtual sites: " << nOfVirtuals << " / " << sites.size() << " = " << 100.0*nOfVirtuals/sites.size() << "%" << std::endl;
    }
  }
  else
    throw std::logic_error("unknown boundaryCondition defined");
  return true;

}

bool Pointpattern2Voronoi::processQhull()
{
  if (sites.size()==0)
    return errorb("Please load sites first...");
  
  std::string qhullInFilename=writeQhullInputfile();
  std::string qhullOutFilename=createSecureQhullOutputfile();    
  // run qhull  
  {
    std::string qVoronoiCommand=std::string("cat ")+qhullInFilename
                +" | qvoronoi p Fv > "+qhullOutFilename;
    if (Config.DEBUG)
      std::cerr << "calling qhull: \""<< qVoronoiCommand << "\"" << std::endl;
    if (system(qVoronoiCommand.c_str()) != 0)
      return errorb("Error calling qvoronoi");
  }
  
  // convert sites to poly format
  {
    if (Config.DEBUG)
      std::cerr << "reading qhull output" << std::endl;  

    std::ifstream infile;
    // catch all errors
    std::cin.exceptions (std::ios::failbit | std::ios::badbit | std::ios::eofbit);
    infile.open(qhullOutFilename.c_str());
    
    unsigned int dimensions;
    unsigned int countVertices;
    infile >> dimensions >> std::ws >> countVertices;
    if (dimensions!=DIM)
      return errorb("dimension error in reading from qhull");
      
    countVertices++; // vertex[0] is infinity
    
    Vertex.resize(countVertices);
    std::vector<bool> isVertexInside;    
    isVertexInside.resize(countVertices, true);

    // insert vertex[0] as infinity
    for (unsigned int n=0; n<DIM; n++)
      Vertex.at(0)[n]=INFINITY;
    isVertexInside.at(0)=false;
    
    if (Config.DEBUG)
      std::cerr << "reading " << countVertices-1 << " vertices" << std::endl;  


    if (fileInfo.bc == bcMinusSampleCuboidal)
    {
      for (unsigned int p=1; p<countVertices; p++)
      {
        for (unsigned int n=0; n<DIM; n++)
        {
          infile >> std::ws >> Vertex.at(p)[n];
          // Cut out all outer parts
          if (Vertex.at(p)[n]<0 || Vertex.at(p)[n]>fileInfo.boxSize[n])
            isVertexInside[p]=false;
        }
      }
    }
    else if (fileInfo.bc == bcMinusSampleCylindrical)
    {
      for (unsigned int p=1; p<countVertices; p++)
      {
        for (unsigned int n=0; n<DIM; n++)
        {
          infile >> std::ws >> Vertex.at(p)[n];
        }
          // Cut out all outer parts
        if (Vertex.at(p)[2]<0 || Vertex.at(p)[2]>fileInfo.cyl_height)
        {
          isVertexInside[p]=false;
          continue;
        }
        double rsq=Vertex.at(p)[0]*Vertex.at(p)[0]+Vertex.at(p)[1]*Vertex.at(p)[1];
        if (rsq>fileInfo.cyl_radius*fileInfo.cyl_radius)
          isVertexInside[p]=false;
      }
    }
    else if (fileInfo.bc == bcPeriodicCuboidal)
    {
      for (unsigned int p=1; p<countVertices; p++)
      {
        for (unsigned int n=0; n<DIM; n++)
        {
          infile >> std::ws >> Vertex.at(p)[n];
          // Cut out all outer parts
          if (Vertex.at(p)[n]<-Config.padding*fileInfo.boxSize[n] ||
              Vertex.at(p)[n]>(1+Config.padding)*fileInfo.boxSize[n])
            isVertexInside[p]=false;
        }
      }
    }
    unsigned int countFaces;
    infile >> std::ws >> countFaces >> std::ws;
    labelFaceVertexMap.resize(labelNumber(sites.size()+1, fileInfo.seedsPerLabel));
        
    if (Config.DEBUG)
      std::cerr << "reading " << countFaces << " faces" << std::endl;
      
    for (unsigned int f=0; f<countFaces; f++)
    {
      unsigned int numVertices;
      unsigned int siteNr1;
      unsigned int siteNr2;
      std::string temp;
      getline(infile, temp);
      std::istringstream is(temp);
      is.exceptions(std::ios::failbit);
      is >> std::ws >> numVertices ;
      is >> std::ws >> siteNr1;
      is >> std::ws >> siteNr2;
      if (siteNr1 >= sites.size() || siteNr2 >= sites.size())
        throw std::logic_error("this is not a site(__FILE__, __LINE__)");

      const unsigned int labelNumber1=labelNumber(siteNr1,fileInfo.seedsPerLabel);
      const unsigned int labelNumber2=labelNumber(siteNr2,fileInfo.seedsPerLabel);
      if (labelNumber1==labelNumber2)
      {
        // both virtual points are of the same real site
        continue;
      }
      if (Config.writeFirstCellOnly)
        if (labelNumber1!=0 && labelNumber2!=0)
          continue;


      std::vector<unsigned int> vertices;
      vertices.resize(numVertices-2);
      bool isInside=true;
      for (unsigned int v=0; v<vertices.size(); v++)
      {
        unsigned int vnum;
        is >> std::ws >> vnum;
        vertices.at(v)=vnum;
        if (vnum>=countVertices)
          throw std::logic_error("vertex number out of range(__FILE__, __LINE__)");        
        if (!isVertexInside.at(vnum))
          isInside=false;
      }
      if (!isInside)
      {
        if (labelIsInside.at(labelNumber1) || labelIsInside.at(labelNumber2))
        {
          if (!Config.testSiteLocations)
            labelIsInside.at(labelNumber1)=labelIsInside.at(labelNumber2)=false;
          else if (fileInfo.bc==bcMinusSampleCuboidal)
            throw std::runtime_error("margin too low");        
          else if (fileInfo.bc==bcMinusSampleCylindrical)
            throw std::runtime_error("margin or margin_radius too low");        
          else if (fileInfo.bc==bcPeriodicCuboidal)
            throw std::runtime_error("padding too low");        
          else
            throw std::logic_error("unknown boundaryCondition defined");
        }
      }
      else
      {
        // Fix for 1d faces:
        if (numVertices==4)
        {
          std::cerr << "Warning: face with 2 vertices between " << siteNr1 << " and " << siteNr2 << std::endl;
        }
        else
        {
          // Align faces such that normals point 
          Vector mid1=(Vertex.at(vertices.at(0))+Vertex.at(vertices.at(1))+Vertex.at(vertices.at(2)))/3-sites.at(siteNr1);
//          Vector mid2=(Vertex.at(vertices.at(0))+Vertex.at(vertices.at(1))+Vertex.at(vertices.at(2)))/3-sites.at(siteNr2);
          Vector d1=Vertex.at(vertices.at(1))-Vertex.at(vertices.at(0));
          Vector d2=Vertex.at(vertices.at(2))-Vertex.at(vertices.at(0));
          Vector cr=cross_product(d1,d2);
          if (dot(mid1, cr)>0)
          {
            labelFaceVertexMap.at(labelNumber1).push_back(vertices);
            for (unsigned int i=0; i<vertices.size()>>1; i++)
            {
              unsigned int temp=vertices[i];
              vertices[i]=vertices[vertices.size()-i-1];
              vertices[vertices.size()-i-1]=temp;
            }
            labelFaceVertexMap.at(labelNumber2).push_back(vertices);
          }
          else
          {
            labelFaceVertexMap.at(labelNumber2).push_back(vertices);
            for (unsigned int i=0; i<vertices.size()>>1; i++)
            {
              unsigned int temp=vertices[i];
              vertices[i]=vertices[vertices.size()-i-1];
              vertices[vertices.size()-i-1]=temp;
            }
            labelFaceVertexMap.at(labelNumber1).push_back(vertices);
          }
        }
      }
    }
    infile.close();
  }
  if (!Config.DEBUG)
  {
    FileUtil::remove(qhullInFilename);
    FileUtil::remove(qhullOutFilename);
  }

  if (Config.DEBUG)
    std::cerr << "finished reading" << std::endl;

  return true;
}

bool Pointpattern2Voronoi::writePoly(std::ostream& os)
{
  os.exceptions(std::ios::badbit | std::ios::failbit);
  std::vector<std::map<unsigned int, unsigned int> > pointMap;
  pointMap.resize(sites.size());
 
  if (Config.DEBUG)
    std::cerr << "writing points" << std::endl;
  os << "POINTS" << std::endl;
  unsigned int currentPoint=0;
  os.precision(outputPrecision);
  unsigned int lmax=labelIsInside.size();
  if (Config.writeFirstCellOnly)
    lmax=1;
  for (unsigned int l=0; l<lmax; l++)
  {
    if (labelIsInside.at(l))
    {
      for (std::list<std::vector<unsigned int> >::iterator it=labelFaceVertexMap.at(l).begin();
          it!=labelFaceVertexMap.at(l).end(); it++)
      {
      for (unsigned int v=0; v<it->size(); v++)
      {
        const unsigned int currentRealPoint=it->at(v);
        std::map<unsigned int, unsigned int>::iterator pos;
        pos=pointMap.at(l).find(currentRealPoint);
        if (pos==pointMap.at(l).end()) // point not found
        {
          currentPoint++;
          os << currentPoint << ":";\

          Vector vec;
          if (Config.shiftLabelToOrigin)
            vec=Vertex.at(currentRealPoint)-sites.at(l);
          else
            vec=Vertex.at(currentRealPoint);
          
          for (unsigned int n=0; n<DIM; n++)
            os << " " << vec[n];
          os << std::endl;
          pointMap.at(l).insert(std::pair<unsigned int, unsigned int>(currentRealPoint,currentPoint));
        }
      }
      }
    }
  }
  if (Config.DEBUG)
    std::cerr << currentPoint << " points written" << std::endl;
  if (Config.DEBUG)
    std::cerr << "writing polys" << std::endl;
  os << "POLYS" << std::endl;
  unsigned int currentFace=0;
  unsigned int currentSite=0;
  for (unsigned int l=0; l<lmax; l++)
  {
    if (labelIsInside.at(l))
    {
      currentSite++;
      for (std::list<std::vector<unsigned int> >::iterator it=labelFaceVertexMap.at(l).begin();
          it!=labelFaceVertexMap.at(l).end(); it++)
      {
        if (Config.triangulateOutput)
        {
          for (unsigned int v=1; v<it->size()-1; v++)
          {
            currentFace++;
            os << currentFace << ": ";
            os << pointMap.at(l).at(it->at(0)) << " ";
            os << pointMap.at(l).at(it->at(v)) << " ";
            os << pointMap.at(l).at(it->at(v+1)) << " ";
            os << "< c(0, 0.9, 0, " << currentSite <<  ")" << std::endl;
#if 0
            {
              Vector mid=(Vertex.at(it->at(0))+Vertex.at(it->at(v))+Vertex.at(it->at(v+1)))/3-sites.at(l);
              Vector d1=Vertex.at(it->at(1))-Vertex.at(it->at(0));
              Vector d2=Vertex.at(it->at(2))-Vertex.at(it->at(0));
              if (dot(cross(d1,d2),mid)<0)
                std::cerr << "error" << std::endl;
            }
#endif
          }
        }
        else
        {
          currentFace++;
          os << currentFace << ": ";
          for (unsigned int v=0; v<it->size(); v++)
          {
            os << pointMap.at(l).at(it->at(v)) << " ";
          }
          os << "< c(0, 0, 0, " << currentSite << ")" << std::endl;
        }
      }
    }
  }
  os << "END" << std::endl;
  if (Config.DEBUG)
    std::cerr << currentFace << " faces written" << std::endl;
  if (Config.DEBUG)
    std::cerr << "done" << std::endl;
  return true;
}


unsigned int Pointpattern2Voronoi::getNumSites()
{
  return sites.size();
}
unsigned int Pointpattern2Voronoi::getNumVertices()
{
  return Vertex.size();
}
unsigned int Pointpattern2Voronoi::getNumFaces()
{
  unsigned int a=0;
  for (unsigned int i=0; i<sites.size(); i++)
    a+=getNumFaces(i);
  return a;
}
unsigned int Pointpattern2Voronoi::getNumFaces(unsigned int siteNr)
{
  if (siteNr<sites.size())
    return labelFaceVertexMap.at(siteNr).size();
  std::cerr << "this site does not exist(" << siteNr << "/" << sites.size() << std::endl;
  return (unsigned int)(-1);
}

bool Pointpattern2Voronoi::shouldSaveLabels()
{
  return Config.saveLabels;
}
bool Pointpattern2Voronoi::saveLabels(std::ostream& os)
{
  // FIXME: ajust for MinusSampleCylindrical here
  if (!Config.saveLabels)
    throw std::logic_error("please check shouldSaveLabels() before saveLabels is called");
  
  unsigned int currentSite=0;
  for (unsigned int s=0; s<sites.size(); s++)
    if (labelIsInside.at(s))
      currentSite++;
  os << currentSite << std::endl;  
  
  os << "boundary_condition=minus_sample_cuboidal "
     << "boxsz_x=" << fileInfo.boxSize[0] << ", "
     << "boxsz_y=" << fileInfo.boxSize[1] << ", "
     << "boxsz_z=" << fileInfo.boxSize[2] << std::endl;
  
  currentSite=0;
  for (unsigned int s=0; s<sites.size(); s++)
  {
    if (labelIsInside.at(s))
    {
      currentSite++;
      os << "P ";
      for (unsigned int n=0; n<DIM; n++)
        os << sites.at(s)[n] << " ";
      os << currentSite << std::endl;
    }
  }
  return true;
}
bool Pointpattern2Voronoi::shouldSaveLabelsExtraInfo()
{
  return Config.saveLabelsExtraInfo;
}
bool Pointpattern2Voronoi::saveLabelsExtraInfo(std::ostream& os)
{
  // FIXME: ajust for MinusSampleCylindrical here
  if (!Config.saveLabelsExtraInfo)
    throw std::logic_error("please check shouldSaveLabelsExtraInfo() before saveLabelsExtraInfo is called");
  
  unsigned int currentSite=0;
  for (unsigned int s=0; s<sites.size(); s++)
    if (labelIsInside.at(s))
      currentSite++;
  os << currentSite << std::endl;  
  
  if (fileInfo.bc==bcMinusSampleCuboidal)
  {
    os << "boundary_condition=minus_sample_cuboidal ";
    os << "boxsz_x=" << fileInfo.boxSize[0] << ", "
       << "boxsz_y=" << fileInfo.boxSize[1] << ", "
       << "boxsz_z=" << fileInfo.boxSize[2] << std::endl;
  }
  else if (fileInfo.bc==bcPeriodicCuboidal)
  {
    os << "boundary_condition=periodic_cuboidal ";
    os << "boxsz_x=" << fileInfo.boxSize[0] << ", "
       << "boxsz_y=" << fileInfo.boxSize[1] << ", "
       << "boxsz_z=" << fileInfo.boxSize[2] << std::endl;
  }
  else if (fileInfo.bc==bcMinusSampleCylindrical)
  {
    os << "boundary_condition=minus_sample_cylindrical ";
    os << "cyl_height=" << fileInfo.cyl_height << ", "
       << "cyl_radius=" << fileInfo.cyl_radius << std::endl;
  }
  else
    throw std::runtime_error("invalid boundary condition for saveLabelsExtraInfo");
  
  currentSite=0;
  for (unsigned int s=0; s<sites.size(); s++)
  {
    if (labelIsInside.at(s))
    {
      currentSite++;
      os << "P ";
      for (unsigned int n=0; n<DIM; n++)
        os << sites.at(s)[n] << " ";
      os << currentSite << " "
         << labelFaceVertexMap[s].size() << std::endl;
    }
  }
  return true;
}

std::string Pointpattern2Voronoi::writeQhullInputfile()
{
  if (Config.DEBUG)
    std::cerr << "writing qhullout..." << std::endl;
  std::string tempinfile=tempfilebase;
  std::ofstream outfile;
  if (!FileUtil::opensectemp(tempinfile, outfile) || !outfile.good())
  {
    errorb(std::string("We cannot open \"") + tempinfile
                             + std::string("\" for output"));
    return "";
  }
  if (Config.DEBUG)
    std::cerr << "outfile \"" << tempinfile << "\" is opened..." << std::endl;
                            
  outfile << DIMSTRING << std::endl;
  outfile << sites.size() << std::endl;
  outfile.precision(outputPrecision);
  for (unsigned int s=0; s<sites.size(); s++)
  {
    outfile << sites.at(s)[0];
    for (unsigned int n=1; n<DIM; n++)
      outfile << " " << sites.at(s)[n];
    outfile << std::endl;
  }
  outfile.close();
  return tempinfile;
}

std::string Pointpattern2Voronoi::createSecureQhullOutputfile()
{
  if (Config.DEBUG)
    std::cerr << "creating tempfile for qhull output..." << std::endl;
  std::ofstream toutfile;
  std::string qhullOutFilename=tempfilebase;
  if (!FileUtil::opensectemp(qhullOutFilename, toutfile) || !toutfile.good())
  {
     errorb(std::string("We cannot open \"") + qhullOutFilename
                             + "\" for output");
    return "";
  }
  toutfile.close();
  if (Config.DEBUG)
    std::cerr << "outfile \"" << qhullOutFilename << "\" was created..." << std::endl;
  return qhullOutFilename;
}

