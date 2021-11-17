cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

set (ProjectName ci-plugin-demo)
set (ProjectVersion 1.0.0)

project(${ProjectName} VERSION ${ProjectVersion})

# By default we don't want Xcode schemes to be made for modules, etc
set(CMAKE_XCODE_GENERATE_SCHEME OFF)

# No ZERO_CHECK target on XCode (it helps bust cache for cmake)
set(CMAKE_SUPPRESS_REGENERATION true)

# Setting this locally will increase build times
# set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)

# Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Create a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

add_subdirectory(JUCE)

juce_add_plugin(${ProjectName}
	# ICON_BIG ...
	# ICON_SMALL ...
	# COMPANY_WEBSITE ...
	# COMPANY_EMAIL ...
	COMPANY_NAME "ADCWorkshop"
	IS_SYNTH FALSE
	NEEDS_MIDI_INPUT FALSE
	NEEDS_MIDI_OUTPUT FALSE
	IS_MIDI_EFFECT FALSE
	EDITOR_WANTS_KEYBOARD_FOCUS FALSE
	COPY_PLUGIN_AFTER_BUILD FALSE
	PLUGIN_MANUFACTURER_CODE Juce
	PLUGIN_CODE Dem0
	FORMATS AU VST3 Standalone
	VST3_CATEGORIES Fx
	AU_MAIN_TYPE kAudioUnitType_Effect
	AAX_CATEGORY AAX_ePlugInCategory_Effect
	PRODUCT_NAME ${ProjectName})

juce_generate_juce_header(${ProjectName})

# Add your source files here
set(SourceFiles
	Source/PluginProcessor.h
	Source/PluginProcessor.cpp
	Source/PluginEditor.h
	Source/PluginEditor.cpp)

target_sources(${ProjectName} PRIVATE ${SourceFiles})

# Organizing the source tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX Source FILES ${SourceFiles})

# Move the INTERFACE auto-created JUCE library stuff into its own folder
source_group("JUCE Library Code" REGULAR_EXPRESSION "juce_")

target_compile_definitions(${ProjectName}
	PUBLIC
	DONT_SET_USING_JUCE_NAMESPACE=0
	JUCE_DONT_DECLARE_PROJECTINFO=0
	JUCE_DISPLAY_SPLASH_SCREEN=0
	JUCE_WEB_BROWSER=0
	JUCE_USE_CURL=0
	JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${ProjectName} PRIVATE
	juce::juce_audio_utils
	juce::juce_dsp
	PUBLIC
	juce::juce_recommended_config_flags
	juce::juce_recommended_lto_flags
	juce::juce_recommended_warning_flags)

file(WRITE version ${ProjectVersion})