#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include "support.h"
#include "MyMap.h"
#include <functional>
#include <algorithm>
#include <iomanip>
using namespace std;

template <class T, class S, class C>  //to extract underlying vector (or other container) from priority queue
S& Container(priority_queue<T, S, C>& q) {
	struct HackedQueue : private priority_queue<T, S, C> {
		static S& Container(priority_queue<T, S, C>& q) {
			return q.*&HackedQueue::c;
		}
	};
	return HackedQueue::Container(q);
}


struct NavNode { //to aid A* algorithm and path reconstruction
	NavNode(StreetSegment s, GeoCoord end, bool sf, const NavNode* prev = nullptr) : sSegment(s), previous(prev), startFirst(sf) {
		if(sf) //constructor handles distance calculation from actual end of segment to final destination
			miles_to_goal = distanceEarthMiles(s.segment.end, end);
		else 
			miles_to_goal = distanceEarthMiles(s.segment.start, end);
		if (prev != nullptr) miles_travelled = prev->miles_travelled + distanceEarthMiles(prev->sSegment.segment.end, prev->sSegment.segment.start) ;
		else miles_travelled = 0;
	};
	StreetSegment sSegment;
	double miles_to_goal; //the admissible heuristic
	double miles_travelled;
	const NavNode* previous; //tracks path
	bool startFirst; //determines direction of traversing segment (start at start or end)

	friend bool operator<(const NavNode& lhs, const NavNode& rhs) 
	{ return lhs.miles_to_goal + distanceEarthMiles(lhs.sSegment.segment.start, lhs.sSegment.segment.end)  + lhs.miles_travelled > rhs.miles_to_goal + distanceEarthMiles(rhs.sSegment.segment.start, rhs.sSegment.segment.end) + rhs.miles_travelled; };
	//operations reversed so it becomes a minheap

	friend bool operator==(const NavNode& lhs, const NavNode& rhs) {
		return lhs.sSegment == rhs.sSegment;
	}


};

bool LessPointer( NavNode* lhs,  NavNode* rhs) //so MyMap works with pointers... need pointers cz otherwise it's value based, not reference based
{
	return *lhs < *rhs;
}


class NavigatorImpl //clean :)  
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;

private:
	MapLoader ml;
	AttractionMapper aMap;
	SegmentMapper sMap;
	
};



NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile) //also clean
{
	if (ml.load(mapFile) == false) return false;
	sMap.init(ml);
	aMap.init(ml);

	return true;  
}



NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const //not so clean
{
	GeoCoord sgc, egc;
	if (aMap.getGeoCoord(start, sgc) == false) return NAV_BAD_SOURCE;
	if (aMap.getGeoCoord(end, egc) == false) return NAV_BAD_DESTINATION;
	auto startSegs = sMap.getSegments(sgc);	//possible starts
	auto endSegs = sMap.getSegments(egc);	//possible ends
	auto startSeg = startSegs.front();	//picks just one; real program would probably check which road/segment the person actually starts on with GPS or smth
	auto endSeg = endSegs.front();	//same as ^

	//Data Structures
	vector<NavSegment>  results;	//temporarily store here and not immeadiately modify directions as we may find that path is non existent
	priority_queue<NavNode*, std::vector<NavNode*>, std::function<bool(NavNode*, NavNode*)>> pQueue(LessPointer); //heap of NavNodes to analyze
	vector<NavNode*> toBeDeleted; //anything popped from the pQueue comes here to be cleaned up later; has to be later as we don't want to delete
									//early and consequently not be able to trace our steps (as the NavNodes are deleted)
	MyMap<GeoSegment, int> closed; //tracks segments are already analyzed

	pQueue.push(new NavNode(startSeg, egc, true, nullptr)); //first node, default startFirst == true, will check near the end of this function


	//while we haven't arrived (or exhausted the connected segments), do A* search by inserting connected NavNode*s to a priority_queue
	while (!pQueue.empty() && !(arrived(pQueue.top()->sSegment, endSegs))) {
		NavNode* topp = pQueue.top();		//actual pointer to actual NavNode, for new NavNodes' previous
		NavNode top = *pQueue.top();	//copy/value
		const StreetSegment& currSeg = pQueue.top()->sSegment;

		//find things connected to current segment
		auto connectedSegs = sMap.getSegments(currSeg.segment.start);
		auto cs2 = sMap.getSegments(currSeg.segment.end);
		connectedSegs.insert(connectedSegs.end(), cs2.begin(), cs2.end());

		closed.associate(top.sSegment.segment, 0);	//top has now been analyzed
		toBeDeleted.push_back(topp);	//remember to delete it later; we can't use MyMap as it doesn't easily traverse all items
		pQueue.pop();	

		for (auto ss : connectedSegs)
			if (closed.find(ss.segment) == nullptr) {	//if the segment has not been analyzed
				bool startFirst = //does ss starts at start?
					(ss.segment.start == top.sSegment.segment.end || ss.segment.start == top.sSegment.segment.start); 
				pQueue.push(new NavNode(ss, egc, startFirst, topp));
			}
	}

	//check if we ran out of segments to analyze
	if (pQueue.empty()) return NAV_NO_ROUTE;

	const NavNode* top = pQueue.top();
	while (top->previous != nullptr) {
		//		test.push_back(top->sSegment.streetName);
		//		cout << top->sSegment.streetName << endl;
		//		cout << "x" << top->sSegment.streetName << top->sSegment.segment.start.latitude << " " << top->sSegment.segment.start.longitudeText << endl;
		if (top->sSegment.streetName == top->previous->sSegment.streetName) //proceed
		{
			double angle = angleOfLine(top->sSegment.segment); 
			if (!top->startFirst)	//if it goes the other way
				angle = (angle + 180);
			if (angle >= 360) angle -= 360; //mod! %!

			double distance = distanceEarthMiles(top->sSegment.segment.start, top->sSegment.segment.end);
			GeoSegment gs = top->sSegment.segment;
			if (!top->startFirst) {
				gs.end = top->sSegment.segment.start;
				gs.start = top->sSegment.segment.end;
			}
			results.push_back(NavSegment(directionOf(angle), top->sSegment.streetName, distance, gs));
		}
		else {	//turn AND proceed
			string direction;
			double angle = angleBetween2Lines(top->previous->sSegment.segment, top->sSegment.segment);
			if (top->previous->startFirst != top->startFirst) // if it moves the other way, make the angle go the other way
				angle = angle + 180;
			if (angle >= 360) angle -= 360;
			if (angle < 180) direction = "left";
			else direction = "right";
			NavSegment turn(direction, top->sSegment.streetName);

			//proceed
			angle = angleOfLine(top->sSegment.segment);
			if (!top->startFirst)
				angle = (angle + 180);
			if (angle >= 360) angle -= 360; //mod! %!

			double distance = distanceEarthMiles(top->sSegment.segment.start, top->sSegment.segment.end);
			GeoSegment gs = top->sSegment.segment;
			if (!top->startFirst) {	//if it goes the other way, switch start and end
				gs.end = top->sSegment.segment.start;
				gs.start = top->sSegment.segment.end;
			}
			
			results.push_back(turn);
			results.push_back(NavSegment(directionOf(angle), top->sSegment.streetName, distance, gs));
			
		}
		top = top->previous; //backtrack
	}

	//special case handling for only one or zero moves,   the else (if more than 1 move) corrects first move's orientation 
	if (results.size() == 0) {
		if (!(sgc == egc)) { //one proceed
			GeoSegment gs(sgc, egc);
			results.push_back(NavSegment(directionOf(angleOfLine(gs)), top->sSegment.streetName, distanceEarthMiles(sgc, egc), gs));
		}
	}
	else
	{
		auto& next = results.back(); //2nd move
		if (next.m_geoSegment.start == top->sSegment.segment.end); //true if first move is a lready in correct orientation; note top == first move
		else //1st move not correct orientation
		{
			if (next.m_direction == "right") next.m_direction = "left";
			else if (next.m_direction == "left")
				next.m_direction = "right";
			NavNode* modifyTop = const_cast<NavNode*>(top);
			modifyTop->startFirst = !top->startFirst;
		}

		double angle = angleOfLine(top->sSegment.segment);
		if (!top->startFirst)
			angle = (angle + 180);
		if (angle >= 360) angle -= 360; //mod! %!

		double distance = distanceEarthMiles(top->sSegment.segment.start, top->sSegment.segment.end);
		GeoSegment gs = top->sSegment.segment;
		if (!top->startFirst) {
			gs.end = top->sSegment.segment.start;
			gs.start = top->sSegment.segment.end;
		}
		results.push_back(NavSegment(directionOf(angle), top->sSegment.streetName, distance, gs));

	}

	//Handle endpoints to match sgc and egc; adjust end/start, distance, and direction
	//handle ending to end at egc 
	if (results.size() > 0) {
		results[0].m_geoSegment.end = egc;	//results[0] is end cz we *back*tracked
		results[0].m_distance = distanceEarthMiles(results[0].m_geoSegment.start, egc);	
		results[0].m_direction = directionOf(angleOfLine(results[0].m_geoSegment));
	}
	//handle start to start at sgc
	if (results.size() > 0) {
		results.back().m_geoSegment.start = sgc;
		results.back().m_distance = distanceEarthMiles(sgc, results.back().m_geoSegment.end);
		results.back().m_direction = directionOf(angleOfLine(results.back().m_geoSegment));
	}
	
	//modify directions
	std::reverse(results.begin(), results.end()); //segments were added last to first, so reverse

	//swap turns and proceeds (to correct for adding in wrong order); can't add in order earlier as my direction corrections require this ordering
	directions.clear();													//^because I'm bad at coding :'(
	for (size_t i = 0; i < results.size()-1; ++i) { //careful not to go out of bounds
		//if next is a turn add that first
		if (results[i + 1].m_command == NavSegment::TURN) {
			directions.push_back(results[i + 1]);
			directions.push_back(results[i]);
			++i;
		}
		else
			directions.push_back(results[i]);
	}
	directions.push_back(results.back()); //don't forget the last item

	//directions = results;
	
	//delete all NavNodes 
	vector<NavNode*>& leftInPQ = Container(pQueue);
	for (int i = 0; i < leftInPQ.size(); i++)
	{
		delete leftInPQ[i];
	}
	for (auto node : toBeDeleted) delete node;

	return NAV_SUCCESS;  // This compiles, but may not be correct
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

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
