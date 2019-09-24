/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <memory>
#include <mutex>

#include "Configuration.hpp"
#include "Utilities.hpp"
#include "Exception.hpp"
#include "Commons.hpp"
#include "NodeData.hpp"
#include "Constants.hpp"

using namespace std;

namespace Dixter
{
	namespace StringUtils = Utilities::Strings;
	namespace AlgoUtils = Utilities::Algorithms;
	template<typename T>
	using TList = std::list<T>;
	
	#ifdef HAVE_CXX17
	template<typename T>
	using scoped_lock = std::scoped_lock<T>;
	#else
	template<typename T>
	using scoped_lock = std::unique_lock<T>;
	#endif
	
	static std::set<TString> g_confPath { g_langConfigPath, g_voiceConfigPath, g_panelConfigPath };
	
	static std::set<TString> g_confPathIni { g_panelConfigPathIni };
	
	TNodeData* xmlLoadHelper(const PropertyTree& prop, const TString& parent, std::function<
			TString(const TString&, const TUString&,
					const TUString&, bool)
	> fn)
	{
		TNodeData* __configData(new TNodeData);
		const TString __attributeKey("<xmlattr>");
		TString __nodeName;
		for (const PropertyTree::value_type& __node : prop)
		{
			auto __nodeValue = __node.second.get_value<TUString>();
			// Not attribute
			if (__node.first != __attributeKey)
			{
				if (__node.second.size() > 0)
				{
					__nodeName = fn(__node.first, TUString(), TUString(), false);
					for (const PropertyTree::value_type& __withAttr : __node.second)
					{
						auto __nodeNameTemp = __nodeName;
						__nodeName = fn(__withAttr.first, TUString(), TUString(), true);
						__configData->insertData(__nodeName, __withAttr.second.get_value<TUString>(), parent);
						__nodeName = __nodeNameTemp;
					}
				}
				else
				{
					__nodeName = fn(__node.first, TUString(), TUString(), false);
					__configData->insertData(__nodeName, __nodeValue, parent);
				}
			}
			else
			{
				if (__node.second.size() > 1)
				{
					xmlLoadHelper(__node.second, parent, fn);
				}
				else
				{
					__nodeName = fn(
							__node.second.front().first, __node.second.front().first,
							__node.second.front().second.get_value<TUString>(), false);
					if (not __nodeValue.empty())
					{
						__configData->insertData(
								__nodeName, __nodeValue, parent, __node.second.front().first,
								__node.second.front().second.get_value<TUString>());
					}
				}
			}
		}
		return __configData;
	}
	
	TString xmlAttributeNameBuilder(const TUString& attributeName, const TUString& attributeValue)
	{
		static TString __rootNameWithAttr;
		
		if (not attributeValue.empty() and not attributeName.empty())
		{
			if (not __rootNameWithAttr.empty())
				__rootNameWithAttr.clear();
			__rootNameWithAttr.push_back('<');
			__rootNameWithAttr.append(attributeValue.asUTF8()).push_back('>');
		}
		
		return __rootNameWithAttr;
	}
	
	// INIConfiguration implementation
	TConfigurationINI::TConfigurationINI(const TString& file) noexcept
			: m_file(file),
			  m_propertyTree(new PropertyTree),
			  m_entries(new TNodeEntry)
	{
		try
		{
			boost::property_tree::ini_parser::read_ini<PropertyTree>(m_file, *m_propertyTree);
		}
		catch (boost::exception& e)
		{
			printerr("Read ini error")
		}
	}
	
	void TConfigurationINI::load()
	{
		TNodeData* __configData(new TNodeData);
		
		for (const auto& __node : *m_propertyTree)
			__configData->insertData(__node.first, __node.second.get_value<TUString>());
		
		m_entries->insertEntry(__configData);
	}
	
	void TConfigurationINI::save()
	{
		println("Not implemented yet")
	}
	
	void TConfigurationINI::set(const TString& key, const TUString& value)
	{
		m_entries->setEntry(key, value);
	}
	
	void TConfigurationINI::keys(list<TString>& keyList) const
	{
		for (auto& __treeKey : *m_propertyTree)
			keyList.push_back(__treeKey.first);
	}
	
	void TConfigurationINI::get(const TString& key,
								std::vector<TUString>& values) const
	{
		for (const auto& __data : *m_entries)
			__data.second->getValues(key, values);
	}
	
	TUString
	TConfigurationINI::get(const TString& key,
						   dxMAYBE_UNUSED const TUString& byValue) const
	{
		return get(key);
	}
	
	TUString
	TConfigurationINI::get(const TString& key) const
	{
		return m_entries->findEntry(key)->m_value;
	}
	
	// XMLConfiguration implementation
	TConfigurationXML::TConfigurationXML(const TString& file) noexcept
			: m_file(file),
			  m_entries(new TNodeEntry),
			  m_propertyTree(new PropertyTree)
	{
		try
		{
			boost::property_tree::xml_parser::read_xml<PropertyTree>(m_file, *m_propertyTree);
		}
		catch (std::exception& e)
		{
			printerr("Error reading XML: " << e.what())
			return;
		}
	}
	
	void TConfigurationXML::load()
	{
		lock_guard<mutex> __lg(m_mutex);
		
		TString __parent;
		m_rootNode = m_propertyTree->front().first;
		const auto& __rootNodeName = m_propertyTree->front().first;
		const auto& __childTree = m_propertyTree->front().second;
		
		auto __f_createNodeName = [ &__parent, &__rootNodeName ]
				(const TString& node, const TUString& attributeName = TUString(),
				 const TUString& attributeVal = TUString(), bool child = false)
		{
			static TString __nodeName {};
			if (not child)
			{
				__nodeName.clear();
				StringUtils::buildPath(__nodeName, '.', __rootNodeName, __parent);
				StringUtils::buildPath(__nodeName, '.', xmlAttributeNameBuilder(attributeName, attributeVal), node);
			}
			else
				StringUtils::buildPath(__nodeName, '.', node);
			
			return __nodeName;
		};
		
		for (const auto& __value : __childTree)
		{
			__parent.clear();
			__parent = __value.first;
			m_entries->insertEntry(xmlLoadHelper(__value.second, __parent, __f_createNodeName));
		}
	}
	
	void TConfigurationXML::save()
	{
		throw TNotImplementedException("%s:d% Saving XML configuration not implemented yet", __FILE__, __LINE__);
	}
	
	void TConfigurationXML::set(const TString& key, const TUString& value)
	{
		m_entries->setEntry(key, value);
	}
	
	void TConfigurationXML::keys(list<TString>& keyList) const
	{
		keyList.push_back(m_rootNode);
	}
	
	TUString TConfigurationXML::get(const TString& key) const
	{
		return m_entries->findEntry(key)->m_value;
	}
	
	TUString TConfigurationXML::get(const TString& key, const TUString& neighborValue) const
	{
		for (const auto& __confData : *m_entries)
		{
			for (const auto& __node : __confData.second->getNodes())
			{
				if (__node->m_value.compare(neighborValue) == 0)
					return m_entries->findEntryData(__confData.first)->getValue(key);
			}
		}
	}
	
	void TConfigurationXML::get(const TString& key, std::vector<TUString>& values) const
	{
		for (const auto& __data : *m_entries)
			__data.second->getValues(key, values);
	}
	
	// ConfigurationProxy implementation
	TConfigurationFactory::TConfigurationFactory(const TString& configPath, EConfiguration type)
			: m_type(type)
	{
		switch (m_type)
		{
			case EConfiguration::INI:
				m_configuration = dxMAKE_SHARED(TConfigurationINI, configPath);
				break;
			case EConfiguration::XML:
				m_configuration = dxMAKE_SHARED(TConfigurationXML, configPath);
				break;
			case EConfiguration::JSON :
				throw TNotImplementedException("%s:%d Reading configuration from JSON is not implemented yet.\n", __FILE__, __LINE__);
			default:
				throw TIllegalArgumentException("%s:%d Unknown type.\n", __FILE__, __LINE__);
		}
	}
	
	void TConfigurationFactory::load()
	{
		if (not m_configuration)
			throw TIllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
		
		m_configuration->load();
	}
	
	void TConfigurationFactory::save()
	{
		if (not m_configuration)
			throw TIllegalArgumentException("%s:%d Configuration is not initialised.");
		
		m_configuration->save();
	}
	
	inline void TConfigurationFactory::keys(std::list<TString>& keyList) const
	{
		m_configuration->keys(keyList);
	}
	
	const std::shared_ptr<IConfiguration>&
	TConfigurationFactory::getConfiguration()
	{
		return m_configuration;
	}
	
	EConfiguration
	TConfigurationFactory::getType() const
	{
		return m_type;
	}
	
	///ConfigurationManager implementation
	TConfigurationManager::TInstancePtr&
	TConfigurationManager::getManager(EConfiguration type, std::set<TString> paths)
	{
		m_instance.reset(new TSelf(type, ( paths.empty() ? g_confPath : paths )));
		return m_instance;
	}
	
	const TConfigurationManager::TAccessor*
	TConfigurationManager::getAccessor() const
	{
		return m_accessor;
	}
	
	TConfigurationManager::TMutator*
	TConfigurationManager::getMutator()
	{
		return m_mutator;
	}
	
	void TConfigurationManager::read(EConfiguration type, const TString& path)
	{
		m_factory.reset(new TConfigurationFactory(path, type));
		TList<TString> __keyList;
		m_factory->load();
		m_factory->keys(__keyList);
		for (auto& __key : __keyList)
			m_properties.emplace(__key, m_factory->getConfiguration());
	}
	
	void TConfigurationManager::write(EConfiguration type, const TString& path)
	{
		dxUNUSED(path)
		switch (type)
		{
			case EConfiguration::XML :
				printl_log("XML write")
				break;
			case EConfiguration::JSON :
				printl_log("JSON write")
				break;
			case EConfiguration::INI :
				printl_log("INI write")
				break;
			default:
				printl_log("None")
				break;
		}
	}
	
	void TConfigurationManager::checkKey(const TConfigurationManager::TConstIterator key,
										 TString errorMsg) const
	{
		if (key == m_properties.end())
			throw TNotFoundException(errorMsg);
	}
	
	TConfigurationManager::TConfigurationManager(EConfiguration type, const std::set<TString>& paths)
			: m_factory(nullptr),
			  m_properties(),
			  m_accessor(new TAccessor(this)),
			  m_mutator(new TMutator(this))
	{
		for (const auto& path : paths)
			this->read(type, path);
	}
	
	TString EConfTypeToString(EConfiguration econf)
	{
		switch (econf)
		{
			case EConfiguration::XML :
				return "XML Conf.";
			case EConfiguration::INI :
				return "INI Conf.";
			case EConfiguration::JSON :
				return "JSON Conf";
			default:
				return "None";
		}
	}
	
	TConfigurationManager::~TConfigurationManager() noexcept
	{
		delete m_accessor;
		delete m_mutator;
	}
	
	TConfigurationManager::
	TAccessor::TAccessor(TConfigurationManager* manager) noexcept
			: m_manager(manager)
	{ }
	
	TUString
	TConfigurationManager::
	TAccessor::getValue(const TString& key, const TString& root) const
	{
		return this->get(m_manager, key, root)->get(key);
	}
	
	TUString
	TConfigurationManager::
	TAccessor::getValue(const TString& key, const TUString& byValue,
						const TString& root) const
	{
		return get(m_manager, key, root)->get(key, byValue);
	}
	
	const TConfigurationManager::TAccessor*
	TConfigurationManager::
	TAccessor::getValues(const TString& key, vector<TUString>& values, const TString& root) const
	{
		get(m_manager, key, root)->get(key, values);
		return this;
	}
	
	IConfiguration*
	TConfigurationManager::
	TAccessor::get(TConfigurationManager* manager, const TString& key, const TString& root)
	{
		auto __iter = manager->m_properties.find(root.empty() ? key : root);
		snprintfm(__msg, "Key \"%s\" not found", key.data());
		manager->checkKey(__iter, __msg);
		delete[] __msg;
		
		return __iter->second.get();
	}
	
	TConfigurationManager::
	TMutator::TMutator(TConfigurationManager* manager) noexcept
			: m_manager(manager)
	{ }
	
	const TConfigurationManager::TMutator*
	TConfigurationManager::
	TMutator::setValue(const TString& key, const TUString& value, const TString& root)
	{
		scoped_lock<mutex> lockGuard(m_mutex);
		this->get(key, root)->set(key, value);
		return this;
	}
	
	IConfiguration*
	TConfigurationManager::
	TMutator::get(const TString& key, const TString& root)
	{
		auto __iter = m_manager->m_properties.find(root.empty() ? key : root);
		snprintfm(__msg, "Key \"%s\" not found", key.data());
		m_manager->checkKey(__iter, __msg);
		delete[] __msg;
		
		return __iter->second.get();
	}
	
	TConfigurationManager::TInstancePtr
			TConfigurationManager::TConfigurationManager::m_instance {};
	
	bool TConfigurationManager::update()
	{
		bool success(false);
		// try
		// {
		// 	auto __type = m_factory->getType();
		// 	auto __paths = m_pathList;
		// 	m_instance.reset(new TConfigurationManager(__type, __paths));
		// 	success = true;
		// }
		// catch (Exception& e)
		// {
		// 	printerr(e.getMessage())
		// }
		// if (success)
		// 	println("Updated")
		return success;
	}
	
	EConfiguration
	TConfigurationManager::getType() const
	{
		return m_factory->getType();
	}
	
	bool TConfigurationManager::release()
	{
		m_instance.reset();
		return m_instance == nullptr;
	}
}