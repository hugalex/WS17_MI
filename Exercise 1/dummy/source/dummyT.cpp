#include "DummyT.h"

namespace ssi {

DummyT::DummyT (const ssi_char_t *file) : _file (0) {

	if (file) {
		if (!OptionList::LoadXML (file, &_options)) {
			OptionList::SaveXML (file, &_options);
		}
		_file = ssi_strcpy (file);
	}
}

DummyT::~DummyT () {

	if (_file) {
		OptionList::SaveXML (_file, &_options);
		delete[] _file;
	}
}

void DummyT::transform_enter (ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[]) {
}

void DummyT::transform (ITransformer::info info, ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[]) {

	// test for valid input
	if(stream_in.dim != 2)
		ssi_wrn("unexpected stream dimension");

	// get input
	float *in = ssi_pcast (float, stream_in.ptr);

	// prepare output	
	float *out = ssi_pcast (float, stream_out.ptr);

	// populate output stream
	int out_index = 0;
	for (int j = 0; j< stream_in.num; j++)
	{
		for (int i = 0; i< stream_in.dim; i++)
		{

			if (!_options.flag)
				out[out_index++] = in[j*stream_in.dim + i]; //simply copy input to output
			else
				out[out_index++] = in[j*stream_in.dim + i] * -1; //invert input		
		}
	}
}

void DummyT::transform_flush (ssi_stream_t &stream_in, ssi_stream_t &stream_out, ssi_size_t xtra_stream_in_num, ssi_stream_t xtra_stream_in[]) {
}

}