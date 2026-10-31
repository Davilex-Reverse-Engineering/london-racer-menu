#include "VideoHandler.hpp"

#include "Utils.hpp"

#define AVI_HEADER_SIZE 56
#define STREAM_HEADER_SIZE 56
#define UNSHARED_BMP_HEADER_SIZE 14

VideoHandler::VideoHandler()
{
}

VideoHandler::~VideoHandler()
{
  if (this->stream) {
    SDL_CloseIO(this->stream);
  }
  if (this->shared_bmp_header) {
    free(this->shared_bmp_header);
  }
  for(SDL_Texture * frame : this->frames) {
    if (frame) {
      SDL_DestroyTexture(frame);
    }
  }
}

VideoHandler::AviHeader * VideoHandler::readAviHeader(uint32_t size) {
  if (size != AVI_HEADER_SIZE) {
    SDL_Log("AVI Header size %i is unsupported, expected %i", size, AVI_HEADER_SIZE);
    return nullptr;
  }

  AviHeader * header = (AviHeader *) calloc(1, sizeof(AviHeader));

  if(!SDL_ReadU32LE(this->stream, &header->microSecPerFrame)) {
    SDL_Log("Could not process avi header, microSecPerFrame could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->maxBytesPerSec)) {
    SDL_Log("Could not process avi header, maxBytesPerSec could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->paddingGranularity)) {
    SDL_Log("Could not process avi header, paddingGranularity could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->flags)) {
    SDL_Log("Could not process avi header, flags could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->totalFrames)) {
    SDL_Log("Could not process avi header, totalFrames could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->initialFrames)) {
    SDL_Log("Could not process avi header, initialFrames could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->streams)) {
    SDL_Log("Could not process avi header, streams could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->suggestedBufferSize)) {
    SDL_Log("Could not process avi header, suggestedBufferSize could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->width)) {
    SDL_Log("Could not process avi header, width could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->height)) {
    SDL_Log("Could not process avi header, height could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  // Here are some reserved bytes that are not used and should always be 0
  for(size_t i = 0; i < 4; i++) {
    uint32_t reserved_byte = 0;
    if(!SDL_ReadU32LE(this->stream, &reserved_byte) || reserved_byte != 0) {
      SDL_Log("Could not process avi header, reserved byte %lu (%u) could not be read: %s", i, reserved_byte, SDL_GetError());
      free(header);
      return nullptr;
    }
  }

  return header;
}

VideoHandler::StreamHeader * VideoHandler::readStreamHeader(uint32_t size) {
  if (size != STREAM_HEADER_SIZE) {
    SDL_Log("Stream Header size %i is unsupported, expected %i", size, STREAM_HEADER_SIZE);
    return nullptr;
  }

  StreamHeader * header = (StreamHeader *) calloc(1, sizeof(StreamHeader));

  if(!SDL_ReadIO(this->stream, (void *) &header->fccType, 4)) {
    SDL_Log("Could not process stream header, fccType could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadIO(this->stream, (void *) &header->fccHandler, 4)) {
    SDL_Log("Could not process stream header, fccHandler could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->flags)) {
    SDL_Log("Could not process stream header, flags could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU16LE(this->stream, &header->priority)) {
    SDL_Log("Could not process stream header, priority could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU16LE(this->stream, &header->language)) {
    SDL_Log("Could not process stream header, language could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->initialFrames)) {
    SDL_Log("Could not process stream header, initialFrames could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->scale)) {
    SDL_Log("Could not process stream header, scale could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->rate)) {
    SDL_Log("Could not process stream header, rate could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->start)) {
    SDL_Log("Could not process stream header, start could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->length)) {
    SDL_Log("Could not process stream header, length could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->suggestedBufferSize)) {
    SDL_Log("Could not process stream header, suggestedBufferSize could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->quality)) {
    SDL_Log("Could not process stream header, quality could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  if(!SDL_ReadU32LE(this->stream, &header->sampleSize)) {
    SDL_Log("Could not process stream header, sampleSize could not be read: %s", SDL_GetError());
    free(header);
    return nullptr;
  }

  for(size_t i = 0; i < 4; i++) {
    if(!SDL_ReadU16LE(this->stream, &header->rcFrame[i])) {
      SDL_Log("Could not process stream header, rcFrame %lu could not be read: %s", i, SDL_GetError());
      free(header);
      return nullptr;
    }
  }

  return header;
}

VideoHandler::ChunkHead VideoHandler::readChunkHead() {
  ChunkHead head;
  head.primary_type = ChunkPrimaryType::NONE;
  head.size = 0;
  head.secondary_type = ChunkSecondaryType::NONE;

  char pt[4];
  if(!SDL_ReadIO(this->stream, &pt, 4)) {
    SDL_Log("Could not read primary type string of chunk head");
    return head;
  }

  if (pt[0] == 'R' && pt[1] == 'I' && pt[2] == 'F' && pt[3] == 'F') {
    head.primary_type = ChunkPrimaryType::RIFF;
  } else if (pt[0] == 'L' && pt[1] == 'I' && pt[2] == 'S' && pt[3] == 'T') {
    head.primary_type = ChunkPrimaryType::LIST;
  } else if (pt[0] == 'J' && pt[1] == 'U' && pt[2] == 'N' && pt[3] == 'K') {
    head.primary_type = ChunkPrimaryType::JUNK;
  } else if (pt[0] == 'i' && pt[1] == 'd' && pt[2] == 'x' && pt[3] == '1') {
    head.primary_type = ChunkPrimaryType::idx1;
  } else if (pt[0] == 'a' && pt[1] == 'v' && pt[2] == 'i' && pt[3] == 'h') {
    head.primary_type = ChunkPrimaryType::avih;
  } else if (pt[0] == 's' && pt[1] == 't' && pt[2] == 'r' && pt[3] == 'h') {
    head.primary_type = ChunkPrimaryType::strh;
  } else if (pt[0] == 's' && pt[1] == 't' && pt[2] == 'r' && pt[3] == 'f') {
    head.primary_type = ChunkPrimaryType::strf;
  } else if (pt[0] == 's' && pt[1] == 't' && pt[2] == 'r' && pt[3] == 'n') {
    head.primary_type = ChunkPrimaryType::strn;
  } else if (pt[0] == 'i' && pt[1] == 'n' && pt[2] == 'd' && pt[3] == 'x') {
    head.primary_type = ChunkPrimaryType::indx;
  } else {
    char output[5] = {
      pt[0],
      pt[1],
      pt[2],
      pt[3],
      '\0'
    };
    SDL_Log("Unsupported chunk head type %s", output);
    return head;
  }

  if(!SDL_ReadU32LE(this->stream, &head.size) || head.size < 1) {
    SDL_Log("Could not read size of chunk head");
    return head;
  }

  if (head.primary_type == ChunkPrimaryType::RIFF) {
    char st[4];
     if(!SDL_ReadIO(this->stream, &st, 4)) {
      SDL_Log("Could not read secondary type string of chunk head type RIFF");
      return head;
    }
    if (st[0] == 'A' && st[1] == 'V' && st[2] == 'I') {
      if (st[3] == 'X') {
        head.secondary_type = ChunkSecondaryType::AVIX;
      } else if (st[3] == ' ') {
        head.secondary_type = ChunkSecondaryType::AVI;
      } else {
        char output[5] = {
          st[0],
          st[1],
          st[2],
          st[3],
          '\0'
        };
        SDL_Log("Unsupported secondary chunk head type for AVI: %s", output);
        return head;
      }
    }
  } else if (head.primary_type == ChunkPrimaryType::LIST) {
    char st[4];
     if(!SDL_ReadIO(this->stream, &st, 4)) {
      SDL_Log("Could not read secondary type string of chunk head type LIST");
      return head;
    }
    if (st[0] == 'h' && st[1] == 'd' && st[2] == 'r' && st[3] == 'l') {
        head.secondary_type = ChunkSecondaryType::hdrl;
    } else if (st[0] == 's' && st[1] == 't' && st[2] == 'r' && st[3] == 'l') {
        head.secondary_type = ChunkSecondaryType::strl;
    } else if (st[0] == 'm' && st[1] == 'o' && st[2] == 'v' && st[3] == 'i') {
        head.secondary_type = ChunkSecondaryType::movi;
    } else {
      char output[5] = {
        st[0],
        st[1],
        st[2],
        st[3],
        '\0'
      };
      SDL_Log("Unsupported secondary chunk head type for LIST: %s", output);
      return head;
    }
    head.size -= 4;
  }

  return head;
}

void VideoHandler::cleanup()
{
  if (this->stream) {
    SDL_CloseIO(this->stream);
    this->stream = NULL;
  }
  if (this->shared_bmp_header) {
    free(this->shared_bmp_header);
    this->shared_bmp_header = 0;
    this->shared_bmp_header_size = 0;
  }
  for(SDL_Texture * frame : this->frames) {
    if (frame) {
      SDL_DestroyTexture(frame);
    }
  }
  this->frames.clear();
  this->frame_time = 0;
  this->frame_offsets.clear();
}

bool VideoHandler::load(const std::string &file_path)
{
  this->cleanup();
  this->stream = SDL_IOFromFile(Utils::getFullPath(file_path).c_str(), "rb");
  if (this->stream == NULL) {
    SDL_Log("Could not load file Scores.dat as stream");
    return false;
  }

  ChunkHead riff_head = readChunkHead();
  if (riff_head.primary_type != ChunkPrimaryType::RIFF || riff_head.secondary_type != ChunkSecondaryType::AVI) {
    SDL_Log("Not a valid AVI file, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    return false;
  }

  ChunkHead list_hdrl_head = readChunkHead();
  if (list_hdrl_head.primary_type != ChunkPrimaryType::LIST || list_hdrl_head.secondary_type != ChunkSecondaryType::hdrl) {
    SDL_Log("Expected hdrl header in file, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    return false;
  }

  ChunkHead avih_head = readChunkHead();
  if (avih_head.primary_type != ChunkPrimaryType::avih) {
    SDL_Log("Expected avih head, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    return false;
  }

  AviHeader * avi_header = readAviHeader(avih_head.size);
  if(!avi_header) {
    SDL_Log("Could not read AVI header");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    return false;
  }
  if (avi_header->streams != 1) {
    SDL_Log("Only AVI files with a single stream are supported at the moment");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    return false;
  }
  this->frame_time = avi_header->microSecPerFrame / 1000;

  // Here there could be a different list I think
  ChunkHead list_strl_head = readChunkHead();
  if (list_strl_head.primary_type != ChunkPrimaryType::LIST || list_strl_head.secondary_type != ChunkSecondaryType::strl) {
    SDL_Log("Expected LIST strl here, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    return false;
  }

  ChunkHead strh_head = readChunkHead();
  if (strh_head.primary_type != ChunkPrimaryType::strh ) {
    SDL_Log("Expected strh here, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    return false;
  }

  StreamHeader * stream_header = readStreamHeader(strh_head.size);
  if(!stream_header) {
    SDL_Log("Could not read stream header");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    return false;
  }
  if (stream_header->fccType[0] != 'v' || stream_header->fccType[1] != 'i' || stream_header->fccType[2] != 'd' || stream_header->fccType[3] != 's') {
    SDL_Log("Expected vids type, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    free(stream_header);
    return false;
  }
  if (stream_header->fccHandler[0] != 'D' || stream_header->fccHandler[1] != 'I' || stream_header->fccHandler[2] != 'B' || stream_header->fccHandler[3] != ' ') {
    SDL_Log("Only DIB handler type is supported, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    free(stream_header);
    return false;
  }

  ChunkHead strf_head = readChunkHead();
  if (strf_head.primary_type != ChunkPrimaryType::strf) {
    SDL_Log("Expected strf here, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    free(stream_header);
    return false;
  }

  this->shared_bmp_header_size = strf_head.size;
  this->shared_bmp_header = malloc(this->shared_bmp_header_size);
  if(!SDL_ReadIO(this->stream, this->shared_bmp_header, this->shared_bmp_header_size)) {
    SDL_Log("Could not read bitmap header from strf, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    free(stream_header);
    return false;
  }

  ChunkHead next_head = readChunkHead();
  while (next_head.primary_type != ChunkPrimaryType::LIST && next_head.secondary_type != ChunkSecondaryType::movi) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Found chunk head with types %i,%i", next_head.primary_type, next_head.secondary_type);
    if (next_head.size < 1) {
      SDL_Log("Failed to read size of chunk, failing");
      break;
    }
    if (SDL_SeekIO(this->stream, (int64_t) next_head.size, SDL_IO_SEEK_CUR) < 0) {
      SDL_Log("Failed to move reading head to next chunk head, failing");
      break;
    }
    next_head = readChunkHead();
  }
  if (next_head.secondary_type != ChunkSecondaryType::movi) {
    SDL_Log("Could not find movi head, failing");
    SDL_CloseIO(this->stream);
    this->stream = NULL;
    free(avi_header);
    free(stream_header);
    free(this->shared_bmp_header);
    this->shared_bmp_header = NULL;
    this->shared_bmp_header_size = 0;
    return false;
  }

  for(uint32_t i = 0; i < stream_header->length; i++) {
    char movi_type[4];
    SDL_ReadIO(this->stream, movi_type, 4);
    FrameOffset offset;
    SDL_ReadU32LE(this->stream, &offset.size);
    if (movi_type[0] != '0' || movi_type[1] != '0' || movi_type[2] != 'd' || movi_type[3] != 'b') {
      SDL_SeekIO(this->stream, (int64_t) offset.size, SDL_IO_SEEK_CUR);
      continue;
    } else {
      offset.offset = SDL_TellIO(this->stream);
      this->frame_offsets.push_back(offset);
      this->frames.push_back(NULL);
      SDL_SeekIO(this->stream, (int64_t) offset.size, SDL_IO_SEEK_CUR);
    }
  }

  free(avi_header);
  free(stream_header);
  return true;
}

SDL_Texture * VideoHandler::getTexture(SDL_Renderer *renderer)
{
  if (this->frame_offsets.empty()) {
    SDL_Log("Loading texture failed, because no frame offsets were loaded");
    return NULL;
  }
  if (this->last_frame_switch != 0 && this->last_frame_switch + this->frame_time <= SDL_GetTicks()) {
    this->last_frame_switch = SDL_GetTicks();
    this->frame++;
  }
  // Make sure the frame is valid
  if (this->frame >= this->frame_offsets.size()) {
    this->frame = 0;
  }
  if (this->frames[this->frame] != NULL) {
    return this->frames[this->frame];
  }

  uint32_t data_offset = UNSHARED_BMP_HEADER_SIZE + this->shared_bmp_header_size;
  uint32_t total_size = data_offset + this->frame_offsets[this->frame].size;

  void * frame_data = calloc(1, total_size);
  if (!frame_data) {
    SDL_Log("Could not free memory for loading frame from AVI file");
    return NULL;
  }
  // Load data first
  SDL_SeekIO(this->stream, this->frame_offsets[this->frame].offset, SDL_IO_SEEK_SET);
  SDL_ReadIO(this->stream, ((uint8_t *) frame_data) + data_offset, this->frame_offsets[this->frame].size);

  // SDL_IOStream * output_stream =  SDL_IOFromFile("output.bmp", "wb");
  SDL_IOStream * output_stream = SDL_IOFromMem(frame_data, total_size);
  if (!output_stream) {
    SDL_Log("Could not create output stream for loading frame from AVI file");
    return NULL;
  }

  // Add BMP head
  SDL_WriteU8(output_stream, 0x42); // B
  SDL_WriteU8(output_stream, 0x4d); // M

  // Set size in the header
  SDL_WriteU32LE(output_stream, total_size);

  // Set the 4 reserved bytes to 0
  SDL_WriteU32LE(output_stream, 0);

  // Set the data offset in the header
  SDL_WriteU32LE(output_stream, data_offset);

  // Add shared header
  SDL_WriteIO(output_stream, this->shared_bmp_header, this->shared_bmp_header_size);


  SDL_SeekIO(output_stream, 0, SDL_IO_SEEK_SET);
  SDL_Surface * surface = SDL_LoadBMP_IO(output_stream, true);
  if (!surface) {
    SDL_Log("Could not load BMP as surface");
    free(frame_data);
    return NULL;
  }
  this->frames[this->frame] = SDL_CreateTextureFromSurface(renderer, surface);
  if (!this->frames[this->frame]) {
    SDL_Log("Failed to create frame texture from surface");
  }
  SDL_DestroySurface(surface);
  free(frame_data);

  this->last_frame_switch = SDL_GetTicks();
  return this->frames[this->frame];
}
