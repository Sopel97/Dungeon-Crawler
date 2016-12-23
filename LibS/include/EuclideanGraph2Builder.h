#pragma once

#include "Fwd.h"

#include "EuclideanGraph2.h"
#include "DelaunayTriangulation2.h"

namespace ls
{
    template <class EuclideanGraphType>
    class EuclideanGraph2Builder
    {
    public:
        static EuclideanGraphType fromTriangulation(const DelaunayTriangulation2<typename EuclideanGraphType::ValueType>& triangulation);

    private:
        EuclideanGraph2Builder() = delete;
        ~EuclideanGraph2Builder() = delete;
    };
}

#include "../src/EuclideanGraph2Builder.cpp"
