BulletLua
=========

This C++ project aims to create a sane scriptable interface to define bullet patterns in shoot 'em up games. These patterns, while difficult to dodge in game and hard to appreciate while in the heat of the moment, can be beautiful to spectate.

Bullet Barrage Examples:

[![Pattern1](https://www.youtube.com/watch?v=yCOYrMgnNGE/0.jpg)](http://youtu.be/yCOYrMgnNGE?t=11m39s)

[![Pattern2](https://www.youtube.com/watch?v=v4m08AeGFWU/0.jpg)](http://youtu.be/v4m08AeGFWU?t=33m22s)

BulletLua is not quite mature enough at the moment to recreate the complex patterns in the videos above, but it's getting there.


![test.lua](./result3.gif)

![test2.lua](./result4.gif)

![test3.lua](./result5.gif)


Usage
=========
This project depends on [Lua](http://www.lua.org/), so make sure your compiler can find correct files (library, header files, etc).
For ease of use, [sol](https://github.com/Rapptz/sol.git) is used as a modern C++ binding to Lua. Remember to

    git submodule update --init --recursive

after you clone this repository!

Both sol and BulletLua use C++11 features, so make sure you have a C++11-capable compiler.

A Makefile is provided, a simple

    make

(or if you want to build it without debugging symbols)

    make config=release

in the terminal should be enough to build the project. I use [premake](http://industriousone.com/what-premake) to generate my makefiles, which can also output other project types. So look into that if you wish to build this outside the terminal.

Link the library generated in the `lib` directory and make sure the headers in the `include` directory can be found by your project, and you're already halfway there.

Because there are so many use cases out there, BulletLua does not do any drawing of sprites. It simply runs lua scripts, manages the generated bullets, and provides a simple method for collision detection. As such, you'll need to produce your own code to draw the bullets. This can be as simple as creating a class to inherit from BulletLuaManager and creating a draw method. Example:

    class BulletManager : public BulletLuaManager
    {
        public:
            void draw()
            {
                for (Bullet* b : bullets)
                {
                    drawPoint(b->x, b->y);
                }
            }
    };

Then to run a script...

    BulletManager manager;
    Bullet origin(x, y, vx, vy);
    Bullet destination(x, y, vx, vy);

    manager.createBullet("MyFile.lua", &origin, &destination);

    // Game Loop
    while (running)
    {
        manager.tick();
        manager.draw();
    }

A moderately complex example (using [SFML](http://www.sfml-dev.org/) can be found in the `example` directory.

Lua Binding
=========

C++ Functions visible in BulletLua scripts:

    -- Get Position (as a tuple).
    x, y = getPosition()

    -- Get Velocity Components (as a tuple).
    vx, vy = getVelocity()

    -- Get Bullet Speed and Direction.
    getSpeed()
    getDir()

    -- Get the amount of frames since this bullet's creation.
    getTurn()

    -- Get the current barrage "difficulty", From [0.0, 1.0]
    getRank()

    -- Generate random floating point numbers
    randFloat()
    randFloat(float min, float max)

    -- Generate random integers
    randInt(int max)
    randInt(int min, int max)

    -- Set Bullet Position.
    setPosition(float x, float y)

    -- Set Bullet Velocity.
    setVelocity(float vx, float vy)

    -- Set Bullet Direction
    setDirection(double dir)

    -- Set Bullet Direction (relatively), adds to current direction.
    setDirectionRelative(double dir)

    -- Set the current bullet to aim at the "player"
    aimTarget()

    -- Set the current bullet to aim at a point
    aimPoint(float x, float y)

    -- Set Speed
    setSpeed(float s)
    setSpeedRelative(float s)

    -- Switch current running function
    setFunction(const sol::function& funcName)

    -- Shoot a bullet at (x, y) moving in direction (d) at speed (s) running function (func).
    fire(double d, double s, const sol::function& funcName)

    -- Shoot (segments) bullets in a circle at speed (s) running function (func).
    fireCircle(int segments, float s, const sol::function& funcName)

    -- Fade out the current bullet. Kill it slowly.
    vanish()

    -- Immediately destroy the bullet.
    kill()

An example BulletLua script can be found in `example/bin/script/test.lua`. More examples to come.

Examples
=========

This script should produce something similar to the first gif above:

    -- BulletLua Test Script

    dir = 0

    function main()
        local turn = getTurn()
        local rank = getRank()

        if (math.fmod(turn, 15) == 0) then
            fire(dir, 1.5, explode)
            dir = dir + 34
        end
    end

    function explode()
        local turn = getTurn()
        if (turn == 60) then
            fireCircle(40, 4.0, homeIn)

            kill()
        end
    end

    function homeIn()
        local turn = getTurn()
        if (turn == 20) then
            setSpeed(1)
        elseif (turn == 25) then
            aimTarget()
            setSpeed(10)
        elseif (turn == 50) then
            vanish()
        end
    end

Right now, BulletLua associates a single function with a bullet and runs that function every frames. However, since it's difficult to dynamically save arbitrary function arguments in a single container, when you set a bullet to run a function, that function cannot have parameters. A way to bypass this is by using a `bind` function. `bind` will create a wrapper function for an existing function and forward some arguments to it. So effectively, you can transform any function with parameters into a function with no parameters. This example (`example/bin/script/test5.lua`) should show some simple `bind` usage.

![test5.lua](./result6.gif)

    -- BulletLua Test Script 5

    cycles = 0
    gwait = 20

    function bind(f,...)
       local args = {...}
       return function(...)
          return f(unpack(args),...)
       end
    end

    function main()
       circle = bind(fireCircle, 40)
       setPosition(320, 240)
       setFunction(bind(go, 10))
    end

    function go(wait)
       local turn = getTurn()
       if (math.fmod(turn, wait) == 0) then
          -- speeds = {4, 5, 6}
          circle(21 - gwait, curve)

          setFunction(bind(go, gwait))
          gwait = gwait - 1
          if gwait < 18 then
             gwait = 20
          end

          cycles = cycles + 1
       end
       if (cycles == 15) then
          vanish()
       end
    end

    function curve()
       local turn = getTurn()
       setDirectionRelative(5)
       setSpeedRelative(0.1)

       if (turn > 180) then
          vanish()
       end
    end
