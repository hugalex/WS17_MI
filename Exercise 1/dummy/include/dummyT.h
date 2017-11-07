#pragma once

#ifndef SSI_TRASNFORMER_DummyT_H
#define SSI_TRASNFORMER_DummyT_H

#include "base/ITransformer.h"
#include "ioput/option/OptionList.h"

namespace ssi {

class DummyT : public ITransformer {

public:

class Options : public OptionList {

	public:

		Options () : flag(false) {
			addOption ("flag", &flag, 1, SSI_BOOL, "example option");
		};

		bool flag;
	};

public:
	static const ssi_char_t *GetCreateName () { return "DummyT"; };
	static IObject *Create (const ssi_char_t *file) { return new DummyT (file); };
	~DummyT ();
	DummyT::Options *getOptions () { return &_options; };
	const ssi_char_t *getName () { return GetCreateName (); };
	const ssi_char_t *getInfo () { return "DummyT info"; };

	void transform_enter (ssi_stream_t &stream_in,
		ssi_stream_t &stream_out,
		ssi_size_t xtra_stream_in_num = 0,
		ssi_stream_t xtra_stream_in[] = 0);
	void transform (ITransformer::info info,
		ssi_stream_t &stream_in,
		ssi_stream_t &stream_out,
		ssi_size_t xtra_stream_in_num = 0,
		ssi_stream_t xtra_stream_in[] = 0);
	void transform_flush (ssi_stream_t &stream_in,
		ssi_stream_t &stream_out,
		ssi_size_t xtra_stream_in_num = 0,
		ssi_stream_t xtra_stream_in[] = 0);

	ssi_size_t getSampleDimensionOut (ssi_size_t sample_dimension_in) { 

		return sample_dimension_in; 
	};
	ssi_size_t getSampleBytesOut (ssi_size_t sample_bytes_in) { 

		return sample_bytes_in; 
	}
	ssi_type_t getSampleTypeOut (ssi_type_t sample_type_in) {

		return sample_type_in;
	}
	ssi_size_t getSampleNumberOut (ssi_size_t sample_number_in) { 

		return sample_number_in; 
	}

protected:

	DummyT (const ssi_char_t *file = 0);
	DummyT::Options _options;
	ssi_char_t *_file;
};

}

#endif
