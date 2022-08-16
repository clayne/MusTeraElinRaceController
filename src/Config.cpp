#include "Config.h"

#include <articuno/archives/ryml/ryml.h>
#include <direct.h>

using namespace articuno::ryml;
using namespace Mus;

namespace Mus {
    bool Config::LoadConfig() {
        std::string configPath = GetRuntimeDirectory();
        configPath += "MusTeraElinRaceController.ini";

        std::ifstream file(configPath);

        if (!file.is_open())
        {
            std::transform(configPath.begin(), configPath.end(), configPath.begin(), ::tolower);
            file.open(configPath);
        }

        if (!file.is_open())
        {
            SKSE::stl::report_and_fail("Unable to load Config file.");
            return false;
        }

        std::string line;
        std::string currentSetting;
        while (std::getline(file, line))
        {
            //trim(line);
            skipComments(line);
            trim(line);
            if (line.length() > 0)
            {
                if (line.substr(0, 1) == "[")
                {
                    currentSetting = line;
                }
                else
                {
                    std::string variableName;
                    std::string variableValue = GetConfigSettingsStringValue(line, variableName);
                    if (currentSetting == "[Debug]")
                    {
                        if (variableName == "logLevel")
                        {
                            logLevel = spdlog::level::from_str(variableValue);
                        }
                        if (variableName == "flushLevel")
                        {
                            flushLevel = spdlog::level::from_str(variableValue);
                        }
                    }
                    else if (currentSetting == "[Feature]")
                    {
                        if (variableName == "RaceController")
                        {
                            RaceController = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "BeforeSaveCompatible")
                        {
                            BeforeSaveCompatible = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "ExceptionHeadParts")
                        {
                            ExceptionHeadParts = split(variableValue, '|');
                        }
                        else if (variableName == "CompatibleHumanoidVampireLord")
                        {
                            CompatibleHumanoidVampireLord = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EnableEmotion")
                        {
                            EnableEmotion = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EmotionEyes")
                        {
                            EmotionEyes = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EmotionIcons")
                        {
                            EmotionIcons = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EmotionHeadOverlay")
                        {
                            EmotionHeadOverlay = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EmotionHeadOverlayOnlyPlayer")
                        {
                            EmotionHeadOverlayOnlyPlayer = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EmotionEffectActiveThreshold")
                        {
                            EmotionEffectActiveThreshold = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "EmotionScanDelay")
                        {
                            EmotionScanDelay = GetConfigSettingsIntValue(line, variableName);
                        }
                    }
                    else if (currentSetting == "[Animation]")
                    {
                        if (variableName == "ElinAnimation")
                        {
                            ElinAnimation = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "DisableOnRaceSexMenu")
                        {
                            DisableOnRaceSexMenu = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EnablePlayer")
                        {
                            EnablePlayer = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "EnableNPCs")
                        {
                            EnableNPCs = GetConfigSettingsBoolValue(line, variableName);
                        }
                        else if (variableName == "AnimationEarsSpeed")
                        {
                            AnimationEarsSpeed = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "AnimationTailSpeed")
                        {
                            AnimationTailSpeed = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "Reversed")
                        {
                            Reversed = GetConfigSettingsBoolValue(line, variableName);
                        }
                    }
                    else if (currentSetting == "[DirectControll]")
                    {
                        if (variableName == "LEarHotkey")
                        {
                            LEarHotkey = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "REarHotkey")
                        {
                            REarHotkey = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "TailHotkey")
                        {
                            TailHotkey = GetConfigSettingsIntValue(line, variableName);
                        }
                    }
                    else if (currentSetting == "[RandomeControll]")
                    {
                        if (variableName == "FrequencyMax")
                        {
                            FrequencyMax = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "FrequencyMin")
                        {
                            FrequencyMin = GetConfigSettingsIntValue(line, variableName);
                        }
                    }
                    else if (currentSetting == "[EmotionControll]")
                    {
                        if (variableName == "EmotionAnimationActiveThreshold")
                        {
                            EmotionAnimationActiveThreshold = GetConfigSettingsIntValue(line, variableName);
                        }
                        else if (variableName == "DialogueAnger")
                        {
                            DialogueAnger = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "DialogueFear")
                        {
                            DialogueFear = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "DialogueHappy")
                        {
                            DialogueHappy = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "DialogueSad")
                        {
                            DialogueSad = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "DialogueSurprise")
                        {
                            DialogueSurprise = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "DialoguePuzzled")
                        {
                            DialoguePuzzled = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "DialogueDisgust")
                        {
                            DialogueDisgust = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodAnger")
                        {
                            MoodAnger = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodFear")
                        {
                            MoodFear = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodHappy")
                        {
                            MoodHappy = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodSad")
                        {
                            MoodSad = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodSurprise")
                        {
                            MoodSurprise = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodPuzzled")
                        {
                            MoodPuzzled = magic_enum::enum_cast<animation_type>(line).value();
                        }
                        else if (variableName == "MoodDisgust")
                        {
                            MoodDisgust = magic_enum::enum_cast<animation_type>(line).value();
                        }
                    }
                    else if (currentSetting == "[Extra]")
                    {
                        if (variableName == "LookupSliderIndexs")
                        {
                            std::vector<std::string> _LookupSliderIndexlist = split(variableValue, '|');
                            for (auto& list : _LookupSliderIndexlist)
                            {
                                RE::BSTArrayBase::size_type index = std::stoi(list);
                                LookupSliderIndexs.emplace_back(index);
                            }
                        }
                        else if (variableName == "LookupSliderNames")
                        {
                            LookupSliderNames = split(variableValue, '|');
                        }
                    }
                }
            }
        }

        return true;
    }


    bool MultipleConfig::LoadElinAnimationConfig() {
        auto& am = ActorManager::GetSingleton();

        char RuntimeDirectory[4096];
        if (_getcwd(RuntimeDirectory, 4096) == NULL) 
        {
            logger::error("Can't get the RuntimeDirectory");
            return false;
        }

        std::string configPath = RuntimeDirectory;
        configPath += "\\Data\\SKSE\\Plugins\\MusTeraElinRaceController\\";

        auto configList = get_all_files_names_within_folder(configPath.c_str());
        concurrency::parallel_for(std::size_t(0), configList.size(), [&](std::size_t i)
        {
            std::string filename = configList.at(i);

            if (filename != "." && filename != "..")
            {

                if (stringEndsWith(filename, ".ini"))
                {

                    logger::info("File found: {}", filename);

                    std::string filepath = configPath;
                    filepath.append(filename);
                    std::ifstream file(filepath);

                    if (!file.is_open())
                    {
                        transform(filepath.begin(), filepath.end(), filepath.begin(), ::tolower);
                        file.open(filepath);
                    }

                    if (file.is_open())
                    {

                        std::string pluginname;
                        std::vector<RE::FormID> baseid_list;
                        ControllerConfig cc = GetConfigDefault();

                        std::string line;
                        std::string currentSetting;
                        while (std::getline(file, line))
                        {
                            //trim(line);
                            skipComments(line);
                            trim(line);
                            if (line.length() > 0)
                            {
                                if (line.substr(0, 1) == "[")
                                {
                                    currentSetting = line;
                                }
                                else
                                {
                                    std::string variableName;
                                    std::string variableValue = GetConfigSettingsStringValue(line, variableName);
                                    if (variableName == "PluginName")
                                    {
                                        pluginname = variableValue;
                                    }
                                    else if (variableName == "ActorBaseID")
                                    {
                                        baseid_list = ConfigLineSplitterFormID(variableValue);
                                    }
                                    else if (variableName == "EmotionEffectActiveThreshold")
                                    {
                                        cc.EmotionEffectActiveThreshold = GetConfigSettingsIntValue(line, variableName);
                                    }
                                    else if (variableName == "FrequencyMax")
                                    {
                                        cc.FrequencyMax = GetConfigSettingsFloatValue(line, variableName);
                                    }
                                    else if (variableName == "FrequencyMin")
                                    {
                                        cc.FrequencyMin = GetConfigSettingsFloatValue(line, variableName);
                                    }
                                    else if (variableName == "Reversed")
                                    {
                                        cc.Reversed = GetConfigSettingsBoolValue(line, variableName);
                                    }
                                    else if (variableName == "EmotionAnimationActiveThreshold")
                                    {
                                        cc.EmotionAnimationActiveThreshold = GetConfigSettingsIntValue(line, variableName);
                                    }
                                    else if (variableName == "AnimationEarsSpeed")
                                    {
                                        cc.AnimationEarsSpeed = GetConfigSettingsIntValue(line, variableName);
                                    }
                                    else if (variableName == "AnimationTailSpeed")
                                    {
                                        cc.AnimationTailSpeed = GetConfigSettingsIntValue(line, variableName);
                                    }
                                    else if (variableName == "DialogueAnger")
                                    {
                                        cc.DialogueAnger = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "DialogueFear")
                                    {
                                        cc.DialogueFear = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "DialogueHappy")
                                    {
                                        cc.DialogueHappy = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "DialogueSad")
                                    {
                                        cc.DialogueSad = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "DialogueSurprise")
                                    {
                                        cc.DialogueSurprise = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "DialoguePuzzled")
                                    {
                                        cc.DialoguePuzzled = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "DialogueDisgust")
                                    {
                                        cc.DialogueDisgust = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodAnger")
                                    {
                                        cc.MoodAnger = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodFear")
                                    {
                                        cc.MoodFear = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodHappy")
                                    {
                                        cc.MoodHappy = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodSad")
                                    {
                                        cc.MoodSad = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodSurprise")
                                    {
                                        cc.MoodSurprise = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodPuzzled")
                                    {
                                        cc.MoodPuzzled = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                    else if (variableName == "MoodDisgust")
                                    {
                                        cc.MoodDisgust = magic_enum::enum_cast<animation_type>(variableValue).value();
                                    }
                                }
                            }
                        }
                        am.InsertTrackingMap(baseid_list, pluginname, cc);
                    }
                }
            }
        });
        return true;
    }
}

