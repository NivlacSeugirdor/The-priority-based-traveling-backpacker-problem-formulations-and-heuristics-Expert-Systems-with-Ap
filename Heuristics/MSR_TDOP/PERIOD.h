#ifndef _PERIOD_H_
#define _PERIOD_H_

#include <iostream>
#include <ctime>
#include <string>
#include <stdlib.h> 
#include <math.h> 
#include <vector>
#include <algorithm>
#include "AIRPORT.h"
#include "FLIGHT.h"
#include "FLIGHTTS.h" //FlightTripSearch
#include <map>

using namespace std;

// Check
class PERIOD
{
	public:
		map<string, vector<FLIGHT*> > flightsListByFrom;	//A map with the names of airports and yours respectives lists of avaible flights

		int departure;  // Unix timestamp when started the period
		
		int startTimeDate; //start of period
		
		int endTimeDate; //end of period

		PERIOD(){}


		//This block of methods treads with the information about the initial departure time 
		
		void set_departure_date(int departure){this->departure = departure;}
		
		int get_departure(){return departure;}
		
		//This block of methods is about the start and the end of this period
		void set_start_time_date(int startTimeDate){this->startTimeDate = startTimeDate;}
		
		int get_start_time_date(){return startTimeDate;}
		
		void set_end_time_date(int endTimeDate){this->endTimeDate = endTimeDate;}
		
		int get_end_time_date(){return endTimeDate;}



		//The following methods are about the map with the names of airports and yours respectives lists of avaible flights
		void set_flights_by_from(map<string, vector<FLIGHT*> > flightsByFrom){this->flightsListByFrom = flightsByFrom;}

		map<string, vector<FLIGHT*> > get_flights_by_from(){return flightsListByFrom;}

		//Those methods works with the selection of flights by period
		void set_flights_by_period_1(vector<FLIGHT*> flights, int departure2, map<int, PERIOD*> timeline)// without look the prices
		{
			int size_timeline = timeline.size();
			int index;
			int s = flights.size();
			
		

			for(int i = 0; i < s; i++)
			{
				
				index = (int)ceil( (flights[i]->get_departure_time() - departure2)/14400.00 );

				

				if(index <= size_timeline && index > 0) //9 131 137 140
				{
					
					if(timeline[index]->flightsListByFrom.empty()) // check
					{ 
						vector<FLIGHT*> list;
						list.clear();
						list.push_back(flights[i]); //create a list of flights

						this->flightsListByFrom.insert( pair<string, vector<FLIGHT*> >(flights[i]->get_from_where()->get_code(), list) ); //add it in the list of that period

						timeline[index]->set_flights_by_from(this->flightsListByFrom); // so add this list in respective period index
						this->flightsListByFrom.clear();
					}else if(timeline[index]->flightsListByFrom.count(flights[i]->get_from_where()->get_code()) == 0)
					{
						vector<FLIGHT*> list;
						list.clear();
						list.push_back(flights[i]); //create a list starting with the present flight

						timeline[index]->flightsListByFrom.insert( pair<string, vector<FLIGHT*> >(flights[i]->get_from_where()->get_code(), list) );// Insert it in the hash/map
					}else
					{ 
						timeline[index]->flightsListByFrom.at(flights[i]->get_from_where()->get_code()).push_back(flights[i]);
					}
				}
			}
		}

		void set_flights_by_period_2(vector<FLIGHT*> flights, double departure, map<int, PERIOD*> timeline)//looking the prices
		{
			int size_timeline = timeline.size();
			int index;

			int s = flights.size();

			for(int i = 0; i < s; i++)
			{
				index = (int)ceil( (flights[i]->get_departure_time() - departure)/14400.00 );

				if(index <= size_timeline && index > 0)
				{
					if(timeline[index]->flightsListByFrom.size()==0)
					{
						vector<FLIGHT*> list;
						list.push_back(flights[i]); //create a list of flights
						this->flightsListByFrom.insert( pair<string, vector<FLIGHT*> >(flights[i]->get_from_where()->get_code(), list) ); //add it in the lsit of that period
						timeline[index]->set_flights_by_from(this->flightsListByFrom); // so add this list in respective period index

					}else if(timeline[index]->flightsListByFrom.count(flights[i]->get_from_where()->get_code()) == 0) //(timeline[index]->flightsListByFrom.find( flights[i]->get_from_where()->get_code() ) == timeline[index]->flightsListByFrom.end())
					{
						vector<FLIGHT*> list;
						list.push_back(flights[i]); //create a list starting with the present flight

						timeline[index]->flightsListByFrom.insert( pair<string, vector<FLIGHT*> >(flights[i]->get_from_where()->get_code(), list) );// Insert it in the hash/map

					}else if(flights[i]->get_price() <= timeline[index]->flightsListByFrom[flights[i]->get_from_where()->get_code()][0]->get_price())
					{
						timeline[index]->flightsListByFrom[flights[i]->get_from_where()->get_code()].pop_back(); //remove the last element from the vector
						timeline[index]->flightsListByFrom[flights[i]->get_from_where()->get_code()].push_back(flights[i]); // add a new element to the vector
					}
				}
			}
		}
};

#endif
