#include "NGE/Geometry/Models/ColladaModel.hpp"

using namespace NGE::Geometry::Models;

bool ColladaModel::Initialize(const std::string& filename)
{
    fileParseResult = colladaFile.load_file(filename.c_str());

    if (fileParseResult.status != pugi::status_ok)
    {
        log_error("ColladaModel -> Could not load file: " + filename);
        return false;
    }

    this->fileName = filename;
    return true;
}

bool ColladaModel::LoadAsset()
{
    // Check if the file is open.
    if (fileParseResult.status != pugi::status_ok)
    {
        log_error("ColladaModel -> File was not opened properly.");
        return false;
    }

    // Load COLLADA and asset node.
    pugi::xml_node colladaNode = colladaFile.child("COLLADA");
    pugi::xml_node assetNode = colladaNode.child("asset");

    // If assetNode is empty we have not to do here.
    if (assetNode == NULL || assetNode.empty())
    {
        log_info("ColladaModel --> Could not load information from \"asset\" node");
        return true;
    }

    // Get contributors.
    int contributorSize = assetNode.select_nodes("contributor").size();

    // If we have at least one contributor, get it data.
    if (contributorSize > 0)
    {
        // Allocate memory for contributors.
        asset.contributor.reserve(contributorSize);

        // Loop through contributors
        for (pugi::xml_node contributorNode = assetNode.child("contributor"); contributorNode != NULL; contributorNode = contributorNode.next_sibling("contributor"))
        {
            collada_contributor contr;
            contr.author = contributorNode.child("author").text().as_string();
            contr.authoring_tool = contributorNode.child("authoring_tool").text().as_string();
            contr.comments = contributorNode.child("comments").text().as_string();
            contr.copyright = contributorNode.child("copyright").text().as_string();
            contr.source_data = contributorNode.child("source_data").text().as_string();
            asset.contributor.push_back(contr);
        }
    }

    // Get create and modified times.
    asset.created = assetNode.child("created").text().as_string();
    asset.modified = assetNode.child("modified").text().as_string();

    // Get keywords.
    asset.keywords = assetNode.child("keywords").text().as_string();

    // Get revision.
    asset.revision = assetNode.child("revision").text().as_string();

    // Get subject.
    asset.subject = assetNode.child("subject").text().as_string();

    // Get title.
    asset.title = assetNode.child("title").text().as_string();

    // Get unit.
    std::string unitName = assetNode.child("unit").attribute("name").as_string();
    //float unitMeter = assetNode.child("unit").attribute("meter").as_float();
    // TODO: asset.unit = std::make_pair<std::string, float>(unitName, unitMeter);

    // Get Up axis.
    std::string upAxis = assetNode.child("up_axis").text().as_string();
    if (upAxis == "X_UP")
        asset.upAxis = X_UP;
    else if (upAxis == "Y_UP")
        asset.upAxis = Y_UP;
    else if (upAxis == "Z_UP")
        asset.upAxis = Z_UP;
    else
        log_error("ColladaModel -> up_axis value is wrong: " + upAxis);

    return true;
}

void ColladaModel::PrintAsset()
{
    asset.Print();
}
