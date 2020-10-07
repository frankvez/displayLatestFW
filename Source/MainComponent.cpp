/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 800);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    std::vector<std::pair<String,String>> latestFw = getLatestFirmwareVersionsVector("https://update.audient.com/products.json");
    g.drawText("Latest firmware availables", 200, 20, 250, 25, Justification::centred);
    for (int i = 0 ; i< latestFw.size(); i++){
        g.drawText(latestFw[i].first, 20 , 90 + (i * 50), 200, 20, Justification::centred);
        g.drawText(latestFw[i].second, 200, 90 + (i * 50), 200, 20, Justification::centred);
    }
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
std::vector<std::pair<String,String>> MainComponent::getLatestFirmwareVersionsVector(String url){
    std::vector<std::pair<String,String>> Output;
    //URL jsonURL ("https://update.audient.com/products.json");
    URL jsonURL (url);
    String jsonContent = jsonURL.readEntireTextStream();
    var jsonObjects =JSON::parse(jsonContent);
    Array<var>* ProductsList = jsonObjects.getProperty("products", var()).getArray();
    String productName;
    for (auto& cmp : *ProductsList){
        productName = cmp.getProperty("name", var()).toString();
        //We got a product
        DBG(productName);
        std::vector<std::pair<int,String>> versions;
        Array<var>* components = cmp.getProperty("components", var()).getArray();
        for (auto& rls : *components){
            Array<var>* releases = rls.getProperty("releases", var()).getArray();
            for (auto& vrsn : *releases){
                //iterating through fw releases
                String version = vrsn.getProperty("version", var()).toString();
                String vrsedit = version.removeCharacters(".vbrc"); //ditching the characters for numerical comparsion
                int versionInt = vrsedit.getIntValue();
                versions.emplace_back(versionInt, version);
            }

        }
        // Done with checking versions for the current product, time to find the latest one (higher versionInt)
        sort (versions.begin(), versions.end(), std::greater<>());
        String latestV = versions[0].second; //getting the actual version format x.x.x
        Output.emplace_back(productName,latestV);
    }
    return Output;
}

