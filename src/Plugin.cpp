#include "MyProcessor.h"
#include "../thirdparty/stack/StackProcessor.h"

#ifdef RNBO_INCLUDE_DESCRIPTION_FILE
#include <rnbo_description.h>
#endif

//This creates new instances of your plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  // return MyProcessor::CreateDefault();
  nlohmann::json patcher_desc, presets;

#ifdef RNBO_BINARY_DATA_STORAGE_NAME
  extern RNBO::BinaryDataImpl::Storage RNBO_BINARY_DATA_STORAGE_NAME;
  RNBO::BinaryDataImpl::Storage dataStorage = RNBO_BINARY_DATA_STORAGE_NAME;
#else
  RNBO::BinaryDataImpl::Storage dataStorage;
#endif
  RNBO::BinaryDataImpl data(dataStorage);

#ifdef RNBO_INCLUDE_DESCRIPTION_FILE
  patcher_desc = RNBO::patcher_description;
  presets = RNBO::patcher_presets;
#endif
  return new MyProcessor(patcher_desc, presets, data);
}


