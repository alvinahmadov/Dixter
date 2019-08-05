#include <boost/timer.hpp>
#include "Randomizer.hpp"
#include "Utilities.hpp"
#include "Commons.hpp"
#include <armadillo>

#ifdef DICT_TESTING

#include "JoinThread.hpp"
#include "Database/Manager.hpp"
#include "Database/Value.hpp"
#include "OpenTranslate/Dictionary.hpp"

void readData(char** argv);

void writeData();

void readDB(Dixter::Database::Manager* dbManager, const Dixter::string_t& table, const std::vector<Dixter::string_t>& cols);

#elif defined(OTR_TESTING)

#include <future>
#include "OpenTranslate/Tokenizer.hpp"
#include "Utilities.h"

using namespace Dixter;
using namespace OpenTranslate;

void otr_test();

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

int main(int argc, char** argv)
{
	printl_log("Translation Module")
	BENCH_BEGIN
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
	
	BENCH_END
	return 0;
}

#ifdef DICT_TESTING

void readData(char** argv)
{
	printl_log("Translation Module")
	using namespace Dixter;
	std::vector<string_t> tables = {"D", "E", "OE"};
	const string_t host = "127.0.0.1";
	const string_t database = "dixterdb_NO";
	const string_t userName = argv[1];
	const string_t password = argv[2];
	auto dbManager = new Database::Manager(host, database, userName, password);
	std::vector<string_t> columns {"id", "word", "paradigm",
	                               "category_1", "category_2", "category_3", "category_4",
	                               "category_5", "category_6", "category_7", "category_8",
	                               "category_9", "category_10", "category_11", "category_12"};

	auto dict = Dixter::OpenTranslate::Dictionary(dbManager);
	const string_t& paradigmKey = "paradigm";
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

void readDB(Dixter::Database::Manager* dbManager, const Dixter::string_t& table, const std::vector<Dixter::string_t>& cols)
{
	std::mutex mtx;
	std::unique_lock l(mtx);
	
	Dixter::string_t dat {};
	static size_t counter {};
	auto res = dbManager->selectColumns(table, cols, 0);
	
	while (res->next())
	{
		Dixter::ui32 colIndex {1};
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
	l.unlock();
}

#elif defined(OTR_TESTING)

void otr_test() try
{
	string_t datFileNo = "../../../data/wiki/isaac_newton_no.txt";
	string_t datFileDe = "../../../data/wiki/isaac_newton_de.txt";
	
	auto fsNo = new FileInputStream(datFileNo);
	auto fsDe = new FileInputStream(datFileDe);
	string_t str1 = fsNo->readAll('\n');
	string_t str2 = fsDe->readAll('\n');
	
	Tokenizer* tok = new Tokenizer();
	
	tok->tokenize(str1);
	tok->tokenize(str2);
	
	printl_log(tok->getTokenData())
	
	SAFE_RELEASE(fsDe)
	SAFE_RELEASE(fsNo)
	SAFE_RELEASE(tok)
} catch (std::exception& e)
{
	printerr(e.what())
}

#elif defined(NNET_TEST)

void nnet_test(int inputDim)
{
	using namespace dix;
	
	auto inputs = new dix::dxVector<double> {};
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
