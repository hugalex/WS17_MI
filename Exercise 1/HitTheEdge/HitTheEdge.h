#ifndef _HITTHEEDGE_H
#define _HITTHEEDGE_H

#include "base/IConsumer.h"
#include "ioput/option/OptionList.h"
using namespace ssi;

class HitTheEdge : public IConsumer {

public:
	static const ssi_char_t *GetCreateName() { return "HitTheEdge"; };
	static IObject *Create(const ssi_char_t *file) { return new HitTheEdge(file); };
	~HitTheEdge();
	const ssi_char_t *getName() { return GetCreateName(); };
	const ssi_char_t *getInfo() { return "HitTheEdge Solution"; }; //TODO

	OptionList *getOptions() { return 0; };

	void consume_enter(ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]);

	void consume(IConsumer::info consume_info,
		ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]);

	void consume_flush(ssi_size_t stream_in_num,
		ssi_stream_t stream_in[]);

protected:

	HitTheEdge(const ssi_char_t *file = 0);
	
};

#endif //_HITTHEEDGE_H
