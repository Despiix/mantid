// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_DF12DETSPECMAP_H_
#define FLATBUFFERS_GENERATED_DF12DETSPECMAP_H_

#include "flatbuffers/flatbuffers.h"



struct SpectraDetectorMapping;

struct SpectraDetectorMapping FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_SPECTRUM = 4,
    VT_DETECTOR_ID = 6,
    VT_N_SPECTRA = 8
  };
  const flatbuffers::Vector<int32_t> *spectrum() const { return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_SPECTRUM); }
  const flatbuffers::Vector<int32_t> *detector_id() const { return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_DETECTOR_ID); }
  int32_t n_spectra() const { return GetField<int32_t>(VT_N_SPECTRA, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_SPECTRUM) &&
           verifier.Verify(spectrum()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DETECTOR_ID) &&
           verifier.Verify(detector_id()) &&
           VerifyField<int32_t>(verifier, VT_N_SPECTRA) &&
           verifier.EndTable();
  }
};

struct SpectraDetectorMappingBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_spectrum(flatbuffers::Offset<flatbuffers::Vector<int32_t>> spectrum) { fbb_.AddOffset(SpectraDetectorMapping::VT_SPECTRUM, spectrum); }
  void add_detector_id(flatbuffers::Offset<flatbuffers::Vector<int32_t>> detector_id) { fbb_.AddOffset(SpectraDetectorMapping::VT_DETECTOR_ID, detector_id); }
  void add_n_spectra(int32_t n_spectra) { fbb_.AddElement<int32_t>(SpectraDetectorMapping::VT_N_SPECTRA, n_spectra, 0); }
  SpectraDetectorMappingBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  SpectraDetectorMappingBuilder &operator=(const SpectraDetectorMappingBuilder &);
  flatbuffers::Offset<SpectraDetectorMapping> Finish() {
    auto o = flatbuffers::Offset<SpectraDetectorMapping>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<SpectraDetectorMapping> CreateSpectraDetectorMapping(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::Vector<int32_t>> spectrum = 0,
   flatbuffers::Offset<flatbuffers::Vector<int32_t>> detector_id = 0,
   int32_t n_spectra = 0) {
  SpectraDetectorMappingBuilder builder_(_fbb);
  builder_.add_n_spectra(n_spectra);
  builder_.add_detector_id(detector_id);
  builder_.add_spectrum(spectrum);
  return builder_.Finish();
}

inline const SpectraDetectorMapping *GetSpectraDetectorMapping(const void *buf) { return flatbuffers::GetRoot<SpectraDetectorMapping>(buf); }

inline bool VerifySpectraDetectorMappingBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<SpectraDetectorMapping>(); }

inline const char *SpectraDetectorMappingIdentifier() { return "df12"; }

inline bool SpectraDetectorMappingBufferHasIdentifier(const void *buf) { return flatbuffers::BufferHasIdentifier(buf, SpectraDetectorMappingIdentifier()); }

inline void FinishSpectraDetectorMappingBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<SpectraDetectorMapping> root) { fbb.Finish(root, SpectraDetectorMappingIdentifier()); }


#endif  // FLATBUFFERS_GENERATED_DF12DETSPECMAP_H_
