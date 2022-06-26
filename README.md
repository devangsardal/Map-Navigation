# Navigator

#### Build
* Download repository
* Available commands in directory:
```bash 
make 					// compile and link program
make clean-objects			// remove object files
make clean 				// remove object files and executable
```


---


#### Run
* After building program:
```bash
./navigation [mapfile] "start location" "end location" [-raw]
```
- mapfile:
	* file containing map data that the program will use
	* mapdata.txt is the file included in this repo used for this argument, it holds data for the Westwood, CA area
	* file format should match the following
		* Name of street, followed by '\n'
		* Coordinates (ex: 34.0550535, -118.4124960 34.0549537,-118.4123981), followed by '\n'
		* Number of  locations at these coordinates, followed by '\n'
		* Name of each location, with '\n' after each



- start/end location
	* name of start and end points selected from validlocs.txt file
	* file format should match the following
    	* Name of location, followed by " | " and the name of the street it is on
 	* provided validlocs file includes locations found in the Westwood, CA area



- raw
	* this optional flag will show all coordinates and segments of the trip, but with a less clean output 	



---



#### About
###### This system implements a shortest path algorithm that ensures the route returned is of the shortest distance possible

* How?
	* This program uses location data that essentially splits all streets into short segments, each with two endpoints specified by coordinates
	* It begins at the starting location and which segment it is on ("discovering" this segment), and then searches the segments connected to that one. 
	* It cycles through all "discovered" segments hoping to find a connected, "undiscovered" segment where the end location resides
	* This would normally essentially check every possible route until it finds one that leads to the destination, however, by using a priority queue, it is possible to check segments with certain attributes before others
	* The attribute used to prioritize which segment to look down is the one whose (total distance traveled) + (distance to destination) is the least
		* Because the destination has coordinates, we are always able to tell how far away a certain segment is from the final location (this is the distance in a straight line from the destination, the distance if traveling by street is most likely larger)
		* This is useful because the program will always search for a route from the segments which have the highest likelihood of finding the shortest route to the destination
		* When the segment where the destination resides is discovered, we know this is the shortest path because the segments that is checked is the one whose total distance traveled plus the distance to the destination is least, and if it is connected the destination segment, then it will have the shortest distance traveled, meaning it is the shortest path



** This is a very crude explanation, so if confused, check out this link on [Diijkstra's Shortest Path Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm), as this program implements a variation of this algorithm