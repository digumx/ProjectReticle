#include <RetiColor.h>

RetiColor::RetiColor(float ir, float ig, float ib, float ia)
            : r(ir), g(ig), b(ib), a(ia) {}

RetiColor::RetiColor(float iv, float ia) : RetiColor(iv, iv, iv, ia) {}

RetiColor::RetiColor() : RetiColor(0.8, 0.2, 0.05, 1) {}

void RetiColor::invert()
{
    r = 1-r;
    g = 1-g;
    b = 1-b;
}

RetiColor& RetiColor::operator=(const RetiColor& col)
{
    r = col.r;
    g = col.g;
    b = col.b;
    a = col.a;
    return *this;
}

bool RetiColor::operator==(const RetiColor& col)
{
    bool ret = r == col.r;
    ret &= g == col.g;
    ret &= b == col.b;
    ret &= a == col.a;
    return ret;
}

RetiColor& RetiColor::operator+=(const RetiColor& col)
{
    r += col.r;
    g += col.g;
    b += col.b;
    a += col.a;
    return *this;
}

RetiColor& RetiColor::operator*=(const RetiColor& col)
{
    r *= col.r;
    g *= col.g;
    b *= col.b;
    a *= col.a;
    return *this;
}

RetiColor& RetiColor::operator&=(const RetiColor& col)
{
    r = r * (1-col.a) + col.r*col.a;
    g = g * (1-col.a) + col.g*col.a;
    r = b * (1-col.a) + col.b*col.a;
    return *this;
}

RetiColor& operator+(const RetiColor& a, const RetiColor& b);
{
    RetiColor* col = new RetiColor(a);
    col += b;
    return *col;
}

RetiColor& operator*(const RetiColor& a, const RetiColor& b);
{
    RetiColor* col = new RetiColor(a);
    col *= b;
    return *col;
}

RetiColor& operator&(const RetiColor& a, const RetiColor& b);
{
    RetiColor* col = new RetiColor(a);
    col &= b;
    return *col;
}

