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
            if (line.length() <= 0)
                continue;

            if (line.substr(0, 1) == "[")
            {
                currentSetting = line;
                continue;
            }
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
                    RaceController = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "BeforeSaveCompatible")
                {
                    BeforeSaveCompatible = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "ExceptionHeadParts")
                {
                    ExceptionHeadParts = split(variableValue, '|');
                }
                else if (variableName == "CompatibleHumanoidVampireLord")
                {
                    CompatibleHumanoidVampireLord = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EnableEmotion")
                {
                    EnableEmotion = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EmotionEyes")
                {
                    EmotionEyes = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EmotionIcons")
                {
                    EmotionIcons = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EmotionHeadOverlay")
                {
                    EmotionHeadOverlay = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EmotionHeadOverlayOnlyPlayer")
                {
                    EmotionHeadOverlayOnlyPlayer = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EmotionEffectActiveThreshold")
                {
                    EmotionEffectActiveThreshold = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "EmotionScanDelay")
                {
                    EmotionScanDelay = GetConfigSettingsFloatValue(variableValue) * 1000;
                }
                else if (variableName == "EmotionForceResetTime")
                {
                    EmotionForceResetTime = GetConfigSettingsFloatValue(variableValue) * 1000;
                }
                else if (variableName == "VanillaScale")
                {
                    VanillaScale = GetConfigSettingsBoolValue(variableValue);
                }
            }
            else if (currentSetting == "[Animation]")
            {
                if (variableName == "ElinAnimation")
                {
                    ElinAnimation = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "DisableOnRaceSexMenu")
                {
                    DisableOnRaceSexMenu = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EnablePlayer")
                {
                    EnablePlayer = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "EnableNPCs")
                {
                    EnableNPCs = GetConfigSettingsBoolValue(variableValue);
                }
                else if (variableName == "AnimationEarsSpeed")
                {
                    AnimationEarsSpeed = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "AnimationTailSpeed")
                {
                    AnimationTailSpeed = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "Reversed")
                {
                    Reversed = GetConfigSettingsBoolValue(variableValue);
                }
            }
            else if (currentSetting == "[DirectControll]")
            {
                if (variableName == "LEarHotkey")
                {
                    LEarHotkey = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "REarHotkey")
                {
                    REarHotkey = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "TailHotkey")
                {
                    TailHotkey = GetConfigSettingsIntValue(variableValue);
                }
            }
            else if (currentSetting == "[RandomeControll]")
            {
                if (variableName == "FrequencyMax")
                {
                    FrequencyMax = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "FrequencyMin")
                {
                    FrequencyMin = GetConfigSettingsIntValue(variableValue);
                }
            }
            else if (currentSetting == "[EmotionControll]")
            {
                if (variableName == "EmotionAnimationActiveThreshold")
                {
                    EmotionAnimationActiveThreshold = GetConfigSettingsIntValue(variableValue);
                }
                else if (variableName == "DialogueAnger")
                {
                    DialogueAnger = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "DialogueFear")
                {
                    DialogueFear = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "DialogueHappy")
                {
                    DialogueHappy = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "DialogueSad")
                {
                    DialogueSad = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "DialogueSurprise")
                {
                    DialogueSurprise = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "DialoguePuzzled")
                {
                    DialoguePuzzled = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "DialogueDisgust")
                {
                    DialogueDisgust = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodAnger")
                {
                    MoodAnger = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodFear")
                {
                    MoodFear = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodHappy")
                {
                    MoodHappy = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodSad")
                {
                    MoodSad = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodSurprise")
                {
                    MoodSurprise = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodPuzzled")
                {
                    MoodPuzzled = magic_enum::enum_cast<animation_type>(variableValue).value();
                }
                else if (variableName == "MoodDisgust")
                {
                    MoodDisgust = magic_enum::enum_cast<animation_type>(variableValue).value();
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
        return true;
    }

    bool Config::LoadExtraConfig() {
        std::string ExtraConfigPath = GetRuntimeDirectory();
        ExtraConfigPath += "MusTeraElinRaceControllerExtraConfig.ini";

        std::ifstream file(ExtraConfigPath);

        if (!file.is_open())
        {
            std::transform(ExtraConfigPath.begin(), ExtraConfigPath.end(), ExtraConfigPath.begin(), ::tolower);
            file.open(ExtraConfigPath);
        }

        if (!file.is_open())
        {
            logger::error("Unable to load Extra Config file.");
            return false;
        }

        std::string line;
        std::string currentSetting;
        while (std::getline(file, line))
        {
            //trim(line);
            skipComments(line);
            trim(line);
            if (line.length() <= 0)
                continue;

            if (line.substr(0, 1) == "[")
            {
                currentSetting = line;
                continue;
            }
            std::string variableName;
            std::string variableValue = GetConfigSettingsStringValue(line, variableName);

            if (currentSetting == "[VanillaScale]")
            {
                if (variableName == "VanillaScale")
                {
                    VanillaScale = GetConfigSettingsBoolValue(variableValue);
                    logger::info("Player scale is set to the {}", VanillaScale ? "Vanilla Scale" : "Elin Scale");
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
                                        cc.EmotionEffectActiveThreshold = GetConfigSettingsIntValue(variableValue);
                                    }
                                    else if (variableName == "FrequencyMax")
                                    {
                                        cc.FrequencyMax = GetConfigSettingsFloatValue(variableValue);
                                    }
                                    else if (variableName == "FrequencyMin")
                                    {
                                        cc.FrequencyMin = GetConfigSettingsFloatValue(variableValue);
                                    }
                                    else if (variableName == "Reversed")
                                    {
                                        cc.Reversed = GetConfigSettingsBoolValue(variableValue);
                                    }
                                    else if (variableName == "EmotionAnimationActiveThreshold")
                                    {
                                        cc.EmotionAnimationActiveThreshold = GetConfigSettingsIntValue(variableValue);
                                    }
                                    else if (variableName == "AnimationEarsSpeed")
                                    {
                                        cc.AnimationEarsSpeed = GetConfigSettingsIntValue(variableValue);
                                    }
                                    else if (variableName == "AnimationTailSpeed")
                                    {
                                        cc.AnimationTailSpeed = GetConfigSettingsIntValue(variableValue);
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

