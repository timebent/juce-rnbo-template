
# ---------------------------------------------------------------------
# foleys_gui_magic as a JUCE module
# ---------------------------------------------------------------------
juce_add_module(thirdparty/foleys_gui_magic/modules/foleys_gui_magic)

# ---------------------------------------------------------------------
# RNBO (C74) as OBJECT library
# ---------------------------------------------------------------------
set(RNBO_JUCE_PARAM_DEFAULT_NOTIFY 1)
if (NOT PLUGIN_PARAM_DEFAULT_NOTIFY)
  set(RNBO_JUCE_PARAM_DEFAULT_NOTIFY 0)
endif()

set(RNBO_C74_SOURCES
  "${RNBO_CPP_DIR}/RNBO.cpp"
  "${RNBO_CPP_DIR}/adapters/juce/RNBO_JuceAudioProcessorEditor.cpp"
  "${RNBO_CPP_DIR}/adapters/juce/RNBO_JuceAudioProcessor.cpp"
  ${RNBO_CLASS_FILE}
)

add_library(RNBO_C74_OBJ OBJECT ${RNBO_C74_SOURCES})
target_compile_features(RNBO_C74_OBJ PUBLIC cxx_std_17)

# JUCE modules include root for:
#   #include <juce_audio_processors/juce_audio_processors.h>
set(JUCE_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/thirdparty/juce/modules")

target_include_directories(RNBO_C74_OBJ PRIVATE
  ${RNBO_CPP_DIR}/
  ${RNBO_CPP_DIR}/src
  ${RNBO_CPP_DIR}/common/
  ${RNBO_CPP_DIR}/src/3rdparty/
  ${RNBO_CPP_DIR}/adapters/juce/
  "${JUCE_MODULES_DIR}"
)

target_compile_definitions(RNBO_C74_OBJ PUBLIC
  RNBO_JUCE_NO_CREATE_PLUGIN_FILTER=1
  RNBO_JUCE_PARAM_DEFAULT_NOTIFY=${RNBO_JUCE_PARAM_DEFAULT_NOTIFY}
)

# Silence warnings for RNBO (optional)
target_compile_options(RNBO_C74_OBJ PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/w>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
)

# CRITICAL: satisfy JUCE's guard in juce_TargetPlatform.h WITHOUT force-including your plugin header
# This prevents: #error "No global header file was included!"
target_compile_definitions(RNBO_C74_OBJ PRIVATE
  JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
)

# IMPORTANT: DO NOT force-include MyPlugin's generated JuceHeader.h
# It may include project-specific headers like foleys, which RNBO should not depend on.

# ---------------------------------------------------------------------
# Stack as OBJECT library
# ---------------------------------------------------------------------
add_library(Stack_OBJ OBJECT
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stack/StackProcessor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stack/StackEditor.cpp
)

target_compile_features(Stack_OBJ PUBLIC cxx_std_17)

target_compile_options(Stack_OBJ PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/w>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
)

target_include_directories(Stack_OBJ PRIVATE
  ${RNBO_CPP_DIR}/
  ${RNBO_CPP_DIR}/src
  ${RNBO_CPP_DIR}/common/
  ${RNBO_CPP_DIR}/src/3rdparty/
  ${RNBO_CPP_DIR}/adapters/juce/
  "${JUCE_MODULES_DIR}"
  "${CMAKE_CURRENT_LIST_DIR}/thirdparty/foleys_gui_magic/modules"
)
target_compile_definitions(Stack_OBJ PRIVATE
   JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
)
# Link as INTERFACE to provide headers/definitions without compiling the module into this object library
target_link_libraries(Stack_OBJ INTERFACE
  foleys_gui_magic
)

# ---------------------------------------------------------------------
# Plugin target
# ---------------------------------------------------------------------
juce_add_plugin(MyPlugin
  COMPANY_NAME "Thompson"
  BUNDLE_ID "com.georgiasouthern.musictech"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  COPY_PLUGIN_AFTER_BUILD TRUE
  PLUGIN_MANUFACTURER_CODE "Gsou"
  PLUGIN_CODE "St2k"
  FORMATS VST3 AU Standalone
  PRODUCT_NAME "StackTest"
)

set(MY_MAGIC_SRC "${CMAKE_CURRENT_SOURCE_DIR}/resources/magic.xml")
if(NOT EXISTS "${MY_MAGIC_SRC}")
  file(WRITE "${MY_MAGIC_SRC}" "")
  message(STATUS "Created empty magic.xml at ${MY_MAGIC_SRC}")
endif()

juce_add_binary_data(BinaryData
  SOURCES resources/magic.xml
)

juce_generate_juce_header(MyPlugin)

target_sources(MyPlugin PRIVATE
  src/Plugin.cpp
  src/MyProcessor.cpp
  src/MyEditor.cpp
)

if (EXISTS ${RNBO_BINARY_DATA_FILE})
  target_sources(MyPlugin PRIVATE ${RNBO_BINARY_DATA_FILE})
endif()

target_include_directories(MyPlugin PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/
  src/
)

target_include_directories(MyPlugin SYSTEM PUBLIC
  ${RNBO_CPP_DIR}/
  ${RNBO_CPP_DIR}/src
  ${RNBO_CPP_DIR}/common/
  ${RNBO_CPP_DIR}/src/3rdparty/
  ${RNBO_CPP_DIR}/adapters/juce/
)

set(RNBO_JUCE_PARAM_DEFAULT_NOTIFY 1)
if (NOT PLUGIN_PARAM_DEFAULT_NOTIFY)
  set(RNBO_JUCE_PARAM_DEFAULT_NOTIFY 0)
endif()

target_compile_definitions(MyPlugin PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0
  RNBO_JUCE_NO_CREATE_PLUGIN_FILTER=1
  RNBO_JUCE_PARAM_DEFAULT_NOTIFY=${RNBO_JUCE_PARAM_DEFAULT_NOTIFY}
  MY_MAGIC_SRC="${MY_MAGIC_SRC}"
)

# ---------------------------------------------------------------------
# Link everything ONLY at the final plugin target
# ---------------------------------------------------------------------
target_link_libraries(MyPlugin
  PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_formats
    juce::juce_audio_utils
    juce::juce_gui_basics
    juce::juce_gui_extra

    foleys_gui_magic

    RNBO_C74_OBJ
    Stack_OBJ

    BinaryData
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)