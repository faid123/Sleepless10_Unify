/******************************************************************************
* \file			video_reader.h
* \brief		Contain all the Component related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
                Reproduction or disclosure of this file or its contents without the
                prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

extern "C"
{
#include <inttypes.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>

/// <summary>
/// Component needed for video reader
/// </summary>
struct VideoReaderState {
    // Public things for other parts of the program to read from
    int width, height;
    AVRational time_base;

    // Private internal state
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    int video_stream_index;
    AVFrame* av_frame;
    AVPacket* av_packet;
    SwsContext* sws_scaler_ctx;
};
/// <summary>
/// opening video files
/// </summary>
/// <param name="state">state of the video</param>
/// <param name="filename">name of the file</param>
/// <returns>true when it is successful</returns>
bool video_reader_open(VideoReaderState* state, const char* filename);\
/// <summary>
/// reading from the video setting frame_buffer 
/// </summary>
/// <param name="state">starting state </param>
/// <param name="frame_buffer"></param>
/// <param name="pts"></param>
/// <returns></returns>
bool video_reader_read_frame(VideoReaderState* state, uint8_t* frame_buffer, int64_t* pts);
bool video_reader_seek_frame(VideoReaderState* state, int64_t ts);
void video_reader_close(VideoReaderState* state);
void video_reader_close_now(VideoReaderState* state);
const char* av_make_error(int errnum);
AVPixelFormat correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt);

}
    