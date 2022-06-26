#include "provided.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> streetSegmentsfromFile;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
    if (! infile)
    {
        return false;
    }
    string s;
    while (getline(infile, s))//loop through until end of file
    {
        //adding name to the streetsegment
        StreetSegment seg;
        seg.streetName = s;

        //adding the coordinates to the streetsegment
        string lat1, lat2, lon1, lon2;//make vairables for latitude and longitiude
        getline(infile,lat1,',');
        
        char d;     //there may or may not be a space inb between the lat and long so deal with it
        infile.get(d);
        if(d != ' ')
        {
            getline(infile, lon1, ' ');
            lon1 = d + lon1;
        }
        else
        {
            getline(infile,lon1,' ');
        }
        
        
        getline(infile,lat2,',');//there may or may not be a space inb between the lat and long so deal with it
        char e;
        infile.get(e);
        if(e != ' ')
        {
            getline(infile, lon2, '\n');
            lon2 = e + lon2;
        }
        else
        {
            getline(infile,lon2,'\n');
        }
        
        
        
        GeoCoord start = GeoCoord(lat1, lon1);
        GeoCoord end = GeoCoord(lat2, lon2);
        GeoSegment geoSeg = GeoSegment(start, end);
        seg.segment = geoSeg;
        
        //adding the attractions to the street segment
        int k;
        infile >> k;
        infile.ignore(10000, '\n');
        for (int i = 0; i < k; i++)
        {
            string attracName, lat, lon;
            getline(infile, attracName, '|');
            getline(infile, lat, ',');
            
            char c;//there may or may not be a space inb between the lat and long so deal with it
            infile.get(c);
            if(c != ' ')
            {
                getline(infile, lon, '\n');
                lon = c + lon;
            }
            else
            {
                getline(infile, lon, '\n');
            }
            
            Attraction A;
            A.name = attracName;
            GeoCoord G = GeoCoord(lat, lon);
            A.geocoordinates = G;
            seg.attractions.push_back(A);
        }
        streetSegmentsfromFile.push_back(seg);
    }
    
    return true;  
}

size_t MapLoaderImpl::getNumSegments() const
{
	return streetSegmentsfromFile.size(); 
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum > streetSegmentsfromFile.size() - 1)
        return false;  
    
    seg = streetSegmentsfromFile[segNum];
    return true;
}

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
