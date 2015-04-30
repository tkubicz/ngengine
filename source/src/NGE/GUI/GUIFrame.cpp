#include "NGE/GUI/GUIFrame.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::GUI;

GUIFrame::GUIFrame() : GUIPanel()
{
    listener = NULL;
    update = true;
}

GUIFrame::~GUIFrame()
{
    
}

bool GUIFrame::LoadXMLSettings(const pugi::xml_node& node)
{
    if (string(node.name()) != "Panel")
    {
        Tools::Logger::WriteErrorLog("Need a Frame node");
        return false;
    }
    
    if (node.attribute("name").empty() && node.attribute("callbackString").empty())
    {
        Tools::Logger::WriteErrorLog("Need 'name' or 'callbackString' attribute");
        return false;
    }
    
    callbackString = !node.attribute("name").empty() ? node.attribute("name").as_string() : node.attribute("callbackString").as_string();
    
    return GUIPanel::LoadXMLSettings(node);
}

void GUIFrame::ForceUpdate(bool update)
{
    updateCount = update ? GetTreeHeight() + 1 : 0;
}

void GUIFrame::Update(float dt)
{
    if (!visible)
        return;
    
    size_t t = 0;
    
    while (updateCount)
    {
        for (t = 0; t < elements.size(); ++t)
            elements[t]->ForceUpdate(true);
        updateCount--;
    }
    
    for (t = 0; t < elements.size(); ++t)
        elements[t]->Update(dt);
}

void GUIFrame::Render()
{
    if (!visible)
        return;
    
    for (size_t t = 0; t < elements.size(); ++t)
        elements[t]->Render();
}

void GUIFrame::SetGUIEventListener(NGE::Events::GUIEventListener* listener)
{
    this->listener = listener;
}

NGE::Events::GUIEventListener* GUIFrame::GetEventsListener()
{
    return listener;
}
