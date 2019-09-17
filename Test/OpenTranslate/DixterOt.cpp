#include <sstream>
#include "Randomizer.hpp"
#include "Commons.hpp"

inline std::ostream&
operator<<(std::ostream& out, const std::stringstream& ss)
{
	out << ss.str();
	return out;
}

#ifdef DICT_TESTING

#include "JoinThread.hpp"
#include "Database/Manager.hpp"
#include "Database/Value.hpp"
#include "OpenTranslate/Dictionary.hpp"

using TDatabaseManager = Dixter::Database::TManager;
using TDatabaseManagerPtr = std::shared_ptr<TDatabaseManager>;
using TStringVector = std::vector<Dixter::TString>;
using Dixter::OpenTranslate::TDictionary;
using Dixter::TString;
using Dixter::StopWatch;
using Dixter::g_whiteSpace;

void readData(int argc, char** argv);

void writeData();

void readDB(TDatabaseManagerPtr dbManager, const TString& table, const TStringVector& cols);

#elif defined(OTR_TESTING)

#include <future>
#include <fstream>
#include "OpenTranslate/Tokenizer.hpp"

using namespace Dixter;
using namespace OpenTranslate;

cuhost void otr_tokenizer_test();

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
	
	#ifdef DICT_TESTING
	
	readData(argc, argv);
	
	#elif defined(OTR_TESTING)
	printl_log("\tOpenTranslate Submodule")
	otr_tokenizer_test();
	#elif defined(NNET_TEST)
	printl_log("\tMachine Learning Submodule")
	const int inputDim = 99;
	int i = 0;
		
	while (i++ != 1)
	{
		nnet_test(inputDim);
		println("")
	}
	#endif
	
	return 0;
}

#ifdef DICT_TESTING
using Dixter::OpenTranslate::operator<<;

void readData(int argc, char** argv)
{
	printl_log("\tDictionary Submodule")
	if (argc <= 2)
	{
		println("Usage: <user_name> <password> <word_to_search>")
		return;
	}
	
	TIMER_START
	TStringVector tables = { "AE", "D", "E", "F", "OE", "J" };
	const TString host { "127.0.0.1" };
	const TString database { "dixterdb_NO" };
	const TString userName { argv[1] };
	const TString password { argv[2] };
	const TString wordToSearch { argv[3] };
	auto dbManager = TDatabaseManagerPtr(new TDatabaseManager(host, database, userName, password));
	const std::vector<TString> columns { "id", "word", "paradigm",
	                                     "category_1", "category_2", "category_3", "category_4",
	                                     "category_5", "category_6", "category_7", "category_8",
	                                     "category_9", "category_10", "category_11", "category_12" };
	
	auto dict = TDictionary(dbManager, "tables", "original_value");
	const auto& col = columns.at(2);
	auto res = dict.search(wordToSearch, col);
	
	std::stringstream ss {};
	size_t index {};
	for (const auto&[__key, __valueVector] : res)
		for (const auto& __value : __valueVector)
			if (__key == col)
				ss << __value << ( ++index == res.size() ? ".\n" : ", " );
	
	println(ss)
	printfm("Read about %lu words for %s", index, wordToSearch.data());
}

void readDB(TDatabaseManagerPtr dbManager,
            const TString& table,
            const TStringVector& cols)
{
	std::stringstream dat {};
	static size_t counter {};
	auto res = dbManager->selectColumns(table, cols, 0);
	
	while (res->next())
	{
		Dixter::UInt32 colIndex { 1 };
		while (colIndex < cols.size())
		{
			dat << res->getString(colIndex) << g_whiteSpace;
			++colIndex;
		}
		++counter;
		println(dat)
		dat.clear();
	}
	
	print_log("Read about ")
	prints(counter)
	println(" table records.")
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

cuhost void otr_tokenizer_test()
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
	println(toString(neuron->getWeights()))
	prints("Inputs:  ")
	println(toString(neuron->getInputs()))
	prints("Output:  ")
	println(neuron->getOutput())
}

#endif
