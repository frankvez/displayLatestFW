/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
     //displayed information product+latest fw
    std::vector<std::pair<String,String>> getLatestFirmwareVersionsVector(String url);
    /** Returns the latest parameter versions for all the products from an URL-based JSON structure.
     @param url web address of the .json data sctructure
     @param parameter attribute for where to find the latest version i.e. firmware, Mac App, Windows Drivers..
     @param strict switches between strict and loose comparison when looking for the parameter inside the JSON structure
     @return vector[i].first = product name , vector[i].second = parameter version, error message if N/A 
    */
    std::vector<std::pair<String,String>> getLatestVersions(String url, String parameter, bool strict);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
