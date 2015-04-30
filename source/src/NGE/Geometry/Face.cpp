#include "NGE/Geometry/Face.hpp"
#include <cstring>
using namespace NGE::Geometry;

Face::Face()
{
    numIndices = 0;
    indices = nullptr;
}

Face::Face(const Face& copy)
{
    delete[] indices;
    indices = nullptr;
    *this = copy;
}

Face::~Face()
{
    numIndices = 0;
    delete[] indices;
    indices = nullptr;
}

const Face& Face::operator=(const Face& o)
{
    if (&o == this)
        return *this;

    delete[] indices;
    numIndices = o.numIndices;
    indices = new unsigned int[numIndices];
    memcpy(indices, o.indices, numIndices * sizeof (unsigned int));
    return *this;
}

bool Face::operator==(const Face& o) const
{
    if (indices == o.indices)
        return true;
    else if (indices && numIndices == o.numIndices)
    {
        for (unsigned int i = 0; i < this->numIndices; ++i)
            if (indices[i] != o.indices[i])
                return false;
        return true;
    }

    return false;
}

bool Face::operator!=(const Face& o) const
{
    return !(*this == o);
}

unsigned int* Face::GetIndices() const
{
    return indices;
}

unsigned int Face::GetNumIndices() const
{
    return numIndices;
}

void Face::SetIndices(unsigned int* indices)
{
    this->indices = indices;
}

void Face::SetNumIndices(unsigned int numIndices)
{
    this->numIndices = numIndices;
}