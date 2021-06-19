extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

#include <stdio.h>

#define CODEC AV_CODEC_ID_MPEG2VIDEO
#define BIT_RATE 4000000

class Decoder
{
private:
	const AVCodec* m_codec;
	AVCodecContext* m_avcodec_context;
	SwsContext* m_img_convert_ctx;
	AVCodecParserContext* m_parser_ctx;

public:
    Decoder(/* args */);
    ~Decoder();
	void setupContexts(int width, int height);
	void decode(char* buffer, AVFrame* frame);
	int decode_packet(AVPacket* pPacket, AVFrame* pFrame);
};

