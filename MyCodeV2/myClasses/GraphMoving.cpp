#include "model/Game.hpp"
#include <iostream>
#include <queue>
#include <set>

using namespace model;
using namespace std;

class GraphMoving {
public:
    inline static vector<vector<int>> planetsGraph;
    inline static Game *gameGlobal;
    inline static set<vector<int>> paths;

    static void planetsGraphSetter(Game &game) {
        gameGlobal = &game;
        vector<Planet> planets = game.planets;
        planetsGraph = vector<vector<int>>(planets.size() + 1, vector<int>(planets.size() + 1, -1));
        for (int i = 0; i < planets.size(); ++i) {
            for (int j = 0; j < planets.size(); ++j) {
                int tempDistance = distanceBetweenPlanets(&planets[i], &planets[j]);
                if (tempDistance <= game.maxTravelDistance) {
                    planetsGraph[i][j] = tempDistance;
                    planetsGraph[j][i] = tempDistance;
                }
            }
        }
    }

    static void startMoveWithPath(vector<int> path){

    }

    static vector<int> dijkstraPathFinder(Planet *firstPlanet, Planet *secondPlanet) {
        vector<int> path = vector<int>();
        queue<Planet> notVisitedPlanets;
        unordered_map<int,int> minimaDistances;
        unordered_map<int,int> parents;
        notVisitedPlanets.push(*firstPlanet);
        minimaDistances[firstPlanet->id] = 0;
        parents[firstPlanet->id] = -1;
        while (!notVisitedPlanets.empty())
        {
            Planet tempPlanet = notVisitedPlanets.front();
            notVisitedPlanets.pop();
            cout<<"from: "<<tempPlanet.id<<" "<<tempPlanet.x<<" "<<tempPlanet.y<<" "<<minimaDistances[tempPlanet.id]<<endl;
            for(int i = 0;i<planetsGraph[tempPlanet.id].size();i++)
            {
                if(i != tempPlanet.id && i != firstPlanet->id) {
                    if (minimaDistances.find(i) == minimaDistances.end()
                            && planetsGraph[tempPlanet.id][i] != -1)
                    {
                        minimaDistances[i] = minimaDistances[tempPlanet.id] + planetsGraph[tempPlanet.id][i];
                        notVisitedPlanets.push(gameGlobal->planets[i]);
                        parents[i] = tempPlanet.id;
                        cout<<gameGlobal->planets[i].id<<" "<<gameGlobal->planets[i].x<<" "<<gameGlobal->planets[i].y<<" "<<minimaDistances[i]<<endl;
                    }
                    else if (minimaDistances.find(i) != minimaDistances.end() && planetsGraph[tempPlanet.id][i] != -1
                            && minimaDistances[tempPlanet.id] + planetsGraph[tempPlanet.id][i] <= minimaDistances[i])
                    {
                        minimaDistances[i] = minimaDistances[tempPlanet.id] + planetsGraph[tempPlanet.id][i];
                        notVisitedPlanets.push(gameGlobal->planets[i]);
                        parents[i] = tempPlanet.id;
                        cout<<gameGlobal->planets[i].id<<" "<<gameGlobal->planets[i].x<<" "<<gameGlobal->planets[i].y<<" "<<minimaDistances[i]<<endl;
                    }
                }
            }
        }
        if(minimaDistances.find(secondPlanet->id) != minimaDistances.end())
        {
            int tempPlanet = secondPlanet->id;
            while(parents[tempPlanet] != -1)
            {
                path.emplace_back(tempPlanet);
                tempPlanet = parents[tempPlanet];
            }
            path.emplace_back(firstPlanet->id);
            return path;
        }
        return path;
    }

    static int distanceBetweenPlanets(Planet *firstPlanet, Planet *secondPlanet) {
        return abs(firstPlanet->x - secondPlanet->x) + abs(firstPlanet->y - secondPlanet->y);
    }


};