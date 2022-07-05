#include "Config.h"

#include <articuno/archives/ryml/ryml.h>
#include <direct.h>

using namespace articuno::ryml;
using namespace Mus;

namespace Mus {
    bool AnimController = true;
    bool RaceController = true;

    const Config& Config::GetSingleton() noexcept {
        static Config instance;

        static std::atomic_bool initialized;
        static std::latch latch(1);
        if (!initialized.exchange(true)) {
            std::ifstream inputFile(R"(Data\SKSE\Plugins\MusTeraElinRaceController.yaml)");
            if (inputFile.good()) {
                yaml_source ar(inputFile);
                ar >> instance;
            }
            latch.count_down();
        }
        latch.wait();

        return instance;
    }


    bool MultipleConfig::LoadElinAnimationConfig() {
        char RuntimeDirectory[4096];
        if (_getcwd(RuntimeDirectory, 4096) == NULL) 
        {
            logger::error("Can't get the RuntimeDirectory");
            return false;
        }

        std::string configPath = RuntimeDirectory;
        configPath += "\\Data\\SKSE\\Plugins\\MusTeraElinRaceController\\";

        auto configList = get_all_files_names_within_folder(configPath.c_str());
        for (std::size_t i = 0; i < configList.size(); i++)
        {
            std::string filename = configList.at(i);

            if (filename == "." || filename == "..")
                continue;

            if (!stringEndsWith(filename, ".ini"))
                continue;

            logger::info("File found: {}", filename);

            std::string filepath = configPath;
            filepath.append(filename);
            std::ifstream file(filepath);

            if (!file.is_open())
            {
                transform(filepath.begin(), filepath.end(), filepath.begin(), ::tolower);
                file.open(filepath);
            }

            if (!file.is_open())
                continue;

            std::string pluginname;
            uint32_t baseid = 0;
            ControllerConfig cc;
            cc.FrequencyMax = Config::GetSingleton().GetSetting().GetAnimation().GetRandomControl().GetFrequencyMax();
            cc.FrequencyMin = Config::GetSingleton().GetSetting().GetAnimation().GetRandomControl().GetFrequencyMin();
            cc.Reversed = false;
            cc.AnimationSpeed = Config::GetSingleton().GetSetting().GetAnimation().GetAnimationSpeed();

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
                            baseid = GetConfigSettingsHexValue(line, variableName);
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
                        else if (variableName == "AnimationSpeed")
                        {
                            cc.AnimationSpeed = GetConfigSettingsValue(line, variableName);;
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
        }
        return true;
    }

    int MultipleConfig::GetConfigSettingsValue(std::string line, std::string& variable) 
    {
        int value = 0;
        std::vector<std::string> splittedLine = split(line, '=');
        variable = "";
        if (splittedLine.size() > 1)
        {
            variable = splittedLine[0];
            trim(variable);

            std::string valuestr = splittedLine[1];
            trim(valuestr);
            value = std::stoi(valuestr);
        }
        return value;
    }

    float MultipleConfig::GetConfigSettingsFloatValue(std::string line, std::string& variable)
    {
        float value = 0;
        std::vector<std::string> splittedLine = split(line, '=');
        variable = "";
        if (splittedLine.size() > 1)
        {
            variable = splittedLine[0];
            trim(variable);

            std::string valuestr = splittedLine[1];
            trim(valuestr);
            value = strtof(valuestr.c_str(), 0);
        }
        return value;
    }

    bool MultipleConfig::GetConfigSettingsBoolValue(std::string line, std::string& variable)
    {
        bool value = false;
        std::vector<std::string> splittedLine = split(line, '=');
        variable = "";
        if (splittedLine.size() > 1)
        {
            variable = splittedLine[0];
            trim(variable);

            std::string valuestr = splittedLine[1];
            trim(valuestr);
            value = (_stricmp(variable.c_str(), "true") == 0);
        }
        return value;
    }

    std::string MultipleConfig::GetConfigSettingsStringValue(std::string line, std::string& variable)
    {
        std::string value = "";
        std::vector<std::string> splittedLine = split(line, '=');
        variable = "";
        if (splittedLine.size() > 1)
        {
            variable = splittedLine[0];
            trim(variable);

            std::string valuestr = splittedLine[1];
            trim(valuestr);
            value = valuestr;
        }
        return value;
    }

    std::uint32_t MultipleConfig::GetConfigSettingsHexValue(std::string line, std::string& variable)
    {
        std::uint32_t value;
        std::vector<std::string> splittedLine = split(line, '=');
        variable = "";
        if (splittedLine.size() > 1)
        {
            variable = splittedLine[0];
            trim(variable);

            std::string valuestr = splittedLine[1];
            trim(valuestr);
            value = GetFormIdFromString(valuestr);
        }
        return value;
    }

    std::uint32_t MultipleConfig::GetFormIdFromString(std::string str)
    {
        return getHex(str.c_str());
    }
}

