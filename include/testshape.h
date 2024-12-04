#ifndef TESTSHAPE_H
#define TESTSHAPE_H

#include "shape.h"

class TestShape : public Shape {
public:
    // Constructor
    TestShape(Matrix4f t = Matrix4f::Identity(), Material m = Material());
    
    // Destructor
    virtual ~TestShape() = default;
};

#endif // TESTSHAPE_H