#include "Decoder.h"

Decoder::Decoder()
{
}

Decoder::~Decoder()
{
}

void Decoder::decode(char* buffer, int length, std::function<void(AVFrame* frame)> callback) {
	auto pkt = av_packet_alloc();

	if (!pkt)
	{
		fprintf(stderr, "Cannot alloc packet\n");
		exit(1);
	}

	uint8_t* data = (uint8_t*) buffer;

	while (length > 0) {
		int ret = av_parser_parse2(m_parser_ctx, m_avcodec_context, &pkt->data, &pkt->size,
			data, length, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
		if (ret < 0) {
			fprintf(stderr, "Error during parse\n");
			exit(1);
		}

		data += ret;
		length -= ret;

		if (pkt->size) {
			decode_packet(pkt, callback);
		}
	}
}

void Decoder::setupContexts(int width, int height) {
	m_codec = avcodec_find_decoder(CODEC);

	m_avcodec_context = avcodec_alloc_context3(m_codec);

	m_avcodec_context->bit_rate = BIT_RATE;

	m_avcodec_context->width = width;
	m_avcodec_context->height = height;

	// frames per second
	m_avcodec_context->time_base.num = 1;
	m_avcodec_context->time_base.den = 25;
	m_avcodec_context->framerate.num = 25;
	m_avcodec_context->framerate.den = 1;

	m_avcodec_context->gop_size = 10; // emit one intra frame every ten frames
	m_avcodec_context->max_b_frames = 1;
	m_avcodec_context->pix_fmt = AV_PIX_FMT_YUV420P;
	if (avcodec_open2(m_avcodec_context, m_codec, NULL) < 0)
	{
		fprintf(stderr, "could not open codec\n");
		exit(1);
	}

	m_img_convert_ctx = sws_getContext(m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_RGBA, m_avcodec_context->width, m_avcodec_context->height, AV_PIX_FMT_YUV420P, 0, NULL, NULL, NULL);
	if (!m_img_convert_ctx)
	{
		fprintf(stderr, "error creating swsContext");
		exit(1);
	}

	m_parser_ctx = av_parser_init(m_codec->id);
	m_parser_ctx->flags = PARSER_FLAG_COMPLETE_FRAMES;
	if (!m_parser_ctx) {
		fprintf(stderr, "error initializing parser\n");
		exit(1);
	}
}

int Decoder::decode_packet(AVPacket* pPacket, std::function<void(AVFrame* frame)> callback)
{
	auto frame = av_frame_alloc();
	if (!frame)
	{
		fprintf(stderr, "Cannot alloc frame\n");
		exit(1);
	}
	// Supply raw packet data as input to a decoder
	// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3
	int response = avcodec_send_packet(m_avcodec_context, pPacket);

	if (response < 0) {
		//logging("Error while sending a packet to the decoder: %s", av_err2str(response));
		return response;
	}

	while (response >= 0)
	{
		// Return decoded output data (into a frame) from a decoder
		// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
		response = avcodec_receive_frame(m_avcodec_context, frame);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) 
		{
			break;
		}

		else if (response < 0) 
		{
			printf("Error while receiving a frame from the decoder: %s", response);
			return response;
		}

		if (response >= 0) 
		{
			printf(
				"Frame %d (type=%c, size=%d bytes, format=%d) pts %d key_frame %d [DTS %d]",
				m_avcodec_context->frame_number,
				av_get_picture_type_char(frame->pict_type),
				frame->pkt_size,
				frame->format,
				frame->pts,
				frame->key_frame,
				frame->coded_picture_number
			);

			if (frame->format != AV_PIX_FMT_YUV420P)
			{
				printf("Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
			}

			callback(frame);
		}
	}
	return 0;
}