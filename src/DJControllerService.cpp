#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {

    // Checks if the track is already on cache
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }

    // Clones the track and performed loading and beatgrid analysis
    AudioTrack *cloned = track.clone().release();
    if (!cloned) {
        std::cout << "[ERROR] Track: \"" << track.get_title() <<  "\"" << " failed to clone" << std::endl;
        return -2;
    }

    cloned -> load();
    cloned -> analyze_beatgrid();

    // Puts the track in cache
    PointerWrapper<AudioTrack> cloned_wrapped = PointerWrapper<AudioTrack>(cloned);
    bool eviction_occured = cache.put(std::move(cloned_wrapped));

    return (eviction_occured) ? -1 : 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack *track = cache.get(track_title);
    return (track) ? track : nullptr;
}
