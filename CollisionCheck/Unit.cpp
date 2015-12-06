#include "Unit.h"

Unit::Unit(vec3 & pos, float rad, float p, float y, float r) : Object(pos, rad, p, y, r), velocity(0, 0, 0), isAlly(false), isAlive(true)
{
}
