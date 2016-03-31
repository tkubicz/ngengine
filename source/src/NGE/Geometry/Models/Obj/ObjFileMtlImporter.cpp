#include "NGE/Geometry/Models/Obj/ObjFileMtlImporter.hpp"
#include "NGE/Geometry/Models/Obj/ObjTools.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
using namespace NGE::Geometry::Models::Obj;

ObjFileMtlImporter::ObjFileMtlImporter(std::vector<char>& buffer, const std::string& absPath, Obj::Model* model) :
dataIt(buffer.begin()), dataItEnd(buffer.end()), model(model), line(0) {
    if (model == nullptr) {
        log_error("Model pointer is null");
        return;
    }

    if (model->defaultMaterial == nullptr) {
        model->defaultMaterial = new Obj::Material;
        model->defaultMaterial->materialName = "default";
    }

    Load();
}

ObjFileMtlImporter::~ObjFileMtlImporter() {
    // Empty.
}

ObjFileMtlImporter::ObjFileMtlImporter(const ObjFileMtlImporter& other) {
    // Empty.
}

ObjFileMtlImporter& ObjFileMtlImporter::operator=(const ObjFileMtlImporter& other) {
    return *this;
}

void ObjFileMtlImporter::Load() {
    if (dataIt == dataItEnd)
        return;

    while (dataIt != dataItEnd) {
        switch (*dataIt) {
            case 'K':
            {
                ++dataIt;
                if (*dataIt == 'a') // Ambient color.
                {
                    ++dataIt;
                    GetColorRGBA(&model->currentMaterial->ambient);
                } else if (*dataIt == 'd') // Diffuse color.
                {
                    ++dataIt;
                    GetColorRGBA(&model->currentMaterial->diffuse);
                } else if (*dataIt == 's') // Specular color.
                {
                    ++dataIt;
                    GetColorRGBA(&model->currentMaterial->specular);
                }
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }

            case 'd': // Alpha value
            {
                ++dataIt;
                GetFloatValue(model->currentMaterial->alpha);
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }

            case 'N': // Shineness
            {
                ++dataIt;
                switch (*dataIt) {
                    case 's':
                        ++dataIt;
                        GetFloatValue(model->currentMaterial->shineness);
                        break;
                    case 'i': // Index of refraction
                        ++dataIt;
                        GetFloatValue(model->currentMaterial->ior);
                        break;
                }
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }

            case 'm': // Texture.
            case 'b': // quick'n'dirty - for bum sections.
            {
                GetTexture();
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }

            case 'n': // New material name.
            {
                CreateMaterial();
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }

            case 'i': // Illumination model.
            {
                dataIt = GetNextToken<DataArrayIt>(dataIt, dataItEnd);
                GetIlluminationModel(model->currentMaterial->illuminationModel);
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }

            default:
            {
                dataIt = SkipLine<DataArrayIt>(dataIt, dataItEnd, line);
                break;
            }
        }
    }
}

void ObjFileMtlImporter::GetColorRGBA(Math::vec3f* color) {
    if (color == nullptr) {
        log_error("Color pointer is null");
        return;
    }

    float r, g, b;
    dataIt = GetFloat<DataArrayIt>(dataIt, dataItEnd, r);
    color->x = r;

    dataIt = GetFloat<DataArrayIt>(dataIt, dataItEnd, g);
    color->y = g;

    dataIt = GetFloat<DataArrayIt>(dataIt, dataItEnd, b);
    color->z = b;
}

void ObjFileMtlImporter::GetIlluminationModel(int& illumModel) {
    dataIt = CopyNextWord<DataArrayIt>(dataIt, dataItEnd, buffer, BUFFERSIZE);
    illumModel = atoi(buffer);
}

void ObjFileMtlImporter::GetFloatValue(float& value) {
    dataIt = CopyNextWord<DataArrayIt>(dataIt, dataItEnd, buffer, BUFFERSIZE);
    value = (float) Math::NumberConverter::FastAToF(buffer);
}

void ObjFileMtlImporter::CreateMaterial() {
    std::string line("");
    while (!IsNewLine(*dataIt)) {
        line += *dataIt;
        ++dataIt;
    }

    std::vector<std::string> token;
    const unsigned int numToken = Tokenize<std::string>(line, token, " ");
    std::string name("");
    if (numToken == 1) {
        name = "DefaultMaterial";
    } else {
        name = token[1];
    }

    std::map<std::string, Obj::Material*>::iterator it = model->materialMap.find(name);
    if (model->materialMap.end() == it) {
        // New material created.
        model->currentMaterial = new Obj::Material;
        model->currentMaterial->materialName = name;
        model->materialLib.push_back(name);
        model->materialMap[name] = model->currentMaterial;
    } else {
        // Use older material.
        model->currentMaterial = (*it).second;
    }
}

void ObjFileMtlImporter::GetTexture() {
    // TODO: Fix that!
}









