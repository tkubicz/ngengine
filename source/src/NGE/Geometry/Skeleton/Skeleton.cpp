#include "NGE/Geometry/Skeleton/Skeleton.hpp"
#include "NGE/Tools/Logger.hpp"
#include <fstream>
using namespace NGE::Geometry::Skeleton;

Skeleton::Skeleton()
{
    root = new SkeletonNode();
}

Skeleton::~Skeleton()
{
    delete root;
}

SkeletonNode* Skeleton::GetRoot() const
{
    return root;
}

void Skeleton::LoadSkeleton(const std::string& filename)
{
    float tx, ty, tz;
    int firstIndex, lastIndex, parentIndex;
    int numBones;
    
    // Korzeń ma zawsze indeks 0
    bones.push_back(root);
    
    std::ifstream ifile(filename.c_str(), std::ios::in);
    
    if (!ifile.is_open())
    {
        Tools::Logger::WriteErrorLog("Skeleton --> Could not open file: " + filename);
        return;
    }
    
    // Odczytanie liczby wierzchołków
    ifile >> numBones;
    
    // Zdefiniowane przez użytkownika wierzchołki zaczynają się od 1
    for (int i = 1; i <= numBones; ++i)
    {
        ifile >> parentIndex >> tx >> ty >> tz >> firstIndex >> lastIndex;
        SkeletonNode* bone = new SkeletonNode(parentIndex, tx, ty, tz, firstIndex, lastIndex);
        bones.push_back(bone);
    }
    
    ifile.close();
    AttachBones();
}

void Skeleton::AttachBones()
{
    int size = (int)bones.size();
    // Indeks rodzica
    for (int i = 0; i < size; ++i)
    {
        // Przypisanie kości z tym indeksem do jej rodzica
        for (int j = 1; j < size; ++j)
        {
            if (bones[j]->GetParentIndex() == i)
            {
                if (i == 0)
                    base = bones[j];
                bones[i]->AddChild(bones[j]);
            }
        }
    }
}

void Skeleton::Rotate(int i, float angleX, float angleY, float angleZ)
{
    bones[i]->RotateX(angleX);
    bones[i]->RotateY(angleY);
    bones[i]->RotateZ(angleZ);
}

void Skeleton::TranslateBase(float tx, float ty, float tz)
{
    if (base != NULL)
        base->Translate(tx, ty, tz);
}
