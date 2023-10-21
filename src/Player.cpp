#include "Player.h"
#include "Utilities.h"

namespace MiniFPS
{
    Player::Player(Level* level, const Settings& settings, const Vec2Int& cameraResolution) : m_level(level)
    {
        m_inputState = InputState();
        m_camera = Camera(settings.playerStartPos,
                          settings.fieldOfView * PI_180,
                          settings.renderDistance,
                          cameraResolution);
    }

    Player::Player() : m_level(nullptr)
    {}

    void Player::UpdateInputState()
    {
        m_inputState.mouseMotionX = 0;
        m_inputState.mouseMotionY = 0;
        m_inputState.endGame = false;
        m_inputState.leftMouseButtonPressed = false;
        m_inputState.rightMouseButtonPressed = false;

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                m_inputState.endGame = true;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                m_inputState.mouseMotionX = event.motion.xrel;
                m_inputState.mouseMotionY = event.motion.yrel;
                m_inputState.mousePosX = event.button.x;
                m_inputState.mousePosY = event.button.y;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    m_inputState.leftMouseButtonPressed = true;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_inputState.rightMouseButtonPressed = true;
                }
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

        if (currentKeyStates[SDL_SCANCODE_ESCAPE])
        {
            m_inputState.endGame = true;
        }

        if (currentKeyStates[SDL_SCANCODE_W])
        {
            m_inputState.moveForward = true;
        }
        else
        {
            m_inputState.moveForward = false;
        }

        if (currentKeyStates[SDL_SCANCODE_S])
        {
            m_inputState.moveBack = true;
        }
        else
        {
            m_inputState.moveBack = false;
        }

        if (currentKeyStates[SDL_SCANCODE_A])
        {
            m_inputState.moveLeft = true;
        }
        else
        {
            m_inputState.moveLeft = false;
        }

        if (currentKeyStates[SDL_SCANCODE_D])
        {
            m_inputState.moveRight = true;
        }
        else
        {
            m_inputState.moveRight = false;
        }
    }

    void Player::Move(float frameDelta, float speedModifier)
    {
        if (frameDelta != 0 && speedModifier != 0)
        {
            const float prevX = m_camera.pos.x;
            const float prevY = m_camera.pos.y;

            if (m_inputState.moveForward != m_inputState.moveBack)
            {
                if (m_inputState.moveForward)
                {
                    m_camera.pos += m_camera.direction * frameDelta * speedModifier;
                }

                if (m_inputState.moveBack)
                {
                    m_camera.pos -= m_camera.direction * frameDelta * speedModifier;
                }
            }

            if (m_inputState.moveLeft != m_inputState.moveRight)
            {
                if (m_inputState.moveLeft)
                {
                    Vec2 moveDirection = {m_camera.direction.y, -m_camera.direction.x};
                    moveDirection.Normalize();

                    m_camera.pos += moveDirection * frameDelta * speedModifier;
                }

                if (m_inputState.moveRight)
                {
                    Vec2 moveDirection = {-m_camera.direction.y, m_camera.direction.x};
                    moveDirection.Normalize();

                    m_camera.pos += moveDirection * frameDelta * speedModifier;
                }
            }

            if (m_level->HasCollided({m_camera.pos.x, m_camera.pos.y}))
            {
                m_camera.pos.x = prevX;
                m_camera.pos.y = prevY;
            }
        }
    }

    void Player::Rotate(float frameDelta, float rotationModifier)
    {
        float rotationAngle = m_inputState.mouseMotionX * frameDelta * rotationModifier;

        Vec2 oldDirection = m_camera.direction;
        m_camera.direction.x = oldDirection.x * cosf(rotationAngle) - oldDirection.y * sinf(rotationAngle);
        m_camera.direction.y = oldDirection.x * sinf(rotationAngle) + oldDirection.y * cosf(rotationAngle);
        m_camera.direction.Normalize();

        Vec2 oldPlane = m_camera.plane;
        m_camera.plane.x = oldPlane.x * cosf(rotationAngle) - oldPlane.y * sinf(rotationAngle);
        m_camera.plane.y = oldPlane.x * sinf(rotationAngle) + oldPlane.y * cosf(rotationAngle);
        m_camera.plane.Normalize();
    }

    bool Player::GameHasEnded() const
    {
        return m_inputState.endGame;
    }

    bool Player::InMainMenu() const
    {
        return m_inputState.inMainMenu;
    }

    void Player::Update(float frameDelta, float speedModifier, float rotationModifier)
    {
        UpdateInputState();
        Move(frameDelta, speedModifier);
        Rotate(frameDelta, rotationModifier);

        if (m_reloading)
        {
            m_reloadTimer += frameDelta;
        }

        if (m_reloadTimer > RELOAD_TIME)
        {
            m_currentAmmo = MAG_SIZE;
            m_reloadTimer = 0.0f;
            m_reloading = false;
        }

        if (m_justFired)
        {
            m_justFiredTimer += frameDelta;
        }

        if (m_justFiredTimer > JUST_FIRED_TIME)
        {
            m_justFiredTimer = 0.0f;
            m_justFired = false;
        }
    }

    bool Player::CanShoot()
    {
        if (m_currentAmmo > 0 && !m_reloading && !m_justFired)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // TODO: Make audio a singleton
    bool Player::Shoot(std::vector<Enemy>& enemies, float wallDistance)
    {
        AudioHandler& audioHandler = AudioHandler::GetInstance();

        if (CanShoot())
        {
            m_justFired = true;
            audioHandler.PlayEffect("GunShoot4");
            m_currentAmmo--;

            for (Enemy& enemy: enemies)
            {
                Vec2 wallIntersectionPoint = m_camera.pos;
                wallIntersectionPoint += (m_camera.direction * wallDistance);

                Vec2 delta = enemy.m_pos - m_camera.pos;
                float projection = (delta.x * (wallIntersectionPoint.x - m_camera.pos.x) +
                                    delta.y * (wallIntersectionPoint.y - m_camera.pos.y)) /
                                   (wallDistance * wallDistance);

                float distanceToShot;

                if (projection < 0.0)
                {
                    distanceToShot = delta.Length();
                }
                else if (projection > 1.0)
                {
                    distanceToShot = wallIntersectionPoint.Distance(enemy.m_pos);
                }
                else
                {
                    const Vec2 closestPointOnLine{
                        m_camera.pos.x + projection * (wallIntersectionPoint.x - m_camera.pos.x),
                        m_camera.pos.y + projection * (wallIntersectionPoint.y - m_camera.pos.y)
                    };

                    distanceToShot = closestPointOnLine.Distance(enemy.m_pos);
                }

                if (enemy.IsVisible() && distanceToShot < MAX_SHOT_DISTANCE)
                {
                    string info;
                    info += "Hit enemy at (";
                    info += std::to_string(enemy.m_pos.x);
                    info += ", ";
                    info += std::to_string(enemy.m_pos.y);
                    info += ")";
                    LogHandler::Log(info.c_str());

                    enemy.SetVisible(false);
                    return true;
                }
            }
        }
        else
        {
            audioHandler.PlayEffect("GunNoAmmo1");
        }


        return false;
    }

    void Player::Reload()
    {
        AudioHandler& audioHandler = AudioHandler::GetInstance();

        if (!m_reloading)
        {
            audioHandler.PlayEffect("GunReload1");
            m_reloadTimer = 0;
            m_reloading = true;
        }
        else
        {
            audioHandler.PlayEffect("GunNoAmmo1");
        }
    }

    InputState::InputState()
    {
        moveLeft = false;
        moveRight = false;
        moveForward = false;
        moveBack = false;
        endGame = false;
        inMainMenu = false;
        leftMouseButtonPressed = false;
        rightMouseButtonPressed = false;
        mousePosX = 0;
        mousePosY = 0;
    }
}