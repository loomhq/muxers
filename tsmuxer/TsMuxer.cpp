#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#pragma warning(disable : 4099 4244 4267 4018)
#endif

#include "include/TsMuxer.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "H264Utils.h"
#include "TsMuxerUtils.h"
typedef unsigned char u_char;

#include <algorithm>
#include <array>
#include <map>
#include <vector>

// from ffmpeg
static const uint32_t crc_table[256] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005, 0x2608edb8,
    0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd, 0x4c11db70, 0x48d0c6c7,
    0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75, 0x6a1936c8, 0x6ed82b7f, 0x639b0da6,
    0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd, 0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5, 0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84,
    0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d, 0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb,
    0xceb42022, 0xca753d95, 0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a,
    0xec7dd02d, 0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca, 0x7897ab07,
    0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02, 0x5e9f46bf, 0x5a5e5b08,
    0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba, 0xaca5c697, 0xa864db20, 0xa527fdf9,
    0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692, 0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a, 0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b,
    0xf771768c, 0xfa325055, 0xfef34de2, 0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34,
    0xdc3abded, 0xd8fba05a, 0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c,
    0x774bb0eb, 0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b, 0x0315d626,
    0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623, 0xf12f560e, 0xf5ee4bb9,
    0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b, 0xd727bbb6, 0xd3e6a601, 0xdea580d8,
    0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3, 0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b, 0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a,
    0x8cf30bad, 0x81b02d74, 0x857130c3, 0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2,
    0x470cdd2b, 0x43cdc09c, 0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093,
    0x65c52d24, 0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654, 0xc5a92679,
    0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c, 0xe3a1cbc1, 0xe760d676,
    0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4, 0x89b8fd09, 0x8d79e0be, 0x803ac667,
    0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c, 0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4};

static uint32_t mpegts_crc32(const u_char* data, size_t len) {
  const u_char* end = data + len;
  uint32_t crc = 0xffffffff;

  for (; data < end; data++) {
    crc = (crc << 8) ^ crc_table[((crc >> 24) ^ *data) & 0xff];
  }

  return crc;
}

enum class Codec { AAC, H264 };

struct OutputStream;

const unsigned kMaxBufferLength = 2 * 4 * 3840 * 2160;  // twice the size of a raw ARGB 4K frame

// Struct to collect all the data for the frame we want to encode
struct FrameToEncode {
  const u_char* data = nullptr;
  int len = 0;
  int pts = 0;
  Codec codec;
};

// TsMuxerClass's lifecycle begins and ends with each recording.
class TsMuxerClass {
 public:
  TsMuxerClass(uint32_t flags);
  ~TsMuxerClass() = default;

  u_char* muxAac(const u_char* data, int data_len, unsigned long pts_in_90khz, int* muxed_len);

  u_char* muxH264(const u_char* data, int data_len, unsigned long pts_in_90khz, int* muxed_len);

 private:
  void buildPmt();

  void writePat();
  void writePmt();
  void writePesPacket(const FrameToEncode&);

  bool shouldWritePsi(const uint8_t* data, uint32_t length, uint64_t pts_in_90khz, Codec codec) const;
  void writePsiIfNecessary(const uint8_t* data, uint32_t length, uint64_t pts_in_90khz, Codec codec);

  void writeToBuffer(const u_char* data, size_t len);
  void writeToBuffer(const std::vector<u_char>& data);
  void writeStuffingBytes();
  bool packetHasPcr();
  OutputStream* getCurrentStream() const;
  int adaptationFieldLength();
  void write_adaptation_field_section();
  void writeH264AudIfAbsent();

  /**
   -Returns: the number of bytes of frame written
   */
  int writeTsPes();
  void writePesHeader(int adapfield_size);

  /**
   -Returns: the number of bytes of frame written
   */
  int writePesPayload();

 private:
  uint32_t config_flags_;

  std::vector<u_char> buffer_;  ///< Internal buffer to write to
  std::vector<u_char> pmt_;
  uint32_t bytes_written_ = 0;

  std::map<Codec, OutputStream> streams_;
  OutputStream* current_stream_ = nullptr;

  uint8_t pat_cc_ = 0;
  uint8_t pmt_cc_ = 0;

  uint64_t latest_90khz_pts_psi_written_ = 0;  // Should use optional,
  // not supported in old mac compilers. Using "0" as "not set"
  const uint64_t max_psi_period_in_90khz_ = 1 * 90000;  // 0 for every packet
};

/*
 Public interface implementation BEGIN
 */
extern "C" {

// Implementation of the type we expose publicly
struct TsMuxer {
  void* obj;
};

const unsigned char* muxAac(struct TsMuxer* muxer, const unsigned char* data, int length, unsigned long pts_in_90khz,
                            int* muxed_len) {
  return (static_cast<TsMuxerClass*>(muxer->obj))->muxAac(data, length, pts_in_90khz, muxed_len);
}
const unsigned char* muxH264(struct TsMuxer* muxer, const unsigned char* data, int length, unsigned long pts_in_90khz,
                             int* muxed_len) {
  return (static_cast<TsMuxerClass*>(muxer->obj))->muxH264(data, length, pts_in_90khz, muxed_len);
}

struct TsMuxer* createTsMuxer(uint32_t flags) {
  struct TsMuxer* ts_muxer = (struct TsMuxer*)malloc(sizeof(struct TsMuxer));
  TsMuxerClass* obj = new TsMuxerClass(flags);
  ts_muxer->obj = obj;
  return ts_muxer;
}

void destroyTsMuxer(struct TsMuxer* ts_muxer) {
  if (!ts_muxer) {
    return;
  }

  delete static_cast<TsMuxerClass*>(ts_muxer->obj);
  free(ts_muxer);
}
}
/*
Public interface implementation END
*/

// From FFMPEG
#define AUDIO_FRAME_CLOCK 1920  // 90000 / 46.875
#define VIDEO_FRAME_CLOCK 3000
#define INITIAL_PCR 63000
#define DEFAULT_PES_ADTS_STREAM_ID 0x0f
#define DEFAULT_PES_H264_STREAM_ID 0x1b

#define PES_H264_PID 256
#define PES_ADTS_PID 257
#define PES_H264_HEADER_SIZE 14
#define PES_ADTS_HEADER_SIZE 14

// From ISO/IEC 13818-1, "Table 2-18 – Stream_id assignments"
constexpr uint8_t kPesStreamIdAudioStream = 0xc0;
constexpr uint8_t kPesStreamIdVideoStream = 0xe0;

#define MPEGTS_PACKET_SIZE 188
#define MPEGTS_HEADER_SIZE 4

struct OutputStream {
  int pes_pid;
  int pes_cc = 0;
  bool pes_initialized = false;

  int frame_size_bytes = 0;
  const u_char* data = nullptr;

  unsigned long pcr = INITIAL_PCR;
  unsigned long pts = 0;
};

TsMuxerClass::TsMuxerClass(uint32_t flags) : config_flags_(flags) {
  bool hasH264 = flags & TSMUXER_HAS_H264;
  bool hasAAC = flags & TSMUXER_HAS_AAC;
  if (!hasH264 && !hasAAC) {
    assert("Flags must indicate HAS_H264 and/or HAS_AAC");
    return;
  }
  if (hasAAC) {
    streams_[Codec::AAC] = OutputStream();
    streams_[Codec::AAC].pes_pid = PES_ADTS_PID;
  }
  if (hasH264) {
    streams_[Codec::H264] = OutputStream();
    streams_[Codec::H264].pes_pid = PES_H264_PID;
  }

  buildPmt();
}

// Table 2-28 of ISO 13818-1
void TsMuxerClass::buildPmt() {
  pmt_ = {0x47, 0x50, 0x00, 0x10};

  pmt_.push_back(0x00);  // pointer_field
  pmt_.push_back(0x02);  // table_id
  pmt_.push_back(0xb0);  // section_syntax_indicator ('1') + '0' + reserved
                         // (2bits)  + '00' section_length (2bits)
  pmt_.push_back(0x00);  // section_length (8bits)

  // We will fill in the section length in the end: we don't know it right now
  size_t section_length_last_pos = pmt_.size();

  // Program Number
  pmt_.push_back(0x00);
  pmt_.push_back(0x01);
  // End of Program Number

  pmt_.push_back(0b11000001);  // reserved(2) + version_number(5) +
                               // current_next_indicator(1);
  pmt_.push_back(0x00);        // section_number
  pmt_.push_back(0x00);        // last_section_number

  bool has_h264 = config_flags_ & TSMUXER_HAS_H264;

  uint16_t pcr_pid =
      has_h264 ? PES_H264_PID : PES_ADTS_PID;  // We checked in the constructor that at least one has to exist

  pmt_.push_back(0xe0 | (pcr_pid >> 8));  // reserved(3) + PCR_PID(5) // e1
  pmt_.push_back(pcr_pid);
  uint16_t program_info_length = 0x00;                // length of "descriptors"
  pmt_.push_back(0xf0 | (program_info_length >> 8));  // reserved(4) + program_info_length(4)
  pmt_.push_back(program_info_length);

  uint16_t es_info_length = 0x00;

  // Add H264 only if present
  if (has_h264) {
    pmt_.push_back(DEFAULT_PES_H264_STREAM_ID);
    pmt_.push_back(0xe0 | (PES_H264_PID >> 8));
    pmt_.push_back(0xff & PES_H264_PID);

    pmt_.push_back(0xf0 | es_info_length);  // reserved(4) + ES_info_length(4)
    pmt_.push_back(es_info_length);
  }

  bool has_aac = config_flags_ & TSMUXER_HAS_AAC;

  if (has_aac) {
    pmt_.push_back(DEFAULT_PES_ADTS_STREAM_ID);
    pmt_.push_back(0xe0 | (PES_ADTS_PID >> 8));
    pmt_.push_back(0xff & PES_ADTS_PID);

    pmt_.push_back(0xf0 | es_info_length);  // reserved(4) + ES_info_length(4)
    pmt_.push_back(es_info_length);
  }

  // Fill the length field!! (10 bits)
  uint16_t section_length =
      static_cast<uint16_t>(pmt_.size()) - section_length_last_pos + 4;  // 4 for the CRC not included yet
  pmt_[6] |= (section_length >> 8) & 0x03;
  pmt_[7] = section_length & 0xff;

  // CRC for the "TS_program_map_section" (from "table_id", included)
  uint32_t crc32 = mpegts_crc32(pmt_.data() + 5, pmt_.size() - 5);

  pmt_.push_back(crc32 >> 24);
  pmt_.push_back(crc32 >> 16);
  pmt_.push_back(crc32 >> 8);
  pmt_.push_back(crc32);
}

void TsMuxerClass::writeToBuffer(const u_char* data, size_t len) {
  assert(buffer_.size() < kMaxBufferLength);

  if (static_cast<uint32_t>(buffer_.size()) < (bytes_written_ + len)) {
    buffer_.resize(static_cast<size_t>(bytes_written_) + static_cast<size_t>(len));
  }

#ifdef __STDC_LIB_EXT1__
  memcpy_s(&buffer_[bytes_written_], len, data, len);
#else
  memcpy(&buffer_[bytes_written_], data, len);
#endif

  bytes_written_ += len;
}

void TsMuxerClass::writeToBuffer(const std::vector<u_char>& data) { writeToBuffer(data.data(), data.size()); }

void TsMuxerClass::writePesPacket(const FrameToEncode& frame) {
  // The PES packetization will be fed to the TS packetization, so we need to
  // run the PesTS packet with the updated buffer until the whole frame is
  // consumed
  OutputStream* stream = getCurrentStream();
  stream->pes_initialized = false;
  stream->frame_size_bytes = frame.len;
  stream->data = frame.data;
  stream->pts = frame.pts;

  while (stream->frame_size_bytes > 0) {
    int frame_bytes_written = writeTsPes();

    assert(frame_bytes_written <= stream->frame_size_bytes);
    stream->frame_size_bytes -= frame_bytes_written;
    stream->data += frame_bytes_written;
  }
}

int TsMuxerClass::writeTsPes() {
  // write ts packet header
  std::array<u_char, 4> ts_header = {0x47, 0x00, 0x00, 0x10};
  OutputStream* stream = getCurrentStream();

  ts_header[1] = 0x1f & (stream->pes_pid >> 8);
  ts_header[2] = 0xff & (stream->pes_pid);
  ts_header[3] |= (stream->pes_cc % 16);

  if (!stream->pes_initialized) {
    ts_header[1] |= 0x40;  // set payload start field
  }

  int adaptation_field_length = adaptationFieldLength();

  if (adaptation_field_length > 0) {
    ts_header[3] |= 0x30;  // adaptation_field_control = 3 (has adaptation field
                           // section and ts payload)
  }

  writeToBuffer(ts_header.data(), ts_header.size());
  write_adaptation_field_section();

  if (!stream->pes_initialized) {
    writePesHeader(adaptation_field_length);

    if (stream->pes_pid == PES_H264_PID) {
      writeH264AudIfAbsent();
    }

    stream->pes_initialized = true;
  }

  stream->pes_cc += 1;

  return writePesPayload();
}

bool TsMuxerClass::shouldWritePsi(const uint8_t* data, uint32_t length, uint64_t pts_in_90khz, Codec codec) const {
  if (latest_90khz_pts_psi_written_ == 0) {
    return true;
  }
  if (max_psi_period_in_90khz_ == 0) {
    return true;
  }

  // This should not happen, but just in case
  if (pts_in_90khz < latest_90khz_pts_psi_written_) {
    return true;
  }

  if (codec == Codec::H264) {
    // "is_intra": either it has intra or SPS
    bool is_intra = H264Utils::hasAnnexBNalOfType(
        data, length, {H264Utils::NalType::SequenceParameterSet, H264Utils::NalType::SliceIdrPicture});
    return is_intra;
  }

  // For AAC:
  if (pts_in_90khz - latest_90khz_pts_psi_written_ > max_psi_period_in_90khz_) {
    return true;
  }

  return false;
}

void TsMuxerClass::writePsiIfNecessary(const uint8_t* data, uint32_t length, uint64_t pts_in_90khz, Codec codec) {
  if (!shouldWritePsi(data, length, pts_in_90khz, codec)) {
    return;
  }

  writePat();
  writePmt();

  latest_90khz_pts_psi_written_ = pts_in_90khz;
}

u_char* TsMuxerClass::muxAac(const u_char* data, int data_len, unsigned long pts_in_90khz, int* muxed_len) {
  bytes_written_ = 0;  // Reset the buffer pointer

  // Do nothing if configuration specifies no audio.
  if (!(config_flags_ & TSMUXER_HAS_AAC) || streams_.find(Codec::AAC) == streams_.end()) {
    *muxed_len = 0;
    return nullptr;
  }
  // Time base for ts is 90000
  unsigned long pts_in_ts_timebase = pts_in_90khz;
  streams_.at(Codec::AAC).pts = pts_in_ts_timebase;

  FrameToEncode frame_to_encode;
  frame_to_encode.data = data;
  frame_to_encode.len = data_len;
  frame_to_encode.codec = Codec::AAC;
  frame_to_encode.pts = (int)pts_in_ts_timebase;

  current_stream_ = &streams_.at(Codec::AAC);
  current_stream_->pcr += AUDIO_FRAME_CLOCK;

  writePsiIfNecessary(data, data_len, pts_in_90khz, Codec::AAC);
  writePesPacket(frame_to_encode);

  *muxed_len = bytes_written_;
  return buffer_.data();
}

u_char* TsMuxerClass::muxH264(const u_char* data, int data_len, unsigned long pts_in_90khz, int* muxed_len) {
  bytes_written_ = 0;  // Reset the buffer pointer

  // Do nothing if configuration specifies no h264.
  if (!(config_flags_ & TSMUXER_HAS_H264) || streams_.find(Codec::H264) == streams_.end()) {
    *muxed_len = 0;
    return nullptr;
  }

  // Time base for ts is 90000
  unsigned long pts_in_ts_timebase = pts_in_90khz;
  streams_.at(Codec::H264).pts = pts_in_ts_timebase;

  FrameToEncode frame_to_encode;
  frame_to_encode.data = data;
  frame_to_encode.len = data_len;
  frame_to_encode.codec = Codec::H264;
  frame_to_encode.pts = (int)pts_in_ts_timebase;

  current_stream_ = &streams_.at(Codec::H264);
  current_stream_->pcr += VIDEO_FRAME_CLOCK;

  writePsiIfNecessary(data, data_len, pts_in_90khz, Codec::H264);
  writePesPacket(frame_to_encode);

  *muxed_len = bytes_written_;
  return buffer_.data();
}

OutputStream* TsMuxerClass::getCurrentStream() const { return current_stream_; }

void TsMuxerClass::writeStuffingBytes() {
  const u_char stuffing_byte = 0xff;

  int bytes_written_in_ts_packet = bytes_written_ % MPEGTS_PACKET_SIZE;
  if (bytes_written_in_ts_packet == 0) {
    return;
  }

  // stuffing bytes
  int len_stuffing = MPEGTS_PACKET_SIZE - bytes_written_in_ts_packet;
  for (size_t i = 0; i < len_stuffing; i++) {
    writeToBuffer(&stuffing_byte, 1);
  }
}

/**
 Payload references the Program Map Table (PID 4096)
 */
void TsMuxerClass::writePat() {
  std::array<u_char, 4> pat_header = {0x47, 0x40, 0x00, 0x10};  // 4 bytes  010 0000000000000 00 01 0000
  std::array<u_char, 13> pat_data_bytes = {0x00, 0x00, 0xb0, 0x0d, 0x00, 0x01, 0xc1,
                                           0x00, 0x00, 0x00, 0x01, 0xf0, 0x00};
  std::array<u_char, 4> pat_crc_32 = {0x2a, 0xb1, 0x04, 0xb2};
  // Set continuity counter
  pat_header[3] = (pat_header[3] & 0xf0) | pat_cc_;

  writeToBuffer(pat_header.data(), pat_header.size());
  writeToBuffer(pat_data_bytes.data(), pat_data_bytes.size());
  writeToBuffer(pat_crc_32.data(), pat_crc_32.size());

  pat_cc_ = (pat_cc_ + 1) % 16;
  writeStuffingBytes();
}

/**
 PID at ts header (from PAT: 4096 = 0x1000)
 Payload describes the stream types

 PID 0100 (256) -> Stream type 1b H.264/14496-10 video (MPEG-4/AVC)
 PID 0101 (257) -> Stream type 0f 13818-7 Audio with ADTS transport syntax
 */
void TsMuxerClass::writePmt() {
  // Set continuity counter
  pmt_[3] = (pmt_[3] & 0xf0) | pmt_cc_;
  writeToBuffer(pmt_);

  // continuity counter is a 4 bit field that must be reseted on overflow
  pmt_cc_ = (pmt_cc_ + 1) % 16;
  writeStuffingBytes();
}

bool TsMuxerClass::packetHasPcr() {
  //	output_stream* stream = get_current_stream(writer);
  //	return stream->pes_pid == PES_H264_PID && get_nalu_type(stream->frame)
  //== SPS;
  return false;  // FOR AAC
}
//
/*
 A PES packet will have an adaptation field if:
 - it has an I-frame, so we need to include the PCR
 - it is the beginning of an ADTS block
 - the remaining frame size is not enough to fill the ts packet, so we need to
 insert stuffing bytes on the adaptation field section
 */
int TsMuxerClass::adaptationFieldLength() {
  OutputStream* stream = getCurrentStream();
  int pes_header_size = stream->pes_pid == PES_H264_PID ? PES_H264_HEADER_SIZE : PES_ADTS_HEADER_SIZE;
  int afsize = 0;

  if (packetHasPcr()) {
    afsize += 8;  // 6 bytes from PCR + 2 bytes from AF fields
  }

  // Determine if we'll need to add stuffing bytes
  int curr_pkt_size = MPEGTS_HEADER_SIZE + afsize;

  if (packetHasPcr() || !stream->pes_initialized) {
    curr_pkt_size += pes_header_size;
  }

  if (MPEGTS_PACKET_SIZE > (curr_pkt_size + stream->frame_size_bytes)) {
    afsize += (MPEGTS_PACKET_SIZE - stream->frame_size_bytes - curr_pkt_size);
  }

  return afsize;
}

void TsMuxerClass::write_adaptation_field_section() {
  /*															 	bits
   adaptation_field_length
   8 if (adaptation_field_length > 0) { discontinuity_indicator
   1 random_access_indicator
   1 elementary_stream_priority_indicator 1 PCR_flag
   1 OPCR_flag
   1 splicing_point_flag
   1 transport_private_data_flag
   1 adaptation_field_extension_flag 1
   }
   if (PCR_flag == '1') {
   program_clock_reference_base
   33 reserved
   6 program_clock_reference_extension 9
   }
   for (i = 0; i < N; i++) {
   stuffing_byte
   8
   }
   */
  int adapfield_size = adaptationFieldLength();

  if (adapfield_size == 0) {
    return;
  }

  long pcr = getCurrentStream()->pcr;
  int bytes_written = 0;

  std::vector<u_char> field;
  field.resize(adapfield_size);

  field[0] = adapfield_size - 1;

  if (adapfield_size == 1) {
    writeToBuffer(field);
    return;
  }

  field[1] = 0x00;
  bytes_written += 1;

  if (packetHasPcr()) {
    field[1] |= 0x50;  // Set PCR flag and random access indicator to 1 on I-frames
    // Write PCR
    field[2] = (pcr >> 25);
    field[3] = 0xff & (pcr >> 17);
    field[4] = 0xff & (pcr >> 9);
    field[5] = 0xff & (pcr >> 1);
    field[6] = (0x01 & pcr) << 7;

    // Write reserved and extension field
    field[6] |= 0x7e;
    field[7] = 0x00;
    bytes_written += 6;
  }

  for (unsigned i = 1; i < std::max(adapfield_size - bytes_written, 0); i++) {
    // stuffing bytes
    field[bytes_written + i] = 0xff;
  }

  writeToBuffer(field);
}

/**
 Table 2-17 PES packet "PES_packet()"
 */
void TsMuxerClass::writePesHeader(int adapfield_size) {
  /*
  clang-format off

   index                                bits
   [0][1][2]  packet_start_code_prefix	24
   [3]        stream_id                 8
   [4][5]     PES_packet_length         16
   [6]        '10'                      2
   [6]        PES_scrambling_control    2
   [6]        PES_priority              1
   [6]        data_alignment_indicator  1
   [6]        copyright                 1
   [6]        original_or_copy          1
   [7]        PTS_DTS_flags             2
   [7]        ESCR_flag                 1
   [7]        ES_rate_flag              1
   [7]        DSM_trick_mode_flag       1
   [7]        additional_copy_info_flag	1
   [7]        PES_CRC_flag              1
   [7]        PES_extension_flag        1
   [8]        PES_header_data_length    8

   if (PTS_DTS_flags == '11') {
      '0010'                  4
      PTS [32..30]            3
      marker_bit              1
      PTS [29..15]            15
      marker_bit              1
      PTS [14..0]             15
      marker_bit              1
      '0001'                  4
      DTS [32..30]            3
      marker_bit              1
      DTS [29..15]            15
      marker_bit              1
      DTS [14..0]             15
      marker_bit              1
   }

   clang-format on
   */

  std::vector<u_char> pes_header;
  pes_header.resize(getCurrentStream()->pes_pid == PES_ADTS_PID ? PES_ADTS_HEADER_SIZE : PES_H264_HEADER_SIZE);

  unsigned long pts = getCurrentStream()->pts;
  int aac_size = 0;

  // packet start code prefix 0x000001
  pes_header[0] = 0x00;
  pes_header[1] = 0x00;
  pes_header[2] = 0x01;

  pes_header[6] = 0x80;  // '10' (fixex) + '00' (scramble control) + '0' (priority) + '0'
                         // (alignment) + '0' (copyright) + '0' (original)

  if (streams_.find(Codec::AAC) != streams_.end()) {
    // The size field will finish after byte 5
    aac_size = streams_.at(Codec::AAC).frame_size_bytes + (PES_ADTS_HEADER_SIZE - 6);
  }

  switch (getCurrentStream()->pes_pid) {
    case PES_ADTS_PID:
      pes_header[3] = kPesStreamIdAudioStream;
      // PES packet length: cannot be 0 in audio elementary streams
      pes_header[4] = aac_size >> 8;
      pes_header[5] = 0xff & aac_size;
      pes_header[7] = 0x80;  // Setting PTS_DTS flag to 10 (PTS only)
      pes_header[8] = 0x05;  // Size of optional PES fields (5 bytes only when
                             // PTS is present)
      TsMuxerUtils::writePts(&pes_header[9], 0b0010, pts);
      break;

    case PES_H264_PID:
      pes_header[3] = kPesStreamIdVideoStream;
      pes_header[4] = 0x00;  // Size undefined
      pes_header[5] = 0x00;
      pes_header[7] = 0x80;  // Setting PTS_DTS flag to 10 (PTS only)
      pes_header[8] = 0x05;  // Size of optional PES fields (5 bytes only when
                             // PTS is present)
      TsMuxerUtils::writePts(&pes_header[9], 0b0010, pts);
      break;
    default:
      assert(false);
  }

  writeToBuffer(pes_header);
}

int TsMuxerClass::writePesPayload() {
  OutputStream* stream = getCurrentStream();
  int max_possible_size_to_write = MPEGTS_PACKET_SIZE - (this->bytes_written_ % MPEGTS_PACKET_SIZE);
  unsigned bytes_to_write = std::min(max_possible_size_to_write, stream->frame_size_bytes);

  writeToBuffer(stream->data, bytes_to_write);

  return bytes_to_write;
}

void TsMuxerClass::writeH264AudIfAbsent() {
  // Write AUD if it's not already in the bitstream
  OutputStream* stream = getCurrentStream();
  const u_char* d = stream->data;
  const uint8_t kAudCode = 9;

  uint8_t start_code_length = 0;

  if (stream->frame_size_bytes < 4) {
    return;
  }

  if (d[1] == 0x00 && d[2] == 0x00 && d[3] == 0x01) {
    start_code_length = 4;
  } else if (d[0] == 0x00 && d[1] == 0x00 && d[2] == 0x01) {
    start_code_length = 3;
  }

  uint8_t nalu_code = stream->data[start_code_length] & 0x1f;

  if (nalu_code != kAudCode) {
    const u_char aud[6] = {0x00, 0x00, 0x00, 0x01, 0x09, 0xf0};  // f = all pic types + stop bit;
    writeToBuffer(aud, 6);
  }
}
