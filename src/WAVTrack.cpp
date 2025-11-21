#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    long size = (long) duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout << "  → Estimated file size: " << size << "bytes" << std::endl;
    std::cout << "  → Fast loading due to uncompressed format.";

}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"

    double beats = (duration_seconds / 60.0) * bpm; 
    std::cout << "  → Estimated beats: " << beats << "  → Precision factor: 1 (uncompressed audio)";
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    const double CD_quality_sample_rate = 44100;
    const double CD_quality_bit_depth = 16;
    const double high_res_quality_sample_rate = 96000;
    const double professional_bit_depth = 24;

    double base_score = 70;
    if (sample_rate >= CD_quality_sample_rate) {
        base_score += 10;
        if (sample_rate >= high_res_quality_sample_rate) {
            base_score += 5;
        }
    }

    if (bit_depth >= CD_quality_bit_depth) {
        base_score += 10;
        if (bit_depth >= professional_bit_depth) {
            base_score += 5; 
        }
    }

    double final_score = (base_score > 100) ? 100 : base_score;
    return final_score; 
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method

    WAVTrack *cloned = new WAVTrack(*this);
    return PointerWrapper<AudioTrack>(cloned); 
}