#pragma once
#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include "libomt.h"
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/video_stream_playback.hpp>

using namespace godot;

class VideoStreamPlaybackOMT : public VideoStreamPlayback {
    GDCLASS(VideoStreamPlaybackOMT, VideoStreamPlayback)

    String source;
    Ref<ImageTexture> texture;
    bool playing = false;
    bool paused = false;
    double position = 0.0;

    omt_receive_t *receiver = nullptr;
    std::thread worker;
    std::atomic<bool> running{false};
    std::mutex frame_mutex;
    std::vector<uint8_t> pixels;
    int frame_w = 0;
    int frame_h = 0;
    int frame_stride = 0;
    bool frame_alpha = false;
    bool frame_ready = false;

    void start_receiver();
    void stop_receiver();
    void receive_loop();

protected:
    static void _bind_methods() {}

public:
    void set_source(const String &p_source) { source = p_source; }

    void _stop() override;
    void _play() override;
    bool _is_playing() const override;
    void _set_paused(bool p_paused) override;
    bool _is_paused() const override;
    double _get_length() const override;
    double _get_playback_position() const override;
    void _seek(double p_time) override;
    void _set_audio_track(int32_t p_idx) override;
    Ref<Texture2D> _get_texture() const override;
    void _update(double p_delta) override;
    int32_t _get_channels() const override;
    int32_t _get_mix_rate() const override;
    void init_texture();
    ~VideoStreamPlaybackOMT();
    
};