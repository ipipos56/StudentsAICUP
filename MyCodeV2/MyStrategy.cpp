#include "MyStrategy.hpp"
#include "myClasses/GraphMoving.cpp"
#include <exception>
#include <iostream>
#include <set>

MyStrategy::MyStrategy() {}

using namespace model;
using namespace std;

unordered_map<optional<Resource>, BuildingType> resoursesTypes;

void initialization(const Game &game) {
    GraphMoving::planetsGraphSetter(const_cast<Game &>(game));
    BuildingType type;
    for (int i = 0; i < 9; i++) {
        type = static_cast<BuildingType>(i);
        if (game.buildingProperties.find(type) != game.buildingProperties.end()) {
            optional<Resource> tempResourse = game.buildingProperties.find(type)->second.produceResource;
            if (tempResourse.has_value()) {
                resoursesTypes.insert(make_pair(tempResourse, type));
                cout << resourceToString(tempResourse.value_or(Resource::NO)) << " " << buildingTypeToString(type)
                     << endl;
            }
        }
    }
}

bool check = false;
bool init = false;
set<int> usedPlanets;

Action MyStrategy::getAction(const Game &game) {
    if (!init) {
        init = true;
        initialization(game);
        Planet pl1 = game.planets[0];
        Planet pl2 = game.planets[50];
        vector<int> answ;
        answ = GraphMoving::dijkstraPathFinder(&pl1, &pl2);
        for(int i = 0;i<answ.size();i++)
        {
            cout<<answ[i]<<" ";
        }
        cout<<endl;
    }
    if (check) {
        //check = true;
        int myGameIndex = game.myIndex;
        vector<Planet> freePlanets;
        vector<WorkerGroup> myWorkerGroups;
        vector<Planet> myWorkerGroupsPlanets;
        vector<MoveAction> moves = vector<MoveAction>();
        vector<BuildingAction> build = vector<BuildingAction>();
        for (int i = 0; i < game.planets.size(); ++i) {
            bool haveWorkers = false;
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
                        haveWorkers = true;
                        myWorkerGroups.emplace_back(workers);
                        myWorkerGroupsPlanets.emplace_back(planet);
                    }
                }
            }
            if (planet.building == nullopt && haveWorkers) {
                if (resoursesTypes.find(planet.harvestableResource) != resoursesTypes.end()) {
                    //cout<<planet.id<<" "<<buildingTypeToString(resoursesTypes[planet.harvestableResource])<<endl;
                    build.emplace_back(BuildingAction(planet.id, resoursesTypes[planet.harvestableResource]));
                }
            }
        }
        for (int i = 0; i < myWorkerGroups.size(); ++i) {
            double minPlanetDistance = 100000;
            int freePlanetMinIndex = -1;
            Planet *minPlanet;
            for (int j = 0; j < freePlanets.size(); ++j) {
                if (usedPlanets.find(freePlanets[j].id) == usedPlanets.end()) {
                    int tempDistance = GraphMoving::distanceBetweenPlanets(&freePlanets[j],
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
                if (myWorkerGroups[i].number > 50) {
                    /*cout << myWorkerGroups[i].number << " " << myWorkerGroupsPlanets[i].x
                         << " " << myWorkerGroupsPlanets[i].y << " " << minPlanet->x
                         << " " << minPlanet->y << endl;*/
                    if (minPlanet->harvestableResource != nullopt) {
                        moves.emplace_back(
                                MoveAction(myWorkerGroupsPlanets[i].id, minPlanet->id,
                                           50,
                                           Resource::STONE));
                    } else {
                        moves.emplace_back(
                                MoveAction(myWorkerGroupsPlanets[i].id, minPlanet->id,
                                           10,
                                           nullopt));
                    }
                    usedPlanets.insert(minPlanet->id);
                }
            }
        }
        return Action(moves, build);
    }
    return Action(vector<MoveAction>(), vector<BuildingAction>());
}