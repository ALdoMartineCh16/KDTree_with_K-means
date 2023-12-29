#ifndef COORD_HPP
#define COORD_HPP

using namespace std;
class CoorD
{
private:
    long double x, y;
public:
    CoorD(long double xVal, long double yVal) : x(xVal), y(yVal) {}

    CoorD() : x(0.0), y(0.0) {}
    ~CoorD() = default;

    double getX() const { return x; }
    double getY() const { return y; }
    double getCord(int cord) const
    {
        if(cord==0){return x;}
        else if(cord==1){return y;}
        else{ return 0.0;}
    }

    double distance(const CoorD &other) const { return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2)); }

    CoorD operator+(const CoorD &other) const { return CoorD(x + other.x, y + other.y); }
    CoorD operator-(const CoorD &other) const { return CoorD(x - other.x, y - other.y); }
    CoorD operator*(double scalar) const { return CoorD(x * scalar, y * scalar); }
    CoorD operator/(double scalar) const { return CoorD(x / scalar, y / scalar); }

    bool operator==(const CoorD &other) const { return x == other.x && y == other.y; }
    bool operator!=(const CoorD &other) const { return !(*this == other); }

    bool operator<(const CoorD &other) const {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }

};
#endif