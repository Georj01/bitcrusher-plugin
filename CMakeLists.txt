cmake_minimum_required(VERSION 3.20)
project(bitcrusher-plugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JUCE CONFIG PATHS "C:/JUCE" NO_DEFAULT_PATH)

if(NOT JUCE_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        8.0.0
    )
    FetchContent_MakeAvailable(juce)
endif()

juce_add_plugin(BitcrusherPlugin
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE Btcr
    FORMATS VST3
    PRODUCT_NAME "Bitcrusher"
)

target_sources(BitcrusherPlugin
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginProcessor.h
        src/PluginEditor.cpp
        src/PluginEditor.h
)

target_link_libraries(BitcrusherPlugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

juce_generate_juce_header(BitcrusherPlugin)
