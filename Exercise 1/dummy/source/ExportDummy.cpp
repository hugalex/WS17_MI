#include "ssidummy.h"
#include "base/Factory.h"

#ifndef DLLEXP
#define DLLEXP extern "C" __declspec( dllexport )
#endif

DLLEXP bool Register(ssi::Factory *factory, FILE *logfile, ssi::IMessage *message) {

	ssi::Factory::SetFactory(factory);

	if (logfile) {
		ssiout = logfile;
	}

	bool result = true;

	result = ssi::Factory::Register(ssi::DummyC::GetCreateName(), ssi::DummyC::Create) && result;
	result = ssi::Factory::Register(ssi::DummyT::GetCreateName(), ssi::DummyT::Create) && result;

	return result;
}
