#include "cube.h"



    bool Cube::create() {
        register_for_swipe_up(myScr);
        lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

        Serial.println("Creating shopping list");

        return true;
    }

    bool Cube::show() {
        Serial.println("Showing shopping list");

        return true;
    }

    void Cube::look(){
        player.dr = {
            static_cast<float>(sin(player.al)*cos(player.bt)), static_cast<float>(cos(player.al)*cos(player.bt)), static_cast<float>(sin(player.bt))
        };
        player.xr = {
                static_cast<float>(cos(player.al)), static_cast<float>(-sin(player.al)), 0
        };
        player.yr = {
                static_cast<float>(-sin(player.bt)*sin(player.al)), static_cast<float>(-sin(player.bt)*cos(player.al)), static_cast<float>(sin(player.al)*sin(player.al)*cos(player.bt)+cos(player.al)*cos(player.al)*cos(player.bt))
        };
        player.wmov = {
                static_cast<float>(sin(player.al)),static_cast<float>(cos(player.al)),0
        };
        player.dmov = {
                static_cast<float>(cos(player.al)),static_cast<float>(-sin(player.al)),0
        };
        player.dd = -player.x*player.dr[0]-player.y*player.dr[1]-player.z*player.dr[2];
        player.dx = -player.x*player.xr[0]-player.y*player.xr[1]-player.z*player.xr[2];
        player.dy = -player.x*player.yr[0]-player.y*player.yr[1]-player.z*player.yr[2];
    }

    std::vector<float> Cube::project(std::vector<float> p){
        float x,y;
        double den = p[0]*player.dr[0] + p[1]*player.dr[1] + p[2]*player.dr[2] + player.dd;
        if(den < 1) {
            return {0,1,1};
        }
        x =(p[0]*player.xr[0] + p[1]*player.xr[1] + p[2]*player.xr[2] + player.dx)/den;
        y =(p[0]*player.yr[0] + p[1]*player.yr[1] + p[2]*player.yr[2] + player.dy)/den;
        return {1,x,y};
}

