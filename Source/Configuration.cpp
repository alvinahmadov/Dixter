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
#include "NodeEntry.hpp"
#include "Constants.hpp"

namespace Dixter
{
	namespace NStringUtils = Utilities::Strings;
	#ifdef HAVE_CXX17
	template<typename T>
	using scoped_lock = std::scoped_lock<T>;
	#else
	template<typename T>
	using scoped_lock = std::unique_lock<T>;
	#endif
	
	static std::set<TString> g_confPath { g_langConfigPath, g_voiceConfigPath};
	
	static std::set<TString> g_settingsConfPath { g_guiConfigPath };
	
	TNodeData* xmlLoadHelper(const IConfiguration::PropertyTree& prop, const TString& parent, std::function<
			TString(const TString&, const TUString&,
					const TUString&, bool)
	> fn)
	{
		TNodeData* __configData(new TNodeData);
		const TString __attributeKey("<xmlattr>");
		TString __nodeName;
		for (const auto& __node : prop)
		{
			auto __nodeValue = __node.second.get_value<TUString>();
			// Not attribute
			if (__node.first != __attributeKey)
			{
				if (__node.second.size() > 0)
				{
					__nodeName = fn(__node.first, TUString(), TUString(), false);
					for (const auto& __withAttr : __node.second)
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
		catch (std::exception& e)
		{
			printerr(e.what())
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
	
	void TConfigurationINI::keys(std::list<TString>& keyList) const
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
	
	// TConfigurationXML implementation
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
		std::lock_guard<std::mutex> __lg(m_mutex);
		
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
				NStringUtils::buildPath(__nodeName, '.', __rootNodeName, __parent);
				NStringUtils::buildPath(__nodeName, '.', xmlAttributeNameBuilder(attributeName, attributeVal), node);
			}
			else
				NStringUtils::buildPath(__nodeName, '.', node);
			
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
	
	void TConfigurationXML::keys(std::list<TString>& keyList) const
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
	
	// TConfigurationJSON implementation
	TConfigurationJSON::TConfigurationJSON(const Dixter::TString& file) noexcept
			: m_file(file),
			  m_rootNode(),
			  m_entries(new TNodeEntry),
			  m_propertyTree(new PropertyTree)
	{
		try
		{
			boost::property_tree::json_parser::read_json<PropertyTree>(file, *m_propertyTree);
		} catch (std::exception& e)
		{
			printerr(e.what())
		}
	};
	
	void TConfigurationJSON::load()
	{
		printl_log("")
	}
	
	
	void TConfigurationJSON::save()
	{
		printl_log("")
	}
	
	void TConfigurationJSON::keys(std::list<TString>&) const
	{
		printl_log("")
	}
	
	void TConfigurationJSON::get(const TString&, std::vector<TUString>&) const
	{
	
	}
	
	TUString 
	TConfigurationJSON::get(const TString&) const 
	{
		printl_log("")
	}
	
	TUString 
	TConfigurationJSON::get(const TString&, const TUString&) const
	{
		printl_log("")
	}
	
	
	// ConfigurationProxy implementation
	TConfigurationFactory::TConfigurationFactory(const TString& configPath,
												 EConfiguration type)
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
			case EConfiguration::JSON:
				m_configuration = dxMAKE_SHARED(TConfigurationJSON, configPath);
				break;
			default:
				throw TIllegalArgumentException("%s:%d Unknown type.\n", __FILE__, __LINE__);
		}
	}
	
	void TConfigurationFactory::load()
	{
		m_configuration->load();
	}
	
	void TConfigurationFactory::save()
	{
		m_configuration->save();
	}
	
	inline void TConfigurationFactory::keys(std::list<TString>& keyList) const
	{
		m_configuration->keys(keyList);
	}
	
	TConfigurationFactory::IConfigurationPtr
	TConfigurationFactory::getConfiguration()
	{
		return m_configuration;
	}
	
	EConfiguration
	TConfigurationFactory::getType() const
	{
		return m_type;
	}
	
	// ConfigurationManager::TAccessor implementation
	TConfigurationManager::
	TAccessor::TAccessor(TConfigurationManager* manager) noexcept
			: m_manager(manager)
	{ }
	
	TUString
	TConfigurationManager::
	TAccessor::getValue(const TString& key, const TString& root) const
	{
		return this->get(key, root)->get(key);
	}
	
	TUString
	TConfigurationManager::
	TAccessor::getValue(const TString& key, const TUString& byValue,
						const TString& root) const
	{
		return this->get(key, root)->get(key, byValue);
	}
	
	const TConfigurationManager::TAccessor*
	TConfigurationManager::
	TAccessor::getValues(const TString& key, std::vector<TUString>& values, const TString& root) const
	{
		this->get(key, root)->get(key, values);
		return this;
	}
	
	std::shared_ptr<IConfiguration>
	TConfigurationManager::
	TAccessor::get(const TString& key, const TString& root) const
	{
		auto __iter = m_manager->m_properties.find(root.empty() ? key : root);
		snprintfm(__msg, "Key \"%s\" not found", key.data());
		m_manager->checkKey(__iter, __msg.get());
		
		return __iter->second;
	}
	
	///ConfigurationManager::TMutator implementation
	TConfigurationManager::
	TMutator::TMutator(TConfigurationManager* manager) noexcept
			: m_manager(manager)
	{ }
	
	const TConfigurationManager::TMutator*
	TConfigurationManager::
	TMutator::setValue(const TString& key, const TUString& value, const TString& root)
	{
		scoped_lock<std::mutex> lockGuard(m_mutex);
		this->get(key, root)->set(key, value);
		return this;
	}
	
	std::shared_ptr<IConfiguration>
	TConfigurationManager::
	TMutator::get(const TString& key, const TString& root)
	{
		auto __iter = m_manager->m_properties.find(root.empty() ? key : root);
		snprintfm(__msg, "Key \"%s\" not found", key.data());
		m_manager->checkKey(__iter, __msg.get());
		
		return __iter->second;
	}
	
	///ConfigurationManager implementation
	TConfigurationManager::TInstancePtr&
	TConfigurationManager::getManager(EConfiguration type, std::set<TString> paths)
	{
		m_instance.reset(new TSelf(type, ( paths.empty() ? g_confPath : paths )));
		return m_instance;
	}
	
	const TConfigurationManager::TAccessorPtr&
	TConfigurationManager::accessor() const
	{
		return m_accessor;
	}
	
	const TConfigurationManager::TMutatorPtr&
	TConfigurationManager::mutator()
	{
		return m_mutator;
	}
	
	EConfiguration
	TConfigurationManager::getType() const
	{
		return m_type;
	}
	
	TConfigurationManager::TConfigurationManager(EConfiguration type,
												 const std::set<TString>& paths)
			: m_paths(paths),
			  m_properties(),
			  m_accessor(new TAccessor(this)),
			  m_mutator( new TMutator(this))
	{
		for (const auto& path : paths)
			this->read(type, path);
	}
	
	void TConfigurationManager::read(EConfiguration type, const TString& path)
	{
		auto __factory = dxMAKE_UNIQUE(TConfigurationFactory, path, type);
		
		std::list<TString> __keyList;
		__factory->load();
		m_type = type;
		__factory->keys(__keyList);
		for (auto& __key : __keyList)
			m_properties.emplace(__key, __factory->getConfiguration());
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
	
	TConfigurationManager::TInstancePtr
			TConfigurationManager::TConfigurationManager::m_instance {};
	
	// bool TConfigurationManager::update()
	// {
	// 	bool success(false);
	// 	try
	// 	{
	// 		auto __type = m_factory->getType();
	// 		auto __paths = m_paths;
	// 		m_instance.reset(new TConfigurationManager(__type, __paths));
	// 		success = true;
	// 	} catch (TException& e)
	// 	{
	// 		printerr(e.getMessage())
	// 	}
	// 	if (success)
	// 		println("Updated")
	// 	return success;
	// }
}