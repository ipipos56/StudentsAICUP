#include "MyStrategy.hpp"
#include <exception>
#include <iostream>
#include <set>

MyStrategy::MyStrategy() {}

using namespace model;
using namespace std;

int distanceBetweenPlanets(Planet *firstPlanet, Planet *secondPlanet) {
    return abs(firstPlanet->x - secondPlanet->x) + abs(firstPlanet->y - secondPlanet->y);
}

bool check = false;
set<int> usedPlanets;

Action MyStrategy::getAction(const Game &game) {
    if (!check) {
        //check = true;
        int myGameIndex = game.myIndex;
        vector<Planet> freePlanets;
        vector<WorkerGroup> myWorkerGroups;
        vector<Planet> myWorkerGroupsPlanets;
        vector<MoveAction> moves = vector<MoveAction>();
        for (int i = 0; i < game.planets.size(); ++i) {
            auto planet = game.planets[i];
            auto workerGroups = planet.workerGroups;
            //cout<<planet.x<<" "<<planet.y<<" "<<workerGroups.size()<<endl;
            if (workerGroups.size() < 1) {
                freePlanets.emplace_back(planet);
            } else {
                //usedPlanets.insert(tempPlanet->index);
                for (int j = 0; j < workerGroups.size(); ++j) {
                    auto workers = workerGroups[j];
                    if (workers.playerIndex == myGameIndex) {
                        myWorkerGroups.emplace_back(workers);
                        myWorkerGroupsPlanets.emplace_back(planet);
                    }
                }
            }
        }
        for (int i = 0; i < myWorkerGroups.size(); ++i) {
            double minPlanetDistance = 100000;
            int freePlanetMinIndex = -1;
            Planet *minPlanet;
            for (int j = 0; j < freePlanets.size(); ++j) {
                if (usedPlanets.find(freePlanets[j].id) == usedPlanets.end()) {
                    int tempDistance = distanceBetweenPlanets(&freePlanets[j],
                                                              &myWorkerGroupsPlanets[i]);
                    //cout<<freePlanets[j]->planet->x<<" "<<freePlanets[j]->planet->y<<endl;
                    if (tempDistance < minPlanetDistance) {
                        minPlanetDistance = tempDistance;
                        minPlanet = &freePlanets[j];
                        freePlanetMinIndex = j;
                    }
                }
            }
            if (freePlanetMinIndex != -1) {
                if (myWorkerGroups[i].number > 20) {
                    /*cout << myWorkerGroups[i].number << " " << myWorkerGroupsPlanets[i].x
                         << " " << myWorkerGroupsPlanets[i].y << " " << minPlanet->x
                         << " " << minPlanet->y << endl;*/
                    if(minPlanet->harvestableResource != nullopt) {
                        moves.emplace_back(
                                MoveAction(myWorkerGroupsPlanets[i].id, minPlanet->id,
                                           20,
                                           Resource::STONE));
                    }
                    else
                    {
                        moves.emplace_back(
                                MoveAction(myWorkerGroupsPlanets[i].id, minPlanet->id,
                                           10,
                                           nullopt));
                    }
                    //usedPlanets.insert(minPlanet->index);
                }
            }
        }
        return Action(moves, vector<BuildingAction>());
    }
    return Action(vector<MoveAction>(), vector<BuildingAction>());
}