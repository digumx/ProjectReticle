#ifndef RETICOLOR_H_INCLUDED
#define RETICOLOR_H_INCLUDED

#include <atomic>

class RetiColor
{
public:

    float r;
    float g;
    float b;
    float a;

    RetiColor();
    RetiColor(float iv, float ia=1);
    RetiColor(float ir, float ig, float ib, float ia=1);
    RetiColor(const RetiColor& other);
    ~RetiColor();

    void invert();

    RetiColor& operator=(const RetiColor& col);
    bool operator==(const RetiColor& col);
    RetiColor& operator+=(const RetiColor& col);
    RetiColor& operator*=(const RetiColor& col);
    RetiColor& operator&=(const RetiColor& col);
};

RetiColor& operator+(const RetiColor& a, const RetiColor& b);
RetiColor& operator*(const RetiColor& a, const RetiColor& b);
RetiColor& operator&(const RetiColor& a, const RetiColor& b);

#endif // RETICOLOR_H_INCLUDED
