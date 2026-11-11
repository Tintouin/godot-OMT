#include "video_stream_omt.h"
#include "video_stream_playback_omt.h"

void VideoStreamOMT::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_source", "source"), &VideoStreamOMT::set_source);
    ClassDB::bind_method(D_METHOD("get_source"), &VideoStreamOMT::get_source);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "source"), "set_source", "get_source");
}

Ref<VideoStreamPlayback> VideoStreamOMT::_instantiate_playback() {
    Ref<VideoStreamPlaybackOMT> playback;
    playback.instantiate();
    playback->set_source(source);
    playback->init_texture();
    return playback;
}

void VideoStreamOMT::set_source(const String &p_source) {
    source = p_source;
}

String VideoStreamOMT::get_source() const {
    return source;
}