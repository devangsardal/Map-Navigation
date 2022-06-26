#include "support.h"
#include <string>

using namespace std;

myGeoCoord::myGeoCoord(GeoCoord a)
{
    m_geocoord = a;
}

myGeoCoord::myGeoCoord()
{}

bool myGeoCoord::operator==(const myGeoCoord& other)
{
    if (m_geocoord.latitude == other.m_geocoord.latitude && m_geocoord.longitude == other.m_geocoord.longitude)
        return true;
    return false;
}
bool myGeoCoord::operator<(const myGeoCoord& other)
{
    if (m_geocoord.latitude < other.m_geocoord.latitude)
        return true;
    if (m_geocoord.longitude < other.m_geocoord.longitude)
        return true;
    return false;
}
bool myGeoCoord::operator>(const myGeoCoord& other)
{
    if (m_geocoord.latitude > other.m_geocoord.latitude)
        return true;
    if (m_geocoord.longitude > other.m_geocoord.longitude)
        return true;
    return false;
}

bool myGeoCoord::operator!=(const myGeoCoord& other)
{
    if (m_geocoord.latitude != other.m_geocoord.latitude || m_geocoord.longitude != other.m_geocoord.longitude)
        return true;
    return false;
}




string getDirection(const myGeoCoord first, const myGeoCoord second) //get the direction being traveled
{
    GeoSegment seg = GeoSegment(first.m_geocoord, second.m_geocoord);
    double angle = angleOfLine(seg);
    
    if(angle >= 0 && angle <=22.5)
        return "east";
    if(angle > 22.5 && angle <= 67.5)
        return "northeast";
    if(angle > 67.5 && angle <= 112.5)
        return "north";
    if(angle > 112.5 && angle <= 157.5)
        return "northwest";
    if(angle > 157.5 && angle <= 202.5)
        return "west";
    if(angle > 202.5 && angle <= 247.5)
        return "southwest";
    if(angle > 247.5 && angle <= 292.5)
        return "south";
    if(angle > 292.5 && angle <= 337.5)
        return "southeast";
    if(angle > 337.5 && angle <= 360)
        return "east";
    return "";
}

string directionOfLine(const GeoSegment& gs)//for the main routine testing provided
{
    return getDirection(gs.start, gs.end);
}

bool Node::operator<(const Node& node1) const //operator declared for priority queue
{
    return this->f > node1.f;
}




