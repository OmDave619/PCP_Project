#pragma once 

class Point {
    public: 
    double x,y;

    Point(double x=0.0, double y=0.0);
    bool operator==(const Point& other) const;
    bool operator<(const Point& other) const;
    double operator() (const Point& other) const;
};