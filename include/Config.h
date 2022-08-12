#pragma once
#include "AnimationController.h"

namespace Mus {
    class MultipleConfig;

    class Debug {
    public:
        [[nodiscard]] inline spdlog::level::level_enum GetLogLevel() const noexcept {
            return _logLevel;
        }

        [[nodiscard]] inline spdlog::level::level_enum GetFlushLevel() const noexcept {
            return _flushLevel;
        }

    private:
        articuno_serialize(ar) {
            auto logLevel = spdlog::level::to_string_view(_logLevel);
            auto flushLevel = spdlog::level::to_string_view(_flushLevel);
            ar <=> articuno::kv(logLevel, "logLevel");
            ar <=> articuno::kv(flushLevel, "flushLevel");
        }

        articuno_deserialize(ar) {
            *this = Debug();
            std::string logLevel;
            std::string flushLevel;
            if (ar <=> articuno::kv(logLevel, "logLevel")) {
                _logLevel = spdlog::level::from_str(logLevel);
            }
            if (ar <=> articuno::kv(flushLevel, "flushLevel")) {
                _flushLevel = spdlog::level::from_str(flushLevel);
            }
        }

        spdlog::level::level_enum _logLevel{spdlog::level::level_enum::info};
        spdlog::level::level_enum _flushLevel{spdlog::level::level_enum::trace};

        friend class articuno::access;
    };
    
    class Feature {
    public:
        [[nodiscard]] inline bool GetRaceController() const noexcept {
            return RaceController;
        }
        [[nodiscard]] inline bool GetBeforeSaveCompatible() const noexcept {
            return BeforeSaveCompatible;
        }
        [[nodiscard]] inline  std::string GetExceptionHeadParts() const noexcept {
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

    private:
        articuno_serde(ar) {
            ar <=> articuno::kv(RaceController, "RaceController");
            ar <=> articuno::kv(BeforeSaveCompatible, "BeforeSaveCompatible");
            ar <=> articuno::kv(ExceptionHeadParts, "ExceptionHeadParts");
            ar <=> articuno::kv(CompatibleHumanoidVampireLord, "CompatibleHumanoidVampireLord");
            ar <=> articuno::kv(EnableEmotion, "EnableEmotion");
            ar <=> articuno::kv(EmotionEyes, "EmotionEyes");
            ar <=> articuno::kv(EmotionIcons, "EmotionIcons");
            ar <=> articuno::kv(EmotionTears, "EmotionTears");
            ar <=> articuno::kv(EmotionHeadOverlay, "EmotionHeadOverlay");
            ar <=> articuno::kv(EmotionHeadOverlayOnlyPlayer, "EmotionHeadOverlayOnlyPlayer");
            ar <=> articuno::kv(EmotionEffectActiveThreshold, "EmotionEffectActiveThreshold");
            ar <=> articuno::kv(EmotionScanDelay, "EmotionScanDelay");
        }

        bool RaceController = true;
        bool BeforeSaveCompatible = false;
        std::string ExceptionHeadParts;
        bool CompatibleHumanoidVampireLord = true;
        bool EnableEmotion = true;
        bool EmotionEyes = true;
        bool EmotionIcons = true;
        bool EmotionTears = true;
        bool EmotionHeadOverlay = true;
        bool EmotionHeadOverlayOnlyPlayer = false;
        std::uint8_t EmotionEffectActiveThreshold = 70;
        clock_t EmotionScanDelay = 0;

        friend class articuno::access;
    };

    class DirectControl {
    public:
        [[nodiscard]] inline uint32_t GetLEarHotkey() const noexcept {
            return LEarHotkey;
        }
        [[nodiscard]] inline uint32_t GetREarHotkey() const noexcept {
            return REarHotkey;
        }
        [[nodiscard]] inline uint32_t GetTailHotKey() const noexcept {
            return TailHotKey;
        }

    private:
        articuno_serde(ar) {
            ar <=> articuno::kv(LEarHotkey, "LEarHotkey");
            ar <=> articuno::kv(REarHotkey, "REarHotkey");
            ar <=> articuno::kv(TailHotKey, "TailHotKey");
        }

        uint32_t LEarHotkey = 75u;
        uint32_t REarHotkey = 76u;
        uint32_t TailHotKey = 77u;

        friend class articuno::access;
    };

    class RandomControl {
    public:
        [[nodiscard]] inline float GetFrequencyMax() const noexcept {
            return FrequencyMax;
        }
        [[nodiscard]] inline float GetFrequencyMin() const noexcept {
            return FrequencyMin;
        }

    private:
        articuno_serde(ar) {
            ar <=> articuno::kv(FrequencyMax, "FrequencyMax");
            ar <=> articuno::kv(FrequencyMin, "FrequencyMin");
        }

        float FrequencyMax = 7.5;
        float FrequencyMin = 2.5;

        friend class articuno::access;
    };

    class EmotionControl {
    public:
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

    private:
        articuno_serialize(ar) {
            ar <=> articuno::kv(EmotionAnimationActiveThreshold, "EmotionAnimationActiveThreshold");
            auto _DialogueAnger = magic_enum::enum_name(DialogueAnger);
            auto _DialogueFear = magic_enum::enum_name(DialogueFear);
            auto _DialogueHappy = magic_enum::enum_name(DialogueHappy);
            auto _DialogueSad = magic_enum::enum_name(DialogueSad);
            auto _DialogueSurprise = magic_enum::enum_name(DialogueSurprise);
            auto _DialoguePuzzled = magic_enum::enum_name(DialoguePuzzled);
            auto _DialogueDisgust = magic_enum::enum_name(DialogueDisgust);
            auto _MoodAnger = magic_enum::enum_name(MoodAnger);
            auto _MoodFear = magic_enum::enum_name(MoodFear);
            auto _MoodHappy = magic_enum::enum_name(MoodHappy);
            auto _MoodSad = magic_enum::enum_name(MoodSad);
            auto _MoodSurprise = magic_enum::enum_name(MoodSurprise);
            auto _MoodPuzzled = magic_enum::enum_name(MoodPuzzled);
            auto _MoodDisgust = magic_enum::enum_name(MoodDisgust);
            ar <=> articuno::kv(_DialogueAnger, "DialogueAnger");
            ar <=> articuno::kv(_DialogueFear, "DialogueFear");
            ar <=> articuno::kv(_DialogueHappy, "DialogueHappy");
            ar <=> articuno::kv(_DialogueSad, "DialogueSad");
            ar <=> articuno::kv(_DialogueSurprise, "DialogueSurprise");
            ar <=> articuno::kv(_DialoguePuzzled, "DialoguePuzzled");
            ar <=> articuno::kv(_DialogueDisgust, "DialogueDisgust");
            ar <=> articuno::kv(_MoodAnger, "MoodAnger");
            ar <=> articuno::kv(_MoodFear, "MoodFear");
            ar <=> articuno::kv(_MoodHappy, "MoodHappy");
            ar <=> articuno::kv(_MoodSad, "MoodSad");
            ar <=> articuno::kv(_MoodSurprise, "MoodSurprise");
            ar <=> articuno::kv(_MoodPuzzled, "MoodPuzzled");
            ar <=> articuno::kv(_MoodDisgust, "MoodDisgust");
        }

        articuno_deserialize(ar) {
            ar <=> articuno::kv(EmotionAnimationActiveThreshold, "EmotionAnimationActiveThreshold");
            std::string _DialogueAnger;
            std::string _DialogueFear;
            std::string _DialogueHappy;
            std::string _DialogueSad;
            std::string _DialogueSurprise;
            std::string _DialoguePuzzled;
            std::string _DialogueDisgust;
            std::string _MoodAnger;
            std::string _MoodFear;
            std::string _MoodHappy;
            std::string _MoodSad;
            std::string _MoodSurprise;
            std::string _MoodPuzzled;
            std::string _MoodDisgust;
            if (ar <=> articuno::kv(_DialogueAnger, "DialogueAnger")) {
                DialogueAnger = magic_enum::enum_cast<animation_type>(_DialogueAnger).value();
            }
            if (ar <=> articuno::kv(_DialogueFear, "DialogueFear")) {
                DialogueFear = magic_enum::enum_cast<animation_type>(_DialogueFear).value();
            }
            if (ar <=> articuno::kv(_DialogueHappy, "DialogueHappy")) {
                DialogueHappy = magic_enum::enum_cast<animation_type>(_DialogueHappy).value();
            }
            if (ar <=> articuno::kv(_DialogueSad, "DialogueSad")) {
                DialogueSad = magic_enum::enum_cast<animation_type>(_DialogueSad).value();
            }
            if (ar <=> articuno::kv(_DialogueSurprise, "DialogueSurprise")) {
                DialogueSurprise = magic_enum::enum_cast<animation_type>(_DialogueSurprise).value();
            }
            if (ar <=> articuno::kv(_DialoguePuzzled, "DialoguePuzzled")) {
                DialoguePuzzled = magic_enum::enum_cast<animation_type>(_DialoguePuzzled).value();
            }
            if (ar <=> articuno::kv(_DialogueDisgust, "DialogueDisgust")) {
                DialogueDisgust = magic_enum::enum_cast<animation_type>(_DialogueDisgust).value();
            }
            if (ar <=> articuno::kv(_MoodAnger, "MoodAnger")) {
                MoodAnger = magic_enum::enum_cast<animation_type>(_MoodAnger).value();
            }
            if (ar <=> articuno::kv(_MoodFear, "MoodFear")) {
                MoodFear = magic_enum::enum_cast<animation_type>(_MoodFear).value();
            }
            if (ar <=> articuno::kv(_MoodHappy, "MoodHappy")) {
                MoodHappy = magic_enum::enum_cast<animation_type>(_MoodHappy).value();
            }
            if (ar <=> articuno::kv(_MoodSad, "MoodSad")) {
                MoodSad = magic_enum::enum_cast<animation_type>(_MoodSad).value();
            }
            if (ar <=> articuno::kv(_MoodSurprise, "MoodSurprise")) {
                MoodSurprise = magic_enum::enum_cast<animation_type>(_MoodSurprise).value();
            }
            if (ar <=> articuno::kv(_MoodPuzzled, "MoodPuzzled")) {
                MoodPuzzled = magic_enum::enum_cast<animation_type>(_MoodPuzzled).value();
            }
            if (ar <=> articuno::kv(_MoodDisgust, "MoodDisgust")) {
                MoodDisgust = magic_enum::enum_cast<animation_type>(_MoodDisgust).value();
            }
        }

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

        friend class articuno::access;
    };
    
    class Animation {
    public:
        [[nodiscard]] inline const DirectControl& GetDirectControl() const noexcept {
            return _directcontrol;
        }

        [[nodiscard]] inline const RandomControl& GetRandomControl() const noexcept {
            return _randomcontrol;
        }

        [[nodiscard]] inline const EmotionControl& GetEmotionControl() const noexcept {
            return _emotioncontrol;
        }


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

    private:
        articuno_serde(ar) {
            ar <=> articuno::kv(_directcontrol, "directcontrol");
            ar <=> articuno::kv(_randomcontrol, "randomcontrol");
            ar <=> articuno::kv(_emotioncontrol, "emotioncontrol");

            ar <=> articuno::kv(ElinAnimation, "ElinAnimation");
            ar <=> articuno::kv(ElinEarsAnimation, "ElinEarsAnimation");
            ar <=> articuno::kv(ElinTailAnimation, "ElinTailAnimation");
            ar <=> articuno::kv(DisableOnRaceSexMenu, "DisableOnRaceSexMenu");
            ar <=> articuno::kv(EnablePlayer, "EnablePlayer");
            ar <=> articuno::kv(EnableNPCs, "EnableNPCs");
            ar <=> articuno::kv(AnimationEarsSpeed, "AnimationEarsSpeed");
            ar <=> articuno::kv(AnimationTailSpeed, "AnimationTailSpeed");
            ar <=> articuno::kv(Reversed, "Reversed");
        }

        bool ElinAnimation = true;
        bool ElinEarsAnimation = true;
        bool ElinTailAnimation = true;
        bool DisableOnRaceSexMenu = true;
        bool EnablePlayer = true;
        bool EnableNPCs = true;
        std::uint8_t AnimationEarsSpeed = 3;
        std::uint8_t AnimationTailSpeed = 3;
        bool Reversed = false;

        DirectControl _directcontrol;
        RandomControl _randomcontrol;
        EmotionControl _emotioncontrol;

        friend class articuno::access;
    };

    class Setting {
    public:
        [[nodiscard]] inline const Feature& GetFeature() const noexcept {
            return _feature;
        }

        [[nodiscard]] inline const Animation& GetAnimation() const noexcept {
            return _animation;
        }

    private:
        articuno_serde(ar) {
            ar <=> articuno::kv(_feature, "feature");
            ar <=> articuno::kv(_animation, "animation");
        }

        Feature _feature;
        Animation _animation;

        friend class articuno::access;
    };

    class Extra {
    public:
        [[nodiscard]] inline std::vector<RE::BSTArrayBase::size_type> GetLookupSliderIndexs() const noexcept {
            return LookupSliderIndexs;
        }
        [[nodiscard]] inline std::vector<std::string> GetLookupSliderNames() const noexcept {
            return LookupSliderNames;
        }

    private:
        articuno_serialize(ar) {
            std::string _LookupSliderIndexs;
            bool IndexFirst = true;
            for (auto& indexlist : LookupSliderNames)
            {
                if (!IndexFirst)
                    _LookupSliderIndexs += "|";
                _LookupSliderIndexs += std::to_string(indexlist);
                IndexFirst = false;
            }
            ar <=> articuno::kv(_LookupSliderIndexs, "LookupSliderNames");

            std::string _LookupSliderNames;
            bool NameFirst = true;
            for (auto& namelist : LookupSliderNames)
            {
                if (!NameFirst)
                    _LookupSliderNames += "|";
                _LookupSliderNames += namelist;
                NameFirst = false;
            }
            ar <=> articuno::kv(_LookupSliderNames, "LookupSliderNames");
        }

        articuno_deserialize(ar) {
            std::string _LookupSliderIndexs;
            std::vector<std::string> _LookupSliderIndexlist;
            if (ar <=> articuno::kv(_LookupSliderIndexs, "LookupSliderIndexs")) {
                _LookupSliderIndexlist = MultipleConfig::split(_LookupSliderIndexs, '|');
                for (auto& list : _LookupSliderIndexlist)
                {
                    RE::BSTArrayBase::size_type index = std::stoi(list);
                    LookupSliderIndexs.emplace_back(index);
                }
            }
            
            std::string _LookupSliderNames;
            if (ar <=> articuno::kv(_LookupSliderNames, "LookupSliderNames")) {
                LookupSliderNames = MultipleConfig::split(_LookupSliderNames, '|');
            }
        }

        std::vector<RE::BSTArrayBase::size_type> LookupSliderIndexs;
        std::vector<std::string> LookupSliderNames;

        friend class articuno::access;
    };

    class Config {
    public:
        [[nodiscard]] inline const Debug& GetDebug() const noexcept {
            return _debug;
        }

        [[nodiscard]] inline const Setting& GetSetting() const noexcept { 
            return _setting;
        }

        [[nodiscard]] inline const Extra& GetExtra() const noexcept {
            return _extra;
        }

        [[nodiscard]] static const Config& GetSingleton() noexcept;

    private:
        articuno_serde(ar) {
            ar <=> articuno::kv(_debug, "debug");
            ar <=> articuno::kv(_setting, "setting");
        }

        Debug _debug;
        Setting _setting;
        Extra _extra;

        friend class articuno::access;
    };

    class MultipleConfig {
    public:
        bool LoadElinAnimationConfig();

        // trim from start (in place)
        static inline void ltrim(std::string& s)
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                [](int c) {return !std::isspace(c); }));
        }

        // trim from end (in place)
        static inline void rtrim(std::string& s)
        {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                [](int c) {return !std::isspace(c); }).base(), s.end());
        }

        // trim from both ends (in place)
        static inline void trim(std::string& s)
        {
            ltrim(s);
            rtrim(s);
        }

        static inline std::string trim_copy(std::string s)
        {
            trim(s);
            return s;
        }

        static inline std::vector<std::string> split(const std::string& s, char delimiter)
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

        static inline void skipComments(std::string& str)
        {
            auto pos = str.find("#");
            if (pos != std::string::npos)
            {
                str.erase(pos);
            }
        }

        static inline RE::FormID getHex(std::string hexstr)
        {
            return (RE::FormID)strtoul(hexstr.c_str(), 0, 16);
        }


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

        static inline ControllerConfig GetConfigDefault()
        {
            ControllerConfig cc;
            cc.FrequencyMax = Config::GetSingleton().GetSetting().GetAnimation().GetRandomControl().GetFrequencyMax();
            cc.FrequencyMin = Config::GetSingleton().GetSetting().GetAnimation().GetRandomControl().GetFrequencyMin();
            cc.AnimationEarsSpeed = Config::GetSingleton().GetSetting().GetAnimation().GetAnimationEarsSpeed();
            cc.AnimationTailSpeed = Config::GetSingleton().GetSetting().GetAnimation().GetAnimationTailSpeed();
            cc.Reversed = Config::GetSingleton().GetSetting().GetAnimation().GetReversed();
            cc.EmotionEffectActiveThreshold = Config::GetSingleton().GetSetting().GetFeature().GetEmotionEffectActiveThreshold();
            cc.EmotionAnimationActiveThreshold = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetEmotionAnimationActiveThreshold();
            cc.DialogueAnger = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueAnger();
            cc.DialogueFear = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueFear();
            cc.DialogueHappy = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueHappy();
            cc.DialogueSad = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueSad();
            cc.DialogueSurprise = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueSurprise();
            cc.DialoguePuzzled = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialoguePuzzled();
            cc.DialogueDisgust = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetDialogueDisgust();
            cc.MoodAnger = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodAnger();
            cc.MoodFear = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodFear();
            cc.MoodHappy = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodHappy();
            cc.MoodSad = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodSad();
            cc.MoodSurprise = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodSurprise();
            cc.MoodPuzzled = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodPuzzled();
            cc.MoodDisgust = Config::GetSingleton().GetSetting().GetAnimation().GetEmotionControl().GetMoodDisgust();

            return cc;
        }

    private:
        int GetConfigSettingsValue(std::string line, std::string& variable);
        float GetConfigSettingsFloatValue(std::string line, std::string& variable);
        bool GetConfigSettingsBoolValue(std::string line, std::string& variable);
        std::string GetConfigSettingsStringValue(std::string line, std::string& variable);
        RE::FormID GetConfigSettingsFormIDValue(std::string line, std::string& variable);
        std::vector<RE::FormID> ConfigLineSplitterFormID(std::string& line);
    };
}
