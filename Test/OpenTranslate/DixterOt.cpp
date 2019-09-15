#include "Randomizer.hpp"
#include "Utilities.hpp"
#include "Commons.hpp"

#ifdef DICT_TESTING

#include "JoinThread.hpp"
#include "Database/Manager.hpp"
#include "Database/Value.hpp"
#include "OpenTranslate/Dictionary.hpp"

void readData(char** argv);

void writeData();

void readDB(Dixter::Database::Manager* dbManager, const Dixter::TString& table, const std::vector<Dixter::TString>& cols);

#elif defined(OTR_TESTING)

#include <future>
#include <fstream>
#include <sstream>
#include "OpenTranslate/Tokenizer.hpp"

using namespace Dixter;
using namespace OpenTranslate;

cuhost void otr_test();

#elif defined(NNET_TEST)

#include "ANNet/Neuron.h"
#include "ANNet/ActivationFunctions"

using namespace dix::nnet;

void nnet_test(int inputDim);

#define DEFAULT_ARGS
#else
#define DEFAULT_ARGS
#endif

using namespace Dixter::Utilities;

int main(dxMAYBE_UNUSED int argc, dxMAYBE_UNUSED char** argv)
{
	printl_log("Translation Module")
	TIMER_START
	#ifdef DICT_TESTING
	if (argc >= 2)
	{
		printl_log("\tDictionary Submodule")
		readData(argv);
	} else
		printl("Usage: <user_name> <password>")
	#elif defined(OTR_TESTING)
	printl_log("\tOpenTranslate Submodule")
	otr_test();
	#elif defined(NNET_TEST)
	printl_log("\tMachine Learning Submodule")
	const int inputDim = 99;
	int i = 0;
		
	while (i++ != 1)
	{
		nnet_test(inputDim);
		printl("")
	}
	#endif
	
	return 0;
}

#ifdef DICT_TESTING

void readData(char** argv)
{
	printl_log("Translation Module")
	using namespace Dixter;
	std::vector<TString> tables = {"D", "E", "OE"};
	const TString host = "127.0.0.1";
	const TString database = "dixterdb_NO";
	const TString userName = argv[1];
	const TString password = argv[2];
	auto dbManager = new Database::Manager(host, database, userName, password);
	std::vector<TString> columns {"id", "word", "paradigm",
								   "category_1", "category_2", "category_3", "category_4",
								   "category_5", "category_6", "category_7", "category_8",
								   "category_9", "category_10", "category_11", "category_12"};

	auto dict = Dixter::OpenTranslate::Dictionary(dbManager);
	const TString& paradigmKey = "paradigm";
	auto res1 = dict.search("jafse", paradigmKey, true);
	
	printl("1. ")
	for (const auto& pr : res1)
	{
		prints(pr.first)
		prints(" ")
		printl(pr.second)
	}
	
	SAFE_RELEASE(dbManager)
}

void writeData()
{

}

void readDB(Dixter::Database::Manager* dbManager, const Dixter::TString& table, const std::vector<Dixter::TString>& cols)
{
	std::mutex mtx;
	std::unique_lock l(mtx);
	
	Dixter::TString dat {};
	static size_t counter {};
	auto res = dbManager->selectColumns(table, cols, 0);
	
	while (res->next())
	{
		Dixter::UInt32 colIndex {1};
		while (colIndex < cols.size())
		{
			dat.append(res->getString(colIndex)).push_back(' ');
			++colIndex;
		}
		++counter;
		printl(dat)
		dat.clear();
	}
	
	print_log("Read about ")
	prints(counter)
	printl(" table records.")
}

#elif defined(OTR_TESTING)

using IFStreamPtr   = std::unique_ptr<std::ifstream>;
using TStringVector = std::vector<TString>;

TString readAll(IFStreamPtr& stream, char delimiter)
{
	std::stringstream ret;
	char c;
	while (( c = stream->peek()) != -1)
	{
		auto pos = stream->tellg();
		if (c != delimiter)
			ret << c;
		stream->seekg(pos += std::ifstream::pos_type(1));
	}
	
	return ret.str();
}

inline void readTestFiles(const TStringVector& fileNames,
                          TStringVector& contents)
{
	if (not fileNames.size())
	{
		printf("Nothing to do.");
		return;
	}
	contents.reserve(fileNames.size());
	for (const auto& fileName : fileNames)
	{
#if defined(HAVE_CXX14) or defined(HAVE_CXX17)
		auto file = std::make_unique<std::ifstream>(fileName);
#else
		auto file = std::unique_ptr<std::ifstream>(new std::ifstream(fileName));
#endif
		contents.push_back(readAll(file, '\n'));
	}
}

cuhost void otr_test()
{
	auto tokenizer = std::unique_ptr<TTokenizer>(new TTokenizer);
	try
	{
		TStringVector contents, fileNames {
				"../../../Data/wiki/isaac_newton_no.txt",
				// "../../../Data/wiki/isaac_newton_de.txt",
				// "../../../Data/wiki/isaac_newton_fr.txt"
		};
		
		readTestFiles(fileNames, contents);
		
		for (const auto& content : contents)
			tokenizer->tokenize(content);
		
		printl_log(tokenizer->toString())
	}
	catch (Dixter::Exception& e)
	{
		printerr(e.getMessage())
	}
}

#elif defined(NNET_TEST)

void nnet_test(int inputDim)
{
	using namespace dix;
	
	auto inputs = new vector<double> {};
	Dixter::nnet::generateRandomData(inputDim, inputs);
	
	IActivation* activation = new HyperTangensActivation();
	
	auto neuron = new Neuron(inputDim, activation);
	neuron->setInputs(*inputs);
	neuron->process();

	prints("Weights: ")
	printl(toString(neuron->getWeights()))
	prints("Inputs:  ")
	printl(toString(neuron->getInputs()))
	prints("Output:  ")
	printl(neuron->getOutput())
}

#endif
