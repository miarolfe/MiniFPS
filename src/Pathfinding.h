#pragma once

#include "Level.h"
#include "Vector.h"

namespace MiniFPS
{
    const float MOVEMENT_COST = 1.0f;

    struct PathfindingNode
    {
        Vec2Int pos;
    };

    struct PathfindingPQEntry
    {
        float priority = 0.0f;
        PathfindingNode* node = nullptr;

        PathfindingPQEntry(float priority, PathfindingNode* node) : priority(priority), node(node) {}

        bool operator>(const PathfindingPQEntry& other) const
        {
            return priority > other.priority;
        }
    };

    struct PathfindingNodeNeighbors
    {
        PathfindingNode* north = nullptr;
        PathfindingNode* west = nullptr;
        PathfindingNode* south = nullptr;
        PathfindingNode* east = nullptr;
    };

    struct PathfindingLevel
    {
        PathfindingNode** nodes;
        Level* level;
        Vec2Int size;
        std::priority_queue<PathfindingPQEntry, std::vector<PathfindingPQEntry>, std::greater<PathfindingPQEntry>> frontier;
        std::unordered_map<PathfindingNode*, PathfindingNode*> cameFrom;
        std::unordered_map<PathfindingNode*, float> costSoFar;
    };

    struct Path
    {
        std::vector<Vec2> waypoints;
        bool valid = false;
    };

    Path CalculatePath(Level* level, const Vec2Int& startPos, const Vec2Int& goalPos);
    PathfindingNodeNeighbors GetNeighbors(const PathfindingLevel& pathfindingLevel, PathfindingNode* node);
    float CalculateNodeToGoalHeuristic(const PathfindingNode& start, const PathfindingNode& end);
    Path ReconstructPath(PathfindingLevel& pathfindingLevel, PathfindingNode* currentNode);
}
