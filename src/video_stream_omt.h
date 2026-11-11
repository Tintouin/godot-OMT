#pragma once
#include <godot_cpp/classes/video_stream.hpp>
#include <godot_cpp/classes/video_stream_playback.hpp>

using namespace godot;

class VideoStreamOMT : public VideoStream {
    GDCLASS(VideoStreamOMT, VideoStream)

    String source;

protected:
    static void _bind_methods();

public:
    Ref<VideoStreamPlayback> _instantiate_playback() override;
    void set_source(const String &p_source);
    String get_source() const;

};