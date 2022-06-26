#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> mapOfAttractions;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for (int k = 0; k < ml.getNumSegments(); k++)//map each attraction to a geocroodinate
    {
        StreetSegment temp;
        ml.getSegment(k, temp);
        for (int i = 0; i < temp.attractions.size(); i++)
        {
            Attraction attract = temp.attractions[i];
            string s;
            for (int j = 0; j < attract.name.size(); j++)
            {
                s += tolower(attract.name[j]);
            }
            mapOfAttractions.associate(s, attract.geocoordinates);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string a;
    for (int j = 0; j < attraction.size(); j++)//lowercase to make easier to compare
    {
        a += tolower(attraction[j]);
    }
    const GeoCoord* value = mapOfAttractions.find(a);//find the geocoord and if its there return the value in the map
    if(value == nullptr)
    {
        return false;  
    }
    else
    {
        gc = *value;
        return true;
    }
}

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
