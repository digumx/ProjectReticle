#ifndef RETICOLOR_H_INCLUDED
#define RETICOLOR_H_INCLUDED

class RetiColor
{
public:

    float r;
    float g;
    float b;
    float a;

    RetiColor();
    RetiColor(float x);
    RetiColor(float v, float ia);
    RetiColor(float ir, float ig, float ib, float ia);

    void invert();

    RetiColor& operator+=(const RetiColor& col);
    RetiColor& operato*=(const RetiColor& col);

};

RetiColor& operator+(const RetiColor& )

#endif // RETICOLOR_H_INCLUDED
