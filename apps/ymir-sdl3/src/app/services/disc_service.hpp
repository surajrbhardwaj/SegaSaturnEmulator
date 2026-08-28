#pragma once

#include <app/settings.hpp>
#include <app/shared_context.hpp>

#include <ymir/media/disc.hpp>

#include <filesystem>
#include <functional>
#include <string>
#include <thread>
#include <optional>

namespace app::services {

/// @brief Handles loading disc images and the recent games list.
class DiscService {
public:
    enum class DiscLoadStatus : uint8 {
        DEFAULT = 0,
        SUCCESS = 1,
        FAIL = 2
    };
    
    std::atomic<DiscLoadStatus> m_asyncLoadStatus;
    std::thread asyncDiscLoadThread;
    std::optional<ymir::media::Disc> asyncDisc;
    std::filesystem::path asyncPath;

    using ShowModalCallback = std::function<void(std::string title, std::function<void()> contents)>;

    DiscService(SharedContext &context, Settings &settings, ShowModalCallback showModal);
    ~DiscService() {
        if(asyncDiscLoadThread.joinable()) {
            asyncDiscLoadThread.join();
        }
    }

    DiscService(const DiscService &) = delete;
    DiscService &operator=(const DiscService &) = delete;

    /// @brief Opens the dialog to select a Saturn disc image.
    void OpenLoadDiscDialog();

    /// @brief Callback for the disc image file dialog selection.
    /// @param[in] filelist List of selected files.
    /// @param[in] filter Selected file dialog filter index.
    void ProcessOpenDiscImageFileDialogSelection(const char *const *filelist, int filter);

    /// @brief Loads a disc image file and updates the recent list.
    /// @param[in] path Path to the disc image.
    /// @param[in] showErrorModal Whether to show an error dialog if loading fails.
    /// @return True if successful.
    bool LoadDiscImage(std::filesystem::path path, bool showErrorModal);
    
    // TODO: add some documenation here
    void LoadDiscImageAsync(std::filesystem::path path, bool showErrorModal);

    /// @brief Loads the list of recent discs from disk.
    void LoadRecentDiscs();

    /// @brief Saves the list of recent discs to disk.
    void SaveRecentDiscs();

    void UpdateSettingsAndContext(ymir::media::Disc disc, std::filesystem::path path);
private:
    SharedContext &m_context;
    Settings &m_settings;
    ShowModalCallback m_showModal;
};

} // namespace app::services
