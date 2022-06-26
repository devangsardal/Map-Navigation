#include "provided.h"
#include "MyMap.h"
#include <vector>
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const myGeoCoord& gc) const;
private:
    MyMap<myGeoCoord, vector<StreetSegment> > m_segmentMapper; // will be a string of the coordinates
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    int segmentsSize = ml.getNumSegments();
    for (int k = 0; k < segmentsSize; k++)
    {
        StreetSegment temp;
        ml.getSegment(k, temp); // get each segment
        
        myGeoCoord endCoord = temp.segment.end;
        myGeoCoord startCoord = temp.segment.start;
        vector<StreetSegment> b;
        vector<StreetSegment> vt;
        vector<StreetSegment> a;
        
        // attractions
        for (int i = 0; i < temp.attractions.size(); i++)
        {
            myGeoCoord attractionCoord = temp.attractions[i].geocoordinates;
            vector<StreetSegment> c;
            if (m_segmentMapper.find(attractionCoord) != nullptr)
            { // if it's not the first item
                c = *m_segmentMapper.find(attractionCoord);
                c.push_back(temp);
                m_segmentMapper.associate(attractionCoord, c);
            }
            else
            { // if it is the first item
                c.push_back(temp);
                m_segmentMapper.associate(attractionCoord, c);
            }
        }
        
        // start coordinates
        if (m_segmentMapper.find(startCoord) != nullptr)
        { // if it's not the first item
            a = *m_segmentMapper.find(startCoord);
            a.push_back(temp);
            m_segmentMapper.associate(startCoord, a);
        }
        else
        { // if it is the first item
            a.push_back(temp);
            m_segmentMapper.associate(startCoord, a);
        }
        
        // end coordinates
        if (m_segmentMapper.find(endCoord) != nullptr)
        { // if it's not the first item
            b = *m_segmentMapper.find(endCoord);
            b.push_back(temp);
            m_segmentMapper.associate(endCoord, b);
        }
        else
        { // if it is the first item
            b.push_back(temp);
            m_segmentMapper.associate(endCoord, b);
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const myGeoCoord& gc) const
{
    vector<StreetSegment> v;
    if(m_segmentMapper.find(gc)==nullptr)
    {
        return v;
    }
    else{
        return *m_segmentMapper.find(gc);
    }
    
}

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}

