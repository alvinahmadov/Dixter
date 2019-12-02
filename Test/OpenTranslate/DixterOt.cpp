#include <sstream>

#include "Macros.hpp"
#include "Exception.hpp"
#include "Utilities.hpp"

inline std::ostream&
operator<<(std::ostream& out, const std::stringstream& ss)
{
	out << ss.str();
	return out;
}

#ifdef DICT_TESTING

#include <cppconn/resultset.h>

#include "JoinThread.hpp"
#include "Configuration.hpp"
#include "Constants.hpp"
#include "Database/Manager.hpp"
#include "Database/Value.hpp"
#include "OpenTranslate/Dictionary.hpp"

using TDatabaseManager = Dixter::Database::TManager;
using TDatabaseManagerPtr = std::shared_ptr<TDatabaseManager>;
using TStringVector = std::vector<Dixter::TString>;
using Dixter::OpenTranslate::TDictionary;
using Dixter::TString;
using Dixter::NodeKey;
using Dixter::StopWatch;
using Dixter::g_whiteSpace;
using Dixter::g_guiConfigPath;
using Dixter::g_endLine;

void readData(int argc, char** argv);

void writeData();

void readDB(TDatabaseManagerPtr dbManager, const TString& table, const TStringVector& cols);

#elif defined(OTR_TESTING)

#include <future>
#include <fstream>
#include "OpenTranslate/SentenceAnalyzer.hpp"

using namespace Dixter;
using namespace OpenTranslate;

cuhost void otr_tokenizer_test();

#elif defined(NNET_TEST)

#include "ANNet/TNeuron.h"
#include "ANNet/ActivationFunctions"

using namespace dix::NNet;

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

void readData(int argc, char** argv)
{
	printl_log("\tDictionary Submodule")
	if (argc <= 2)
	{
		println("Usage: <user_name> <password> <word_to_search>")
		return;
	}
	
	dxTIMER_START
	auto __confMan = Dixter::getIniManager({ g_guiConfigPath })->accessor();
	
	const TString __host(__confMan->getValue(NodeKey::kDatabaseHostNode).asUTF8());
	const TString __userName(__confMan->getValue(NodeKey::kDatabaseUserNode).asUTF8());
	const TString __password(__confMan->getValue(NodeKey::kDatabasePassNode).asUTF8());
	
	const TString __database(argv[1]);
	const TString __wordToSearch(argv[2]);
	const auto& __column = "paradigm";
	
	auto __dbManager = dxMAKE_SHARED(TDatabaseManager, __host, __userName, __password);
	auto __dict = dxMAKE_SHARED(TDictionary, __dbManager);
	
	__dbManager->selectDatabase(__database);
	
	auto res = __dict->search(__wordToSearch, __column);
	
	uint32_t __count;
	for (const auto&[__key, __valueVector] : res)
		for (uint32_t __index {}; __index < __valueVector.size(); ++__index)
		{
			if (__key == __column)
				printl_log(__valueVector.at(__index));
			__count = __index;
		}
	
	printfm("Read about %i words for %s", __count, __wordToSearch.data());
}

void readDB(TDatabaseManagerPtr dbManager,
			const TString& table)
{
	std::stringstream __dat {};
	static size_t __counter {};
	
	const std::vector<TString> __columns { "id", "word", "paradigm",
										   "category_1", "category_2", "category_3", "category_4",
										   "category_5", "category_6", "category_7", "category_8",
										   "category_9", "category_10", "category_11", "category_12" };
	auto res = dbManager->selectColumns(table, __columns, 0);
	
	while (res->next())
	{
		uint32_t colIndex(1);
		while (colIndex < __columns.size())
		{
			__dat << res->getString(colIndex) << g_whiteSpace;
			++colIndex;
		}
		++__counter;
		println(__dat)
		__dat.clear();
	}
	
	print_log("Read about ")
	prints(__counter)
	println(" table records.")
}

#elif defined(OTR_TESTING)

using IFStreamPtr   = std::unique_ptr<std::ifstream>;
using TStringVector = std::vector<TString>;

TString readAll(IFStreamPtr& stream, char sep = '\n')
{
	std::stringstream ret;
	char __c;
	while (( __c = stream->peek()) != -1)
	{
		auto pos = stream->tellg();
		if (__c != sep)
			ret << __c;
		stream->seekg(pos += std::ifstream::pos_type(1));
	}
	return ret.str();
}

inline void readTestFiles(const TStringVector& fileNames,
						  TStringVector& contents)
{
	if (not fileNames.size())
	{
		printl_log("Nothing to do.");
		return;
	}
	contents.reserve(fileNames.size());
	auto __file = dxMAKE_UNIQUE(std::ifstream);
	
	for (const auto& fileName : fileNames)
	{
		__file->open(fileName);
		if (__file->is_open())
			contents.push_back(readAll(__file));
		 else
			 printl_log("Couldn't open file.")
		__file->close();
	}
}

cuhost void otr_tokenizer_test()
{
	dxTIMER_START
	auto sa = dxMAKE_UNIQUE(TSentenceAnalyzer);
	try
	{
		TStringVector contents, fileNames {
				"../../../Data/wiki/isaac_newton_no.txt"
				,"../../../Data/wiki/isaac_newton_de.txt"
				,"../../../Data/wiki/isaac_newton_fr.txt"
		};
		
		readTestFiles(fileNames, contents);
		
		for (const auto& content : contents)
		{
			sa->process(content);
			println(sa->toString())
		}
	}
	catch (Dixter::TException& e)
	{
		printerr(e.getMessage())
	}
}

#elif defined(NNET_TEST)

void nnet_test(int inputDim)
{
	using namespace dix;
	
	inputs = dxMAKE_UNIQUE(vector<double>);
	Dixter::NNet::generateRandomData(inputDim, inputs);
	
	IActivation* activation = new HyperTangensActivation();
	
	auto neuron = new TNeuron(inputDim, activation);
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
