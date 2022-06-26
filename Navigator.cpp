#include "provided.h"
#include <string>
#include <vector>
#include "MyMap.h"
#include <queue>
#include "support.h"
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    SegmentMapper segMap;
    AttractionMapper attracMap;
    string getStreetName(myGeoCoord first, myGeoCoord second) const;
    bool nextIsTurn(myGeoCoord first, myGeoCoord second, myGeoCoord third) const;
    string getDirectionForTurn(myGeoCoord g1, myGeoCoord g2, myGeoCoord g3) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    MapLoader ml;
    bool b = ml.load(mapFile);
    if (! b)
        return false;
    
    segMap.init(ml);
    attracMap.init(ml);
    
    return true; 
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    myGeoCoord c_start;//create all data members needed
    myGeoCoord c_end;
    MyMap<myGeoCoord,myGeoCoord> segmentsConnected;
    priority_queue<Node> possibleRoutes;
    
    
    attracMap.getGeoCoord(start, c_start.m_geocoord);
    attracMap.getGeoCoord(end, c_end.m_geocoord);
    GeoCoord record[1000];
    
    vector<StreetSegment> startingStreets = segMap.getSegments(c_start.m_geocoord);

    vector<StreetSegment> endingStreets = segMap.getSegments(c_end.m_geocoord);

    
    if(startingStreets.size()==0)//if cant find the source
    {
        return NAV_BAD_SOURCE;
    }
    if(endingStreets.size()==0)//if cant find the end destination
    {
        return NAV_BAD_DESTINATION;
    }
    
    bool found = false;
    
    Node startNode = Node(c_start.m_geocoord, c_end.m_geocoord, nullptr);
    possibleRoutes.push(startNode);
    
    while(!found)//loop through until the end point is found and added to the queue
    {        
        if(possibleRoutes.empty())
        {
            return NAV_NO_ROUTE;
        }
        
        Node cur = possibleRoutes.top();
        possibleRoutes.pop();
    
        for (int i = 0; i < endingStreets.size(); i++)//if you found the end coordinate then break the loop
        {
            if((cur.cur.m_geocoord.latitude == endingStreets[i].segment.end.latitude && cur.cur.m_geocoord.longitude == endingStreets[i].segment.end.longitude) ||
               (cur.cur.m_geocoord.latitude == endingStreets[i].segment.start.latitude && cur.cur.m_geocoord.longitude == endingStreets[i].segment.start.longitude))
            {
                segmentsConnected.associate(c_end, cur.cur);
                found = true;
                continue;
            }
        }
        
        vector<StreetSegment>  curStreet = segMap.getSegments(cur.cur.m_geocoord);
        
        for(int i = 0; i < curStreet.size(); i++)//add all street segments connected to the current geocoord to the queue
        {
            
            if(segmentsConnected.find(curStreet[i].segment.start) == nullptr)
            {
                Node startOfSeg = Node(curStreet[i].segment.start, c_end.m_geocoord, &cur);
                possibleRoutes.push(startOfSeg);
                segmentsConnected.associate(curStreet[i].segment.start, cur.cur);
            }
            if(segmentsConnected.find(curStreet[i].segment.end) == nullptr)
            {
                Node endOfSeg = Node(curStreet[i].segment.end, c_end.m_geocoord, &cur);
                possibleRoutes.push(endOfSeg);
                segmentsConnected.associate(curStreet[i].segment.end, cur.cur);
            }
        }
    }
    
    vector<myGeoCoord> trace;
    myGeoCoord cur = c_end;
    
    trace.push_back(c_end);//add the ending coordinate to the begining of the vector
    
    while(!(cur.m_geocoord.latitude == c_start.m_geocoord.latitude && cur.m_geocoord.longitude == c_start.m_geocoord.longitude))
    {//add each coordinate to the queue and find the coorinate that we came from before that
        const myGeoCoord* value = segmentsConnected.find(cur);
        trace.push_back(*value);
        cur = *value;
    }
    
    while(!directions.empty())//empty the directions vecotr
    {
        directions.erase(directions.begin());
    }
    
    for(int k = trace.size()-1; k>0 ; k--)//add to the directions vector each street segment and check if there is a turn made between segments and if there is then add a turn nav segment to the directions vector
    {
        NavSegment temp = NavSegment(getDirection(trace[k].m_geocoord, trace[k-1].m_geocoord), getStreetName(trace[k].m_geocoord, trace[k-1].m_geocoord), distanceEarthMiles(trace[k].m_geocoord, trace[k-1].m_geocoord), GeoSegment(trace[k].m_geocoord, trace[k-1].m_geocoord));
        directions.push_back(temp);
        
        if(k > 1 && nextIsTurn(trace[k], trace[k-1], trace[k-2]))
        {
            NavSegment turnTemp = NavSegment(getDirectionForTurn(trace[k], trace[k-1], trace[k-2]), getStreetName(trace[k-1], trace[k-2]));
            directions.push_back(turnTemp);
        }
    }
    return NAV_SUCCESS;
}


    
string NavigatorImpl::getStreetName(myGeoCoord first, myGeoCoord second) const //return the name of the street that the segment is on
{
    vector<StreetSegment> one = segMap.getSegments(first.m_geocoord);
    vector<StreetSegment> two = segMap.getSegments(second.m_geocoord);
    for (int k = 0; k < one.size(); k++)
    {
        for (int i = 0; i < two.size(); i++)
        {
            if (one[k].streetName == two[i].streetName)
                return one[k].streetName;
        }
    }
    return "name not found for street";
}


bool NavigatorImpl::nextIsTurn(myGeoCoord first, myGeoCoord second, myGeoCoord third) const//check if there is a turn before the next street segment
{
    if (getStreetName(first, second) == getStreetName(second, third))
        return false;
    return true;
}

string NavigatorImpl::getDirectionForTurn(myGeoCoord g1, myGeoCoord g2, myGeoCoord g3) const//check what direction you turned
{
    string dir;
    GeoSegment seg1(g1.m_geocoord, g2.m_geocoord);
    GeoSegment seg2(g2.m_geocoord, g3.m_geocoord);
    
    double angle;
    angle = angleBetween2Lines(seg1, seg2);
    
    if (angle >= 0 && angle < 180)
        dir = "left";
    if (angle >= 180 && angle <= 360)
        dir = "right";

    return dir;
}

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
