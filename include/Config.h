#pragma once
#include "AnimationController.h"

namespace Mus {
    class Config {
    public:
        [[nodiscard]] static Config& GetSingleton() {
            static Config instance;
            return instance;
        };

        bool LoadConfig();
        bool LoadExtraConfig();

        //Debug
        [[nodiscard]] inline spdlog::level::level_enum GetLogLevel() const noexcept {
            return logLevel;
        }

        [[nodiscard]] inline spdlog::level::level_enum GetFlushLevel() const noexcept {
            return flushLevel;
        }

        //Feature
        [[nodiscard]] inline bool GetRaceController() const noexcept {
            return RaceController;
        }
        [[nodiscard]] inline bool GetBeforeSaveCompatible() const noexcept {
            return BeforeSaveCompatible;
        }
        [[nodiscard]] inline  std::vector<std::string> GetExceptionHeadParts() const noexcept {
            return ExceptionHeadParts;
        }
        [[nodiscard]] inline bool GetCompatibleHumanoidVampireLord() const noexcept {
            return CompatibleHumanoidVampireLord;
        }
        [[nodiscard]] inline  bool GetEnableEmotion() const noexcept {
            return EnableEmotion;
        }
        [[nodiscard]] inline  bool GetEmotionEyes() const noexcept {
            return EmotionEyes;
        }
        [[nodiscard]] inline  bool GetEmotionIcons() const noexcept {
            return EmotionIcons;
        }
        [[nodiscard]] inline  bool GetEmotionTears() const noexcept {
            return EmotionTears;
        }
        [[nodiscard]] inline  bool GetEmotionHeadOverlay() const noexcept {
            return EmotionHeadOverlay;
        }
        [[nodiscard]] inline  bool GetEmotionHeadOverlayOnlyPlayer() const noexcept {
            return EmotionHeadOverlayOnlyPlayer;
        }
        [[nodiscard]] inline  std::uint8_t GetEmotionEffectActiveThreshold() const noexcept {
            return EmotionEffectActiveThreshold;
        }
        [[nodiscard]] inline  clock_t GetEmotionScanDelay() const noexcept {
            return EmotionScanDelay;
        }
        [[nodiscard]] inline  clock_t GetEmotionForceResetTime() const noexcept {
            return EmotionForceResetTime;
        }
        [[nodiscard]] inline  clock_t GetVanillaScale() const noexcept {
            return VanillaScale;
        }

        //Animation
        [[nodiscard]] inline bool GetElinAnimation() const noexcept {
            return ElinAnimation;
        }
        [[nodiscard]] inline bool GetElinEarsAnimation() const noexcept {
            return ElinEarsAnimation;
        }
        [[nodiscard]] inline bool GetElinTailAnimation() const noexcept {
            return ElinTailAnimation;
        }
        [[nodiscard]] inline bool GetDisableOnRaceSexMenu() const noexcept {
            return DisableOnRaceSexMenu;
        }
        [[nodiscard]] inline bool GetEnablePlayer() const noexcept {
            return EnablePlayer;
        }
        [[nodiscard]] inline bool GetEnableNPCs() const noexcept {
            return EnableNPCs;
        }
        [[nodiscard]] inline  std::uint8_t GetAnimationEarsSpeed() const noexcept {
            return AnimationEarsSpeed;
        }
        [[nodiscard]] inline  std::uint8_t GetAnimationTailSpeed() const noexcept {
            return AnimationTailSpeed;
        }
        [[nodiscard]] inline  bool GetReversed() const noexcept {
            return Reversed;
        }

        //DirectControll
        [[nodiscard]] inline uint32_t GetLEarHotkey() const noexcept {
            return LEarHotkey;
        }
        [[nodiscard]] inline uint32_t GetREarHotkey() const noexcept {
            return REarHotkey;
        }
        [[nodiscard]] inline uint32_t GetTailHotkey() const noexcept {
            return TailHotkey;
        }

        //RandomeControll
        [[nodiscard]] inline float GetFrequencyMax() const noexcept {
            return FrequencyMax;
        }
        [[nodiscard]] inline float GetFrequencyMin() const noexcept {
            return FrequencyMin;
        }

        //EmotionControll
        [[nodiscard]] inline  std::uint8_t GetEmotionAnimationActiveThreshold() const noexcept {
            return EmotionAnimationActiveThreshold;
        }
        [[nodiscard]] inline animation_type GetDialogueAnger() const noexcept {
            return DialogueAnger;
        }
        [[nodiscard]] inline animation_type GetDialogueFear() const noexcept {
            return DialogueFear;
        }
        [[nodiscard]] inline animation_type GetDialogueHappy() const noexcept {
            return DialogueHappy;
        }
        [[nodiscard]] inline animation_type GetDialogueSad() const noexcept {
            return DialogueSad;
        }
        [[nodiscard]] inline animation_type GetDialogueSurprise() const noexcept {
            return DialogueSurprise;
        }
        [[nodiscard]] inline animation_type GetDialoguePuzzled() const noexcept {
            return DialoguePuzzled;
        }
        [[nodiscard]] inline animation_type GetDialogueDisgust() const noexcept {
            return DialogueDisgust;
        }
        [[nodiscard]] inline animation_type GetMoodAnger() const noexcept {
            return MoodAnger;
        }
        [[nodiscard]] inline animation_type GetMoodFear() const noexcept {
            return MoodFear;
        }
        [[nodiscard]] inline animation_type GetMoodHappy() const noexcept {
            return MoodHappy;
        }
        [[nodiscard]] inline animation_type GetMoodSad() const noexcept {
            return MoodSad;
        }
        [[nodiscard]] inline animation_type GetMoodSurprise() const noexcept {
            return MoodSurprise;
        }
        [[nodiscard]] inline animation_type GetMoodPuzzled() const noexcept {
            return MoodPuzzled;
        }
        [[nodiscard]] inline animation_type GetMoodDisgust() const noexcept {
            return MoodDisgust;
        }

        //Extra
        [[nodiscard]] inline std::vector<RE::BSTArrayBase::size_type> GetLookupSliderIndexs() const noexcept {
            return LookupSliderIndexs;
        }
        [[nodiscard]] inline std::vector<std::string> GetLookupSliderNames() const noexcept {
            return LookupSliderNames;
        }

    private:
        //Debug
        spdlog::level::level_enum logLevel{ spdlog::level::level_enum::info };
        spdlog::level::level_enum flushLevel{ spdlog::level::level_enum::trace };

        //Feature
        bool RaceController = true;
        bool BeforeSaveCompatible = false;
        std::vector<std::string> ExceptionHeadParts;
        bool CompatibleHumanoidVampireLord = true;
        bool EnableEmotion = true;
        bool EmotionEyes = true;
        bool EmotionIcons = true;
        bool EmotionTears = true;
        bool EmotionHeadOverlay = true;
        bool EmotionHeadOverlayOnlyPlayer = false;
        std::uint8_t EmotionEffectActiveThreshold = 70;
        clock_t EmotionScanDelay = 0;
        clock_t EmotionForceResetTime = 30000;
        bool VanillaScale = false;

        //Animation
        bool ElinAnimation = true;
        bool ElinEarsAnimation = true;
        bool ElinTailAnimation = true;
        bool DisableOnRaceSexMenu = true;
        bool EnablePlayer = true;
        bool EnableNPCs = true;
        std::uint8_t AnimationEarsSpeed = 3;
        std::uint8_t AnimationTailSpeed = 3;
        bool Reversed = false;

        //DirectControll
        uint32_t LEarHotkey = 75u;
        uint32_t REarHotkey = 76u;
        uint32_t TailHotkey = 77u;

        //RandomControll
        float FrequencyMax = 7.5;
        float FrequencyMin = 2.5;

        //EmotionControll
        std::uint8_t EmotionAnimationActiveThreshold = 70;
        animation_type DialogueAnger = animation_type::attention;
        animation_type DialogueFear = animation_type::droop;
        animation_type DialogueHappy = animation_type::continueAnim;
        animation_type DialogueSad = animation_type::droop;
        animation_type DialogueSurprise = animation_type::attention;
        animation_type DialoguePuzzled = animation_type::continueAnimL;
        animation_type DialogueDisgust = animation_type::droop;
        animation_type MoodAnger = animation_type::attention;
        animation_type MoodFear = animation_type::droop;
        animation_type MoodHappy = animation_type::continueAnim;
        animation_type MoodSad = animation_type::droop;
        animation_type MoodSurprise = animation_type::attention;
        animation_type MoodPuzzled = animation_type::continueAnimL;
        animation_type MoodDisgust = animation_type::droop;

        //Extra
        std::vector<RE::BSTArrayBase::size_type> LookupSliderIndexs;
        std::vector<std::string> LookupSliderNames;

    protected:
        // trim from start (in place)
        inline void ltrim(std::string& s)
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                [](int c) {return !std::isspace(c); }));
        }

        // trim from end (in place)
        inline void rtrim(std::string& s)
        {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                [](int c) {return !std::isspace(c); }).base(), s.end());
        }

        // trim from both ends (in place)
        inline void trim(std::string& s)
        {
            ltrim(s);
            rtrim(s);
        }

        inline std::string trim_copy(std::string s)
        {
            trim(s);
            return s;
        }

        inline std::vector<std::string> split(const std::string& s, char delimiter)
        {
            std::string str = trim_copy(s);

            std::vector<std::string> tokens;
            if (!str.empty())
            {
                std::string token;
                std::istringstream tokenStream(str);
                while (std::getline(tokenStream, token, delimiter))
                {
                    trim(token);
                    tokens.emplace_back(token);
                }
            }
            return tokens;
        }

        inline void skipComments(std::string& str)
        {
            auto pos = str.find("#");
            if (pos != std::string::npos)
            {
                str.erase(pos);
            }
        }

        inline std::string GetConfigSettingsStringValue(std::string line, std::string& variable)
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

        inline RE::FormID getHex(std::string hexstr)
        {
            return (RE::FormID)strtoul(hexstr.c_str(), 0, 16);
        }

        inline int GetConfigSettingsIntValue(std::string valuestr)
        {
            int value = 0;
            value = std::stoi(valuestr);
            return value;
        }

        inline float GetConfigSettingsFloatValue(std::string valuestr)
        {
            float value = 0;
            value = strtof(valuestr.c_str(), 0);
            return value;
        }

        inline bool GetConfigSettingsBoolValue(std::string valuestr)
        {
            bool value = false;
            valuestr = lowLetter(valuestr.c_str());
            value = (valuestr.compare("true") == 0);
            return value;
        }

        inline RE::FormID GetConfigSettingsFormIDValue(std::string valuestr)
        {
            RE::FormID value;
            value = getHex(valuestr.c_str());
            return value;
        }

        inline std::vector<RE::FormID> ConfigLineSplitterFormID(std::string valuestr)
        {
            std::vector<std::string> SplittedFormID = split(valuestr, '|');
            std::vector<RE::FormID> value;
            for (size_t index = 0; index < SplittedFormID.size(); index++)
            {
                trim(SplittedFormID[index]);
                value.emplace_back(getHex(SplittedFormID[index].c_str()));
            }
            return value;
        }
    };

    class MultipleConfig : public Config {
    public:
        bool LoadElinAnimationConfig();


        static inline ControllerConfig GetConfigDefault()
        {
            ControllerConfig cc;
            cc.FrequencyMax = Config::GetSingleton().GetFrequencyMax();
            cc.FrequencyMin = Config::GetSingleton().GetFrequencyMin();
            cc.AnimationEarsSpeed = Config::GetSingleton().GetAnimationEarsSpeed();
            cc.AnimationTailSpeed = Config::GetSingleton().GetAnimationTailSpeed();
            cc.Reversed = Config::GetSingleton().GetReversed();
            cc.EmotionEffectActiveThreshold = Config::GetSingleton().GetEmotionEffectActiveThreshold();
            cc.EmotionAnimationActiveThreshold = Config::GetSingleton().GetEmotionAnimationActiveThreshold();
            cc.DialogueAnger = Config::GetSingleton().GetDialogueAnger();
            cc.DialogueFear = Config::GetSingleton().GetDialogueFear();
            cc.DialogueHappy = Config::GetSingleton().GetDialogueHappy();
            cc.DialogueSad = Config::GetSingleton().GetDialogueSad();
            cc.DialogueSurprise = Config::GetSingleton().GetDialogueSurprise();
            cc.DialoguePuzzled = Config::GetSingleton().GetDialoguePuzzled();
            cc.DialogueDisgust = Config::GetSingleton().GetDialogueDisgust();
            cc.MoodAnger = Config::GetSingleton().GetMoodAnger();
            cc.MoodFear = Config::GetSingleton().GetMoodFear();
            cc.MoodHappy = Config::GetSingleton().GetMoodHappy();
            cc.MoodSad = Config::GetSingleton().GetMoodSad();
            cc.MoodSurprise = Config::GetSingleton().GetMoodSurprise();
            cc.MoodPuzzled = Config::GetSingleton().GetMoodPuzzled();
            cc.MoodDisgust = Config::GetSingleton().GetMoodDisgust();
            return cc;
        }
    private:
        inline std::vector<std::string> get_all_files_names_within_folder(std::string folder)
        {
            std::vector<std::string> names;

            DIR* directory = opendir(folder.c_str());
            struct dirent* direntStruct;

            if (directory != nullptr) {
                while (direntStruct = readdir(directory)) {
                    names.emplace_back(direntStruct->d_name);
                }
            }
            closedir(directory);

            return names;
        }

        inline bool stringStartsWith(std::string str, std::string prefix)
        {
            // convert string to back to lower case
            std::for_each(str.begin(), str.end(), [](char& c)
                {
                    c = ::tolower(c);
                });
            // std::string::find returns 0 if toMatch is found at starting
            if (str.find(prefix) == 0)
                return true;
            else
                return false;
        }

        inline bool stringEndsWith(std::string str, std::string const& suffix)
        {
            std::for_each(str.begin(), str.end(), [](char& c)
                {
                    c = ::tolower(c);
                });
            if (str.length() >= suffix.length())
            {
                return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
            }
            else
            {
                return false;
            }
        }
    };
}
