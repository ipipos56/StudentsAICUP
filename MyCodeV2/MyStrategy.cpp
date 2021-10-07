#include "MyStrategy.hpp"
#include <exception>

MyStrategy::MyStrategy() {}

using namespace model;
using namespace std;

int distanceBetweenPlanets(Planet firstPlanet, Planet secondPlanet) {
    return abs(firstPlanet.x - secondPlanet.x) + abs(firstPlanet.y - secondPlanet.y);
}

MoveAction createMove(int startPlanet, int finalPlanet, int workersCount)
{
    MoveAction tempMove;
    tempMove.startPlanet = startPlanet;
    tempMove.targetPlanet = finalPlanet;
    tempMove.workerNumber = workersCount;
    return tempMove;
}

class PlanetWithIndex {
public:
    Planet planet;
    int index;

    PlanetWithIndex(Planet planet, int index) {
        this->planet = planet;
        this->index = index;
    }

    PlanetWithIndex() {
        this->planet = Planet();
        this->index = -1;
    }
};

Action MyStrategy::getAction(const Game &game) {
    int myGameIndex = game.myIndex;
    vector<PlanetWithIndex> freePlanets;
    vector<WorkerGroup> myWorkerGroups;
    vector<PlanetWithIndex> myWorkerGroupsPlanets;
    vector<MoveAction> moves;
    for (int i = 0; i < game.planets.size(); ++i) {
        auto planet = game.planets[i];
        auto workerGroups = planet.workerGroups;
        if (workerGroups.size() < 1) {
            freePlanets.push_back(PlanetWithIndex(planet, i));
        } else {
            for (int j = 0; j < workerGroups.size(); ++j) {
                auto workers = workerGroups[j];
                if (workers.playerIndex == myGameIndex) {
                    myWorkerGroups.push_back(workers);
                    myWorkerGroupsPlanets.push_back(PlanetWithIndex(planet, i));
                }
            }
        }
    }
    for (int i = 0; i < myWorkerGroups.size(); ++i) {
        double minPlanetDistance = 100000;
        int freePlanetMinIndex = -1;
        PlanetWithIndex minPlanet = PlanetWithIndex();
        for (int j = 0; j < freePlanets.size(); ++j) {
            int tempDistance = distanceBetweenPlanets(freePlanets[j].planet, myWorkerGroupsPlanets[i].planet);
            if (tempDistance < minPlanetDistance) {
                minPlanetDistance = tempDistance;
                minPlanet = freePlanets[j].planet;
                freePlanetMinIndex = j;
            }
        }
        if(freePlanetMinIndex != -1) {
            moves.push_back(createMove(myWorkerGroupsPlanets[i].index, minPlanet.index, myWorkerGroups[i].number / 2));
            myWorkerGroupsPlanets.erase(myWorkerGroupsPlanets.begin() + i);
            freePlanets.erase(freePlanets.begin() + freePlanetMinIndex);
        }
    }
    return Action(moves, vector<BuildingAction>());
}