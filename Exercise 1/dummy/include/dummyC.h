#pragma once

#ifndef SSI_CONSUMER_DummyC_H
#define SSI_CONSUMER_DummyC_H

#include "base/IConsumer.h"
#include "ioput/option/OptionList.h"

namespace ssi {

class DummyC : public IConsumer {

public:

class Options : public OptionList {

	public:

		Options () : flag(false) {
			addOption ("flag", &flag, 1, SSI_BOOL, "example option");
		};

		bool flag;
	};

public:
	static const ssi_char_t *GetCreateName () { return "DummyC"; };
	static IObject *Create (const ssi_char_t *file) { return new DummyC (file); };
	~DummyC ();
	DummyC::Options *getOptions () { return &_options; };
	const ssi_char_t *getName () { return GetCreateName (); };
	const ssi_char_t *getInfo () { return "DummyC info"; };

	void consume_enter (ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]);
	void consume (IConsumer::info consume_info,
		ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]);
	void consume_flush (ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]);

protected:

	DummyC (const ssi_char_t *file = 0);
	DummyC::Options _options;
	ssi_char_t *_file;
};

}

#endif
