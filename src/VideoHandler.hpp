#ifndef VIDEO_HANDLER_HPP
#define VIDEO_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <vector>
#include <string>

class VideoHandler {
public:
  VideoHandler();
  ~VideoHandler();

  bool load(const std::string &file_path);
  SDL_Texture * getTexture(SDL_Renderer * renderer);

private:
  typedef struct {
    uint32_t microSecPerFrame;
    uint32_t maxBytesPerSec;
    uint32_t paddingGranularity;
    uint32_t flags;
    uint32_t totalFrames;
    uint32_t initialFrames;
    uint32_t streams;
    uint32_t suggestedBufferSize;
    uint32_t width;
    uint32_t height;
  } AviHeader;

  typedef struct {
    char fccType[4];
    char fccHandler[4];
    uint32_t flags;
    uint16_t priority;
    uint16_t language;
    uint32_t initialFrames;
    uint32_t scale;
    uint32_t rate;
    uint32_t start;
    uint32_t length;
    uint32_t suggestedBufferSize;
    uint32_t quality;
    uint32_t sampleSize;
    uint16_t rcFrame[4];
  } StreamHeader;

  enum class ChunkPrimaryType {
    NONE,
    RIFF,
    LIST,
    JUNK,
    idx1,
    avih,
    strh,
    strf,
    strn,
    indx,
  };

  enum class ChunkSecondaryType {
    NONE,
    AVI,
    AVIX,
    hdrl,
    strl,
    movi,
  };

  typedef struct {
    ChunkPrimaryType primary_type;
    uint32_t size;
    ChunkSecondaryType secondary_type;
  } ChunkHead;

  typedef struct {
    uint32_t size;
    int64_t offset;
  } FrameOffset;

  uint32_t frame = 0;
  uint64_t last_frame_switch = 0;
  uint32_t frame_time = 0;
  SDL_IOStream * stream = NULL;
  std::vector<SDL_Texture *> frames;
  void * shared_bmp_header = NULL;
  uint32_t shared_bmp_header_size = 0;

  void cleanup();

  std::vector<FrameOffset> frame_offsets;
  AviHeader * readAviHeader(uint32_t size);
  StreamHeader * readStreamHeader(uint32_t size);
  ChunkHead readChunkHead();
};

#endif // VIDEO_HANDLER_HPP