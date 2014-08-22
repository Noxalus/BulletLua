#include "BulletLuaManager.hpp"

float BulletLuaManager::rank = 0.8;

BulletLuaManager::BulletLuaManager()
    : rng(rd())
{
    // increaseCapacity();
    blocks.push_back(new BulletLua[BLOCK_SIZE]);

    // Throw all bullets into free stack
    for (unsigned int i = 0; i < BLOCK_SIZE; ++i)
    {
        freeBullets.push(&blocks.back()[i]);
    }
}

BulletLuaManager::~BulletLuaManager()
{
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter)
    {
        delete [] *iter;
    }
}

void BulletLuaManager::createBullet(const std::string& filename, Bullet* origin, Bullet* target)
{
    BulletLua* b = getFreeBullet();
    b->set(filename, origin, target, this);
    bullets.push_back(b);
}

void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua, const std::string& func, Bullet* origin, Bullet* target)
{
    BulletLua* b = getFreeBullet();
    b->set(lua, func, origin, target, this);
    bullets.push_back(b);
}

void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua, const std::string& func,
                  float x, float y, float d, float s, Bullet* target)
{
    BulletLua* b = getFreeBullet();
    b->set(lua, func, x, y, d, s, target, this);
    bullets.push_back(b);
}

bool BulletLuaManager::checkCollision(Bullet& b)
{
    return collision.checkCollision(b);
}

void BulletLuaManager::tick()
{
    // Reset containers inside collision detection object.
    // Since bullets are dynamic and are most likely unpredictable,
    // this must be called every frame.
    collision.reset();

    for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
    {
        BulletLua* bullet = *iter;

        if (bullet->isDead())
        {
            freeBullets.push(bullet);
            iter = bullets.erase(iter);
        }
        else
        {
            bullet->run();
            collision.addBullet(bullet);
        }
    }
}

void BulletLuaManager::clear()
{
    while (!bullets.empty())
    {
        freeBullets.push(bullets.front());
        bullets.pop_front();
    }
}

void BulletLuaManager::vanishAll()
{
    for (BulletLua* b : bullets)
    {
        b->vanish();
    }
}

unsigned int BulletLuaManager::bulletCount() const
{
    return bullets.size();
}

unsigned int BulletLuaManager::freeCount() const
{
    return freeBullets.size();
}

unsigned int BulletLuaManager::blockCount() const
{
    return blocks.size();
}

float BulletLuaManager::randFloat()
{
    return randFloat(0.0f, 1.0f);
}

float BulletLuaManager::randFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

int BulletLuaManager::randInt(int max)
{
    return randInt(0, max);
}

int BulletLuaManager::randInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

BulletLua* BulletLuaManager::getFreeBullet()
{
    if (freeBullets.empty())
        increaseCapacity();

    BulletLua* bullet = freeBullets.top();
    freeBullets.pop();

    return bullet;
}

void BulletLuaManager::increaseCapacity(unsigned int blockSize)
{
    blocks.push_back(new BulletLua[blockSize]);

    // Throw all bullets into free stack
    for (unsigned int i = 0; i < blockSize; ++i)
    {
        freeBullets.push(&blocks.back()[i]);
    }

    // vertexCount += blockSize * 4;
    // vertices.resize(vertexCount);
}
