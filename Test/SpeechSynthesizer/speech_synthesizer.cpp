#include "SpeechSynthesizer/vogen.h"

using namespace std;

int main()
{
	print_log("Speech Synthesizer Module")
	auto aud_gen = new Dixter::vsynth::Vogen();
	aud_gen->generateRandomSoundData(16, 256, Dixter::vsynth::DIST_UNIFORM, af::dtype::s32);
	aud_gen->generateSound(48000, 1, 5);
	return 0;
}