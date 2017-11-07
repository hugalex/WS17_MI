#include "DummyC.h"

namespace ssi {

	DummyC::DummyC(const ssi_char_t *file) : _file(0) {

		if (file) {
			if (!OptionList::LoadXML(file, &_options)) {
				OptionList::SaveXML(file, &_options);
			}
			_file = ssi_strcpy(file);
		}
	}

	DummyC::~DummyC() {

		if (_file) {
			OptionList::SaveXML(_file, &_options);
			delete[] _file;
		}
	}

	void DummyC::consume_enter(ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]) {
	};

	void DummyC::consume(IConsumer::info consume_info,
		ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]) {

		// test for valid input
		if (stream_in[0].dim != 2)
			ssi_wrn("unexpected stream dimension");

		// get input
		float *in = ssi_pcast(float, stream_in[0].ptr);

		for (int j = 0; j< stream_in[0].num; j++)
		{

			for (int i = 0; i < stream_in[0].dim; i++)
			{
				if (!_options.flag)
				{
					ssi_print("value = %f\n", in[j*stream_in[0].dim + i]); //simply output value
				}
				else
				{
					ssi_print("%f -> %f\n", in[j*stream_in[0].dim + i], in[j*stream_in[0].dim + i] * -1); //invert value	
				}
			}
		}
	}

	void DummyC::consume_flush(ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]) {
	};

}