#ifndef _SpacialPartition_hpp_
#define _SpacialPartition_hpp_

#include "Bullet.hpp"

// Simple collision detection
class SpacialPartition
{
    public:
        SpacialPartition();
        // SpacialPartition(int width, int height);

        void addBullet(const Bullet* bullet);
        void reset();

        bool checkCollision(Bullet& b);

    private:
        static constexpr unsigned int tileSize = 100;
        static constexpr int WIDTH  = 640 / tileSize + 3;
        static constexpr int HEIGHT = 480 / tileSize + 3;
        static constexpr int CAP = 1000;

        // Out of bounds error waiting to happen...
        const Bullet* space[WIDTH][HEIGHT][CAP];
        int bulletCount[WIDTH][HEIGHT];
};

#endif // _SpacialPartition_hpp_
