#pragma once

    #include "../../app.h"

    LV_IMG_DECLARE(cube);


    class Cube : public App {

        virtual bool create();
        virtual bool show();
        virtual bool hide() { return true;};
        virtual bool destroy() { return true;}
        
        void* getIcon() { return (void*) &cube; }
        const char * getName() { return "Cube"; }

        struct obj{
            void (*draw)();
            float* data;
        };

        struct Player{
            float x=0;
            float y=-10;
            float z=0;
            float al=0;
            float bt=0;
            std::vector<float> dr = {
                    static_cast<float>(sin(al)*cos(bt)), static_cast<float>(cos(al)*cos(bt)), static_cast<float>(sin(bt))
            };
            std::vector<float> xr = {
                    static_cast<float>(cos(al)), static_cast<float>(-sin(al)), 0
            };
            std::vector<float> yr = {
                    static_cast<float>(-sin(bt)*sin(al)), static_cast<float>(-sin(bt)*cos(al)), static_cast<float>(sin(al)*sin(al)*cos(bt)+cos(al)*cos(al)*cos(bt))
            };
            std::vector<float> wmov = {
                    static_cast<float>(sin(al)),static_cast<float>(cos(al)),0
            };
            std::vector<float> dmov = {
                    static_cast<float>(cos(al)),static_cast<float>(-sin(al)),0
            };
            std::vector<float> v = {
                    0,0,0
            };
            float dd = -x*dr[0]-y*dr[1]-z*dr[2];
            float dx = -x*xr[0]-y*xr[1]-z*xr[2];
            float dy = -x*yr[0]-y*yr[1]-z*yr[2];
        };

        void look();

        obj* objarr;

        int objcount;

        Player player;

        float pi = 3.1415926;

        float FOV = pi/2;

        std::vector<float>project(std::vector<float> p);

    };