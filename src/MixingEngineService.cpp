#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr; 
    std::cout << "[MixingEngineService] Initialized with 2 empty decks" << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (int i = 0; i < 2; i++) {
        delete decks[i];
        decks[i] = nullptr;
    }
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;

    // Clone the track and wrap in a pointer wrapper for safety
    PointerWrapper<AudioTrack> cloned  =  track.clone();
    if (!cloned) {
        std::cout << "[ERROR] Track: " << "\"" << track.get_title() << "\"" << " failed to clone" << std::endl;
        return -1;
    }

    size_t target_deck;
    bool first_track = false;

    // Handle first track assignment
    if (!(decks[0]) && !(decks[1])) {
        target_deck = 0;
        first_track = true;
    } 

    // Handle subsequent tracks assignment
    else {
        target_deck = 1 - active_deck;
        std::cout << "[Deck Switch] Target Deck: " << target_deck << std::endl;
        if (decks[target_deck]) {
            delete decks[target_deck];
            decks[target_deck] = nullptr;
        }

        cloned -> load();
        cloned -> analyze_beatgrid();

        if (decks[active_deck] && auto_sync && !can_mix_tracks(cloned)) {
            sync_bpm(cloned);
        }
    }

    decks[target_deck] = cloned.release();
    std::cout << "[Load Complete] " << track.get_title() << " is now loaded on deck" << target_deck << std::endl;
    
    // Unloading active deck
    if(!first_track && decks[active_deck]) {
        std::cout << "[Unload] unloading previous deck " << active_deck << " (" << decks[active_deck] -> get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }

    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;

    return target_deck; // Placeholder
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (!decks[active_deck] || !track.get()) {
        return false; 
    }

    return std::abs(decks[active_deck] -> get_bpm() - track -> get_bpm()) <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] && track.get()) {
        int old_bpm = track -> get_bpm();
        int average_bpm = static_cast<int>((old_bpm + decks[active_deck] -> get_bpm()) / 2);
        track -> set_bpm(average_bpm);

        std::cout << "[Sync BPM] Syncing BPM from " << old_bpm << "to " << average_bpm << std::endl;
    }
}
