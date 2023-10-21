#include "Pathfinding.h"

namespace MiniFPS
{
    Path CalculatePath(Level* level, const Vec2Int& startPos, const Vec2Int& goalPos)
    {
        PathfindingLevel pathfindingLevel;
        pathfindingLevel.level = level;
        pathfindingLevel.size = {level->GetWidth(), level->GetHeight()};
        pathfindingLevel.nodes = new PathfindingNode* [level->GetWidth()];

        // Allocate memory
        for (int i = 0; i < pathfindingLevel.size.x; i++)
        {
            pathfindingLevel.nodes[i] = new PathfindingNode[level->GetHeight()];
            for (int j = 0; j < pathfindingLevel.size.y; j++)
            {
                pathfindingLevel.nodes[i][j].pos = {i, j};
            }
        }

        std::priority_queue<PathfindingPQEntry, std::vector<PathfindingPQEntry>, std::greater<PathfindingPQEntry>> frontier;
        std::unordered_map<PathfindingNode*, PathfindingNode*> cameFrom;
        std::unordered_map<PathfindingNode*, float> costSoFar;

        PathfindingNode* startNode = &pathfindingLevel.nodes[startPos.x][startPos.y];
        PathfindingNode* goalNode = &pathfindingLevel.nodes[goalPos.x][goalPos.y];

        frontier.push({
            CalculateNodeToGoalHeuristic(*startNode, *goalNode),
            startNode
        });

        // cameFrom[startNode] = startNode;
        costSoFar[startNode] = 0.0f;

        while (!frontier.empty())
        {
            PathfindingNode* currentNode = frontier.top().node;
            frontier.pop();

            if (currentNode->pos == goalPos)
            {
                break;
            }

            const PathfindingNodeNeighbors nodeNeighbors = GetNeighbors(pathfindingLevel, currentNode);

            for (PathfindingNode* neighborNode : {nodeNeighbors.north, nodeNeighbors.west, nodeNeighbors.south, nodeNeighbors.east})
            {
                if (neighborNode != nullptr)
                {
                    float newCost = costSoFar[currentNode] + MOVEMENT_COST;

                    if (costSoFar.find(neighborNode) == costSoFar.end() || newCost < costSoFar[neighborNode])
                    {
                        costSoFar[neighborNode] = newCost;
                        float priority = newCost + CalculateNodeToGoalHeuristic(*neighborNode, *goalNode);
                        frontier.push({
                           priority,
                           neighborNode
                        });
                        cameFrom[neighborNode] = currentNode;
                    }
                }
            }
        }

        Path path;

        PathfindingNode* currentNode = &pathfindingLevel.nodes[goalPos.x][goalPos.y];

        while (currentNode != nullptr)
        {
            path.waypoints.push_back(currentNode->pos);
            currentNode = cameFrom[currentNode];
        }

        if (path.waypoints.size() >= 2)
        {
            path.valid = true;
        }

        std::reverse(path.waypoints.begin(), path.waypoints.end());

        // Free memory
        for (int i = 0; i < pathfindingLevel.size.x; i++)
        {
            delete[] pathfindingLevel.nodes[i];
        }
        delete[] pathfindingLevel.nodes;

        return path;
    }

    PathfindingNodeNeighbors GetNeighbors(const PathfindingLevel& pathfindingLevel, PathfindingNode* node)
    {
        PathfindingNodeNeighbors neighbors;

        const Vec2Int northNeighborPos {node->pos.x, node->pos.y-1};
        if (pathfindingLevel.level->IsPositionValid(northNeighborPos) && pathfindingLevel.level->Get(northNeighborPos) == 0)
        {
            PathfindingNode& northNode = pathfindingLevel.nodes[northNeighborPos.x][northNeighborPos.y];
            northNode.pos = northNeighborPos;
            neighbors.north = &northNode;
        }

        const Vec2Int westNeighborPos {node->pos.x-1, node->pos.y};
        if (pathfindingLevel.level->IsPositionValid(westNeighborPos) && pathfindingLevel.level->Get(westNeighborPos) == 0)
        {
            PathfindingNode& westNode = pathfindingLevel.nodes[westNeighborPos.x][westNeighborPos.y];
            westNode.pos = westNeighborPos;
            neighbors.west = &westNode;
        }

        const Vec2Int southNeighborPos {node->pos.x, node->pos.y+1};
        if (pathfindingLevel.level->IsPositionValid(southNeighborPos) && pathfindingLevel.level->Get(southNeighborPos) == 0)
        {
            PathfindingNode& southNode = pathfindingLevel.nodes[southNeighborPos.x][southNeighborPos.y];
            southNode.pos = southNeighborPos;
            neighbors.south = &southNode;
        }

        const Vec2Int eastNeighborPos {node->pos.x+1, node->pos.y};
        if (pathfindingLevel.level->IsPositionValid(eastNeighborPos) && pathfindingLevel.level->Get(eastNeighborPos) == 0)
        {
            PathfindingNode& eastNode = pathfindingLevel.nodes[eastNeighborPos.x][eastNeighborPos.y];
            eastNode.pos = eastNeighborPos;
            neighbors.east = &eastNode;
        }

        return neighbors;
    }

    float CalculateNodeToGoalHeuristic(const PathfindingNode& start, const PathfindingNode& end)
    {
        return std::sqrtf
        (
                std::pow(start.pos.x - end.pos.x, 2) +
                std::pow(start.pos.y - end.pos.y, 2)
        );
    }
}