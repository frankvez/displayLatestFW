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
    String parameter = "firmware"; //strict = true will return the latest v1 while strict = false will return the latest v2
    //Other parameters to play with
    //String parameter = "firmware-v2";
    // String parameter = "Mac app";
    // String parameter = "documentation";
    // String parameter = "libusb";
    //String parameter = "id app for mac";
    //String parameter = "LibUSB 10.7/10.8";
    String jsonUrl = "https://update.audient.com/products.json";
    try{
        auto versions = getLatestVersions(jsonUrl, parameter, true);
        g.drawText("Latest "+parameter+" version available", 80, 20, 350, 25, Justification::centred);
        if (!versions.empty()){
            for (int i = 0 ; i< versions.size(); i++){
                g.drawText(versions[i].first, 20 , 90 + (i * 50), 350, 20, Justification::left);
                g.drawText(versions[i].second, 200, 90 + (i * 50), 350, 20, Justification::left);
            }
        }
    } catch (const char* msg){
        DBG(msg);
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

std::vector<std::pair<String,String>> MainComponent::getLatestVersions(String url, String parameter, bool strict){
    std::vector<std::pair<String,String>> Output;
    URL jsonURL (url);
    var jsonObjects =JSON::parse(jsonURL.readEntireTextStream());
    if (jsonObjects.hasProperty("products")){
        auto ProductsList = jsonObjects.getProperty("products", var()).getArray();
        String productName;
            for (auto& prd : *ProductsList){
                if (prd.hasProperty("components")){
                    auto components = prd.getProperty("components", var()).getArray();
                    productName = prd.getProperty("name", var()).toString();
                    bool hasparameter = false;
                    String version;
                    if (!(components->isEmpty())){
                        for (auto& cmp : *components){
                            if (!strict && cmp.getProperty("name", var()).toString().containsIgnoreCase(parameter)){
                                hasparameter = true;
                                version = cmp.getProperty("latest_release", var()).getProperty("version", var()).toString(); // will display the latest fw v
                                } else if (strict && cmp.getProperty("name", var()).toString().compareIgnoreCase(parameter) == 0){ //JUCE's compareIgnoreCase returns 0 when the two strings are identical
                                    hasparameter = true;
                                    version = cmp.getProperty("latest_release", var()).getProperty("version", var()).toString();
                            }
                        }
                        if (hasparameter){
                            Output.emplace_back(productName,version);
                        } else {
                            Output.emplace_back(productName,"No "+parameter+ " detected");
                        }
                    }
                } else{
                    throw "Key 'components' missing";
                }
                }
            return Output;
    } else{
        throw "Key 'products' missing";
    }
}
