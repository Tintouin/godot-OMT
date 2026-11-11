#include "video_stream_playback_omt.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/image.hpp>
#include <cmath>

bool VideoStreamPlaybackOMT::_is_playing() const { return playing; }
void VideoStreamPlaybackOMT::_set_paused(bool p_paused) { paused = p_paused; }
bool VideoStreamPlaybackOMT::_is_paused() const { return paused; }
double VideoStreamPlaybackOMT::_get_length() const { return 0.0; }
double VideoStreamPlaybackOMT::_get_playback_position() const { return position; }
void VideoStreamPlaybackOMT::_seek(double p_time) {}
void VideoStreamPlaybackOMT::_set_audio_track(int32_t p_idx) {}
Ref<Texture2D> VideoStreamPlaybackOMT::_get_texture() const {return texture;}
int32_t VideoStreamPlaybackOMT::_get_channels() const { return 2; }
int32_t VideoStreamPlaybackOMT::_get_mix_rate() const { return 48000; }

void VideoStreamPlaybackOMT::init_texture() {
    Ref<Image> img = Image::create_empty(256, 144, false, Image::FORMAT_RGBA8);
    img->fill(Color(0.0, 0.0, 0.0, 1.0));
    texture = ImageTexture::create_from_image(img);
}

VideoStreamPlaybackOMT::~VideoStreamPlaybackOMT() {
    stop_receiver();
}

void VideoStreamPlaybackOMT::start_receiver() {
    stop_receiver();
    if (source.is_empty()) {
        return;
    }
    receiver = omt_receive_create(source.utf8().get_data(), OMTFrameType_Video,
            OMTPreferredVideoFormat_BGRA, OMTReceiveFlags_None);
    if (!receiver) {
        UtilityFunctions::push_warning("OMT: unable to create receiver");
        return;
    }
    running = true;
    worker = std::thread(&VideoStreamPlaybackOMT::receive_loop, this);
}

void VideoStreamPlaybackOMT::stop_receiver() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
    if (receiver) {
        omt_receive_destroy(receiver);
        receiver = nullptr;
    }
}

void VideoStreamPlaybackOMT::receive_loop() {
    while (running) {
        OMTMediaFrame *f = omt_receive(receiver, OMTFrameType_Video, 100);
        if (!f || f->Type != OMTFrameType_Video || f->Codec != OMTCodec_BGRA || !f->Data || f->DataLength <= 0) {
            continue;
        }
        std::lock_guard<std::mutex> lock(frame_mutex);
        const uint8_t *src = static_cast<const uint8_t *>(f->Data);
        pixels.assign(src, src + f->DataLength);
        frame_w = f->Width;
        frame_h = f->Height;
        frame_stride = f->Stride;
        frame_alpha = (f->Flags & OMTVideoFlags_Alpha) != 0;
        frame_ready = true;
    }
}

void VideoStreamPlaybackOMT::_play() {
    playing = true;
    paused = false;
    position = 0.0;
    start_receiver();
}

void VideoStreamPlaybackOMT::_stop() {
    playing = false;
    position = 0.0;
    stop_receiver();
}

void VideoStreamPlaybackOMT::_update(double p_delta) {
    if (!playing || paused) {
        return;
    }
    position += p_delta;

    std::vector<uint8_t> local;
    int w, h, stride;
    bool alpha;
    {
        std::lock_guard<std::mutex> lock(frame_mutex);
        if (!frame_ready) {
            return;
        }
        local.swap(pixels);
        w = frame_w;
        h = frame_h;
        stride = frame_stride;
        alpha = frame_alpha;
        frame_ready = false;
    }

    PackedByteArray data;
    data.resize((int64_t)w * h * 4);
    uint8_t *dst = data.ptrw();
    for (int y = 0; y < h; y++) {
        const uint8_t *src = local.data() + (size_t)y * stride;
        for (int x = 0; x < w; x++) {
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = alpha ? src[3] : 255;
            src += 4;
            dst += 4;
        }
    }

    Ref<Image> img = Image::create_from_data(w, h, false, Image::FORMAT_RGBA8, data);
    texture->set_image(img);
}