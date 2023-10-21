#include "Level.h"

namespace MiniFPS
{
    Level::Level() = default;

    Level::Level(const string& filePath)
    {
        Load(filePath);
    }

    short Level::Get(const Vec2Int& point)
    {
        assert(point.x >= 0);
        assert(point.x < m_width);
        assert(point.y >= 0);
        assert(point.y < m_height);
        return m_matrix[point.y][point.x];
    }

    bool Level::HasCollided(const Vec2& point)
    {
        bool collided = false;

        // Check if out-of-bounds
        if (point.x < collisionThreshold || point.x > m_width - collisionThreshold || point.y < collisionThreshold ||
            point.y > m_height - collisionThreshold)
        {
            return true;
        }

        const int roundedX = static_cast<int>(roundf(point.x));
        const int roundedY = static_cast<int>(roundf(point.y));

        for (int cellX = roundedX - 1; cellX <= roundedX + 1; cellX++)
        {
            for (int cellY = roundedY - 1; cellY <= roundedY + 1; cellY++)
            {
                if (!(cellX < 0 || cellX >= m_width || cellY < 0 || cellY >= m_height))
                {
                    if (Get({cellX, cellY}) != 0)
                    {
                        if (point.x >= static_cast<double>(cellX) - collisionThreshold &&
                            point.x <= static_cast<double>(cellX) + 1 + collisionThreshold &&
                            point.y >= static_cast<double>(cellY) - collisionThreshold &&
                            point.y <= static_cast<double>(cellY) + 1 + collisionThreshold)
                        {
                            collided = true;
                        }
                    }
                }

                if (collided)
                {
                    break;
                }
            }

            if (collided)
            {
                break;
            }
        }

        return collided;
    }

    bool Level::IsPositionValid(const Vec2& point)
    {
        return (point.x >= 0 && point.x < m_width && point.y >= 0 && point.y < m_height);
    }

    bool Level::IsPositionValid(const Vec2Int &point)
    {
        return (point.x >= 0 && point.x < m_width && point.y >= 0 && point.y < m_height);
    }

    void Level::Load(const string& filePath)
    {
        std::ifstream infile(filePath);
        if (!infile)
        {
            string err;
            err += "Failed to open level file: ";
            err += filePath;
            LogHandler::LogError(err.c_str());
        }

        infile >> m_width >> m_height;

        m_matrix = new short* [m_height];

        for (int i = 0; i < m_height; i++)
        {
            m_matrix[i] = new short[m_width];
        }

        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++)
            {
                infile >> m_matrix[i][j];
            }
        }

        int numEnemies;
        infile >> numEnemies;

        for (int i = 0; i < numEnemies; i++)
        {
            short id;
            infile >> id;
            float x, y;
            infile >> x >> y;
            m_enemySpawnLocations.push_back({id, {x, y}});
        }

        while (infile.peek() != EOF)
        {
            short id;
            infile >> id;
            string textureName;
            infile >> textureName;
            m_textureIdMap[id] = textureName;
        }

        infile.close();
    }

    int Level::GetWidth()
    {
        return m_width;
    }

    int Level::GetHeight()
    {
        return m_height;
    }
}
