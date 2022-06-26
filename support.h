#ifndef support_h
#define support_h

#include "provided.h"
#include <string>
using namespace std;

class myGeoCoord
{
public:
    myGeoCoord(GeoCoord a);
    myGeoCoord();
    bool operator==(const myGeoCoord& other);
    bool operator<(const myGeoCoord& other);
    bool operator>(const myGeoCoord& other);
    bool operator!=(const myGeoCoord& other);
    
    GeoCoord m_geocoord;
private:
};

struct Node
{
public:
    Node();
    Node(GeoCoord current, GeoCoord end, Node* parent)
    {
        cur = current;
        h = distanceEarthMiles(current, end);
        if (parent == nullptr)
            g = 0;
        else
            g = distanceEarthMiles(current, parent->cur.m_geocoord) + parent -> g;
        f = g+h;
        m_parent = parent;
    }
    Node* m_parent;
    double g;
    double h;
    double f;
    myGeoCoord cur;
    bool operator<(const Node&) const;
};

string getDirection(const myGeoCoord first, const myGeoCoord second);
string directionOfLine(const GeoSegment& gs);


#endif 
