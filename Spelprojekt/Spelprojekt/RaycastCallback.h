#pragma once
#include "Physics.h"
#include <iostream>
#include "Logger.h"

class RaycastCallback : public rp3d::RaycastCallback
{

public:

    virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) {

        Logger::Write("dude waddap");

        // Display the world hit point coordinates 
        std::cout << "Hit point : " <<
            info.worldPoint.x <<
            info.worldPoint.y <<
            info.worldPoint.z <<
            std::endl;

        // Return a fraction of 1.0 to gather all hits 
        return rp3d::decimal(1.0);
    }
};
