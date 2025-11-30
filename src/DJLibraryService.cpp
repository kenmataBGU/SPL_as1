#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for (const SessionConfig::TrackInfo& track_info : library_tracks) {
        if (track_info.type == "MP3") {
            AudioTrack* new_track = new MP3Track(track_info.title, track_info.artists, track_info.duration_seconds, track_info.bpm, track_info.extra_param1, track_info.extra_param2);
            library.push_back(new_track);
            std::cout << "MP3Track created: " << track_info.extra_param1 << "kbps" << std::endl;

        }

        if (track_info.type == "WAV") {
            AudioTrack* new_track = new WAVTrack(track_info.title, track_info.artists, track_info.duration_seconds, track_info.bpm, track_info.extra_param1, track_info.extra_param2);
            library.push_back(new_track);
            std::cout << "WAVTrack created: " << track_info.extra_param1 << "Hz/" << track_info.extra_param2 << " bit" << std::endl;
        }
    }

    std::cout << "[INFO] Track library built:" << library_tracks.size() << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
 
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    
    // Creating new playist...
    std::cout << "[INFO] Loading playlist:" << playlist_name << std::endl;
    playlist = Playlist(playlist_name);  

    for (int index : track_indices) {
        index--; // Converting index to 0-based.

        // Skipping invalid indexes
        if (index < 0 || index >= static_cast<int>(library.size())) {
            std::cout << "[Warning]: Invalid track index: " << index + 1 << std::endl;
            continue;
        }

        // Cloning track
        AudioTrack* lib_track = library[index]; 
        AudioTrack* cloned_track = (lib_track -> clone()).release();


        // Skipping index if clone failed
        if (!cloned_track) {
            std::cout << "[Warning]: failed to clone track";
            continue;
        }

        cloned_track -> load();
        cloned_track -> analyze_beatgrid();
        playlist.add_track(cloned_track);
        std::cout << "Added " << cloned_track -> get_title() << "to playlist " << playlist_name << std::endl;
    }     
    std::cout << "[INFO] playlist loaded: " << playlist_name << "(" << playlist.get_track_count() << " tracks)";                                       
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    for (AudioTrack* track : playlist.getTracks()) {
        titles.push_back(track -> get_title());
    }

    return titles;
}
