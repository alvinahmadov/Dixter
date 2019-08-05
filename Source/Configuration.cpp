/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <unicode/ustring.h>

#include "Configuration.hpp"
#include "Utilities.hpp"
#include "Commons.hpp"
#include "Node.hpp"

using namespace std;

namespace Dixter
{
	using namespace Utilities::Strings;
	
	// NodeEntry implementation
	NodeEntry::NodeEntry(ExceptionCase throws)
			: m_index { },
			  m_throw { throws },
			  m_nodeEntries { new Entry }
	{ }
	
	NodeEntry::~NodeEntry()
	{
		SAFE_RELEASE(m_nodeEntries)
	}
	
	bool NodeEntry::insertEntry(NodeData* nodeEntry)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		if (m_nodeEntries->find(m_index) == m_nodeEntries->end())
		{
			m_nodeEntries->insert({ m_index++, std::shared_ptr<NodeData>(nodeEntry) });
			return true;
		}
		return false;
	}
	
	bool NodeEntry::setEntry(const string_t& key, const ustring_t& value)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		bool __found { checkKey(key) };
		if (__found)
		{
			for (auto& __valuePair : *m_nodeEntries)
			{
				for (auto& __node : __valuePair.second->getNodes())
					if (__node->name.compare(key) == 0)
					{
						__node->value = value;
					}
			}
		} else
		{
			if (m_throw == ExceptionCase::Throw)
			{
				throw NotFoundException { "%s:%d Node data for name %s not found.", __FILE__, __LINE__, key};
			}
		}
		
		return __found;
	}
	
	bool NodeEntry::setEntry(size_t index, const ustring_t& value)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		if (checkIndex(index))
		{
			auto iter = m_nodeEntries->find(index);
			auto __nodeName = findEntry(value)->name;
			
			std::replace_if(
					iter->second->getNodes().begin(), iter->second->getNodes().end(),
					[ &__nodeName ](shared_ptr<const Node> node)
					{ return node->name.compare(__nodeName) == 0; },
					std::make_shared<Node>(__nodeName, value)
			);
			return true;
		}
		return false;
	}
	
	bool NodeEntry::removeEntry()
	{
		return false;
	}
	
	std::shared_ptr<const Node>
	NodeEntry::findEntry(const string_t& key) const
	{
		std::lock_guard<std::mutex> l(m_mutex);
		std::shared_ptr<Node> __data;
		for (const auto& __valuePair : *m_nodeEntries)
		{
			for (shared_ptr<Node>& __node : __valuePair.second->getNodes())
			{
				if (__node->name.compare(key) == 0)
				{
					__data = __node;
				}
			}
		}
		if (m_throw == ExceptionCase::Throw)
		{
			if (!__data)
			{
				throw NotFoundException { "%s:%d Node data for %s not found.", __FILE__, __LINE__, key};
			}
		}
		return __data;
	}
	
	std::shared_ptr<const Node>
	NodeEntry::findEntry(const ustring_t& value) const
	{
		std::lock_guard<std::mutex> l(m_mutex);
		std::shared_ptr<Node> __data;
		for (const auto& __valuePair : *m_nodeEntries)
		{
			for (shared_ptr<Node>& __node : __valuePair.second->getNodes())
				if (__node->value.compare(value) == 0)
				{
					__data = __node;
				}
		}
		if (m_throw == ExceptionCase::Throw)
		{
			if (__data == nullptr)
			throw NotFoundException { "%s:%d Node data for %s not found.", __FILE__, __LINE__, value.asUTF8()};
		}
		return __data;
	}
	
	std::shared_ptr<const NodeData>
	NodeEntry::findEntryData(i32 index) const
	{
		std::lock_guard<std::mutex> l(m_mutex);
		std::shared_ptr<NodeData> __data;
		if (index != -1)
		{
			if (checkIndex(static_cast<size_t>(index)))
			{
				for (const auto& __p : *m_nodeEntries)
				{
					if (__p.first == index)
					{
						__data = __p.second;
					}
				}
			}
		}
		
		if (m_throw == ExceptionCase::Throw)
		{
			if (__data == nullptr)
			{
				throw NotFoundException { "%s:%d Node data for index %d not found.", __FILE__, __LINE__, index};
			}
		}
		return __data;
	}
	
	i32 NodeEntry::findEntryIndex(const string_t& key) const
	{
		i32 __index = -1;
		for (const auto& __p : *m_nodeEntries)
			for (const auto& __node : __p.second->getNodes())
				if (__node->name.compare(key) == 0)
				{
					__index = __p.first;
				}
		
		return __index;
	}
	
	size_t NodeEntry::getSize() const
	{
		return m_nodeEntries->size();
	}
	
	const NodeEntry::Entry*
	NodeEntry::get() const
	{
		return m_nodeEntries;
	}
	
	NodeEntry::Entry*
	NodeEntry::operator()()
	{
		return m_nodeEntries;
	}
	
	bool NodeEntry::checkKey(const string_t& key) const
	{
		bool __found { };
		if (!key.empty())
		{
			try
			{
				if (findEntry(key) != nullptr)
				{
					__found = true;
				}
			} catch (std::exception& e)
			{
				__found = false;
				printerr(e.what())
			}
		} else
		{
			__found = false;
		}
		
		return __found;
	}
	
	bool NodeEntry::checkIndex(size_t index) const
	{
		if (index >= std::numeric_limits<size_t>::min() && index <= std::numeric_limits<size_t>::max())
		{
			if (index <= m_nodeEntries->size())
			{
				if (m_nodeEntries->find(index) != m_nodeEntries->end())
				{
					return true;
				}
			}
		}
		return false;
	}
	
	// INIConfiguration implementation
	INIConfiguration::INIConfiguration(const string_t& file)
			: m_configurationList { list<shared_ptr<NodeData>>() },
			  m_propertyTree(unique_ptr<PropertyTree>(new PropertyTree()))
	{
		boost::property_tree::ini_parser::read_ini<PropertyTree>(file, *m_propertyTree);
	}
	
	INIConfiguration::~INIConfiguration()
	{
		m_propertyTree.reset(nullptr);
		SAFE_RESET_LIST(m_configurationList);
	}
	
	void INIConfiguration::load()
	{
		PropertyTree __localTree = *m_propertyTree;
		auto __configData = std::shared_ptr<NodeData>(new NodeData());
		for (const auto& __value : __localTree)
		{
			for (const auto& __node : __value.second)
			{
				__configData->insertData(__node.first, __node.second.get_value<ustring_t>());
			}
		}
		m_configurationList.push_back(__configData);
	}
	
	void INIConfiguration::save()
	{
		printl("Not implemented yet")
	}
	
	void INIConfiguration::getKey(list<string_t>& keyList) const
	{
		for (auto& __treeKey : *m_propertyTree)
			keyList.push_back(__treeKey.first);
	}
	
	ustring_t
	INIConfiguration::getValue(const string_t& key) const
	{
		return m_propertyTree->find(key)->second.data();
	}
	
	// XMLConfiguration implementation
	XMLConfiguration::XMLConfiguration(const string_t& file)
			: m_file { file },
			  m_entries { new NodeEntry() },
			  m_propertyTree(new PropertyTree())
	{
		try
		{
			boost::property_tree::xml_parser::read_xml<PropertyTree>(file, *m_propertyTree);
		} catch (Exception& e)
		{
			printerr(e.getMessage())
		}
	}
	
	XMLConfiguration::~XMLConfiguration()
	{
		SAFE_RELEASE(m_propertyTree)
	}
	
	void XMLConfiguration::load()
	{
		lock_guard<mutex> loadLock(m_mutex);
		
		static string_t __parentBefore { };
		string_t __parent { };
		string_t __nodeName { };
		const string_t __attributeKey { "<xmlattr>" };
		m_rootNode = m_propertyTree->front().first;
		const auto* __rootNodeName = &m_propertyTree->front().first;
		NodeData* __configData { nullptr };
		const auto& __childTree = m_propertyTree->front().second;
		
		auto __createNodeName = [ &__nodeName, &__parent, &__rootNodeName ]
				(const string_t& node, const ustring_t& attributeName = ustring_t(),
				 const ustring_t& attributeVal = ustring_t(), bool child = false)
		{
			static string_t __rootNameWithAttr { };
			if (!attributeVal.empty() && !attributeName.empty())
			{
				if (!__rootNameWithAttr.empty()) __rootNameWithAttr.clear();
				__rootNameWithAttr.push_back('<');
				__rootNameWithAttr.append(attributeVal.asUTF8()).push_back('>');
			} else
			{
				if (child)
				{
					Utilities::Strings::buildPath(__nodeName, '.', node);
				} else
				{
					__nodeName.clear();
					Utilities::Strings::buildPath(__nodeName, '.', *__rootNodeName, __parent);
					Utilities::Strings::buildPath(__nodeName, '.', __rootNameWithAttr, node);
				}
			}
		};
		function<
				void(const PropertyTree&, const string_t&)> internalLoad =
				[ & ](const PropertyTree& prop, const string_t& parent)
				{
					__configData = new NodeData;
					for (const PropertyTree::value_type& node : prop)
					{
						auto __nodeValue = node.second.get_value<ustring_t>();
						// Not attribute
						if (node.first != __attributeKey)
						{
							if (node.second.size() > 0)
							{
								__createNodeName(node.first);
								for (const PropertyTree::value_type& __withAttr : node.second)
								{
									auto __nodeNameTemp = __nodeName;
									__createNodeName(__withAttr.first, ustring_t(), ustring_t(), true);
									__configData->insertData(__nodeName, __withAttr.second.get_value<ustring_t>(), parent);
									__nodeName = __nodeNameTemp;
								}
							} else
							{
								__createNodeName(node.first);
								__configData->insertData(__nodeName, __nodeValue, parent);
							}
						} else
						{
							if (node.second.size() > 1)
							{
								internalLoad(node.second, parent);
							} else
							{
								
								__createNodeName(node.second.front().first,
								                 node.second.front().first,
								                 node.second.front().second.get_value<ustring_t>());
								if (!__nodeValue.empty())
								{
									__configData->insertData(__nodeName, __nodeValue, parent, node.second.front().first,
									                         node.second.front().second.get_value<ustring_t>());
								}
							}
						}
					}
					m_entries->insertEntry(__configData);
				};
		
		{
			for (const auto& value : __childTree)
			{
				__parent.clear();
				__parent = value.first;
				if (__parent != __parentBefore)
				{
					__parentBefore = __parent;
				}
				
				internalLoad(value.second, __parent);
			}
		}
	}
	
	void XMLConfiguration::save()
	{
		throw NotImplementedException("%s:d% Saving xml configuration not implemented yet", __FILE__, __LINE__);
	}
	
	void XMLConfiguration::getKey(list<string_t>& keyList) const
	{
		keyList.push_back(m_rootNode);
	}
	
	// ConfigurationProxy implementation
	ConfigurationFactory::ConfigurationFactory(const string_t& configPath, ConfigurationType type)
			: m_type { type }
	{
		switch (type)
		{
			case ConfigurationType::ConfigIni: m_configuration = new INIConfiguration(configPath);
				break;
			case ConfigurationType::ConfigXml: m_configuration = new XMLConfiguration(configPath);
				break;
			case ConfigurationType::ConfigJson : throw NotImplementedException { "%s:%d Reading configuration from json file is not implemented yet.\n", __FILE__, __LINE__ };
			default: throw IllegalArgumentException { "%s:%d Unknown type.\n", __FILE__, __LINE__};
		}
	}
	
	ConfigurationFactory::~ConfigurationFactory()
	{
	}
	
	void ConfigurationFactory::load()
	{
		if (m_configuration == nullptr)
		{
			throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
		}
		switch (m_type)
		{
			case ConfigurationType::ConfigIni: dynamic_cast<INIConfiguration*>(m_configuration)->load();
				break;
			case ConfigurationType::ConfigXml: dynamic_cast<XMLConfiguration*>(m_configuration)->load();
				break;
			case ConfigurationType::ConfigJson: throw NotImplementedException { "%s:%d Reading configuration from json file is not implemented yet.\n", __FILE__, __LINE__ };
			default: throw NotImplementedException("%s:%d Configuration is not initialised", __FILE__, __LINE__);
		}
	}
	
	void ConfigurationFactory::save()
	{
		if (m_configuration == nullptr)
		{
			throw IllegalArgumentException("%s:%d Configuration is not initialised.");
		}
		switch (m_type)
		{
			case ConfigurationType::ConfigIni: dynamic_cast<INIConfiguration*>(m_configuration)->save();
				break;
			case ConfigurationType::ConfigXml: dynamic_cast<XMLConfiguration*>(m_configuration)->save();
				break;
			case ConfigurationType::ConfigJson: throw NotImplementedException { "%s:%d Reading configuration from json file is not implemented yet.\n", __FILE__, __LINE__ };
			default: throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
		}
	}
	
	void ConfigurationFactory::getKey(list<string_t>& keyList) const
	{
		switch (m_type)
		{
			case ConfigurationType::ConfigIni: dynamic_cast<INIConfiguration*>(m_configuration)->getKey(keyList);
				break;
			case ConfigurationType::ConfigXml: dynamic_cast<XMLConfiguration*>(m_configuration)->getKey(keyList);
				break;
			case ConfigurationType::ConfigJson: throw NotImplementedException { "%s:%d Reading configuration from json file is not implemented yet.\n", __FILE__, __LINE__ };
			default: throw IllegalArgumentException { "%s:%d Configuration is not initialised.", __FILE__, __LINE__ };
		}
	}
	
	void ConfigurationManagerInterface::checkKey(const ConfigurationProperty::const_iterator key,
	                                             ConfigurationProperty* properties, string_t errorMsg) const
	{
		if (properties != nullptr && key == properties->end())
		{
			throw NotFoundException { errorMsg };
		}
	}
	
	ConfigurationInterface* ConfigurationFactory::getConfiguration()
	{
		switch (m_type)
		{
			case ConfigurationType::ConfigXml : return dynamic_cast<XMLConfiguration*>(m_configuration);
			case ConfigurationType::ConfigIni : return dynamic_cast<INIConfiguration*>(m_configuration);
			case ConfigurationType::ConfigJson: throw NotImplementedException { "%s:%d Reading configuration from json file is not implemented yet.\n", __FILE__, __LINE__ };
			default: throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
		}
	}
	
	const ConfigurationType&
	ConfigurationFactory::getType() const
	{
		return m_type;
	}
	
	///ConfigurationManager implementation
	ConfigurationManager*
	ConfigurationManager::getManager(ConfigurationType type, const list<string_t>& configPaths)
	{
		if (m_instance == nullptr)
		{
			m_instance = new ConfigurationManager(type, configPaths);
		} else if (m_instance->m_factory->getType() != type)
		{
			m_instances->insert(m_instance);
			m_instance = new ConfigurationManager(type, configPaths);
		}
		return m_instance;
	}
	
	const ConfigurationManager::Accessor* const
	ConfigurationManager::getAccessor() const
	{
		return m_accessor;
	}
	
	ConfigurationManager::Mutator*
	ConfigurationManager::getMutator()
	{
		return m_mutator;
	}
	
	void ConfigurationManager::read(ConfigurationType type, const string_t& path)
	{
		m_factory = new ConfigurationFactory(path, type);
		list<string_t> __keyList;
		m_factory->load();
		m_factory->getKey(__keyList);
		for (auto& val : __keyList)
			m_properties->emplace(val, m_factory->getConfiguration());
	}
	
	void ConfigurationManager::write(ConfigurationType type, const string_t& path)
	{
		printl_log(path)
		
		switch (type)
		{
			case ConfigurationType::ConfigXml : printl_log("XML write")
				break;
			case ConfigurationType::ConfigJson : printl_log("JSON write")
				break;
			case ConfigurationType::ConfigIni : printl_log("INI write")
				break;
			default: printl_log("None")
				break;
		}
	}
	
	ConfigurationManager::~ConfigurationManager()
	{
		SAFE_RELEASE(m_factory)
		SAFE_RELEASE_MAP((*m_properties))
		if (m_instances->size() > 0)
		{
			for (ConfigurationManager* v : *m_instances)
				SAFE_RELEASE(v)
		};
		SAFE_RELEASE(m_instances)
	}
	
	void ConfigurationManager::checkKey(const ConfigurationProperty::const_iterator key,
	                                    string_t errorMsg) const
	{
		ConfigurationManagerInterface::checkKey(key, m_properties, errorMsg);
	}
	
	ConfigurationManager::ConfigurationManager(ConfigurationType type, const list<string_t>& paths)
			: m_pathList { paths },
			  m_properties { new ConfigurationProperty() },
			  m_accessor { new Accessor(this) }
	{
		for (const auto& path : paths)
			read(type, path);
	}
	
	ConfigurationManager::Accessor::Accessor(ConfigurationManager* manager)
			: m_manager { manager }
	{ }
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreturn-type"
	
	ustring_t
	ConfigurationManager::Accessor::getValue(const string_t& root, const string_t& key) const
	{
		auto iter = m_manager->m_properties->find(root);
		
		if (iter != m_manager->m_properties->end())
		{
			switch (m_instance->m_factory->getType())
			{
				case ConfigurationType::ConfigXml :
				{
					return dynamic_cast<XMLConfiguration*>(iter->second)
							->m_entries->findEntry(key)->value;
				}
				case ConfigurationType::ConfigIni :
				{
					return dynamic_cast<INIConfiguration*>(iter->second)
							->m_propertyTree->find(key)->second.data();
				}
				case ConfigurationType::ConfigJson : break;
				default: throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
			}
		}
	}
	
	ustring_t
	ConfigurationManager::Accessor::getValue(const string_t& root, const ustring_t& value, const string_t& key) const
	{
		auto iter = m_manager->m_properties->find(root);
		
		if (iter != m_manager->m_properties->end())
		{
			switch (m_instance->m_factory->getType())
			{
				case ConfigurationType::ConfigXml :
				{
					auto __entries { dynamic_cast<XMLConfiguration*>(iter->second)->m_entries };
					
					for (const auto& __confData : *__entries->get())
					{
						for (const auto& __node : __confData.second->getNodes())
						{
							if (__node->value.compare(value) == 0)
							{
								return __entries->findEntryData(__confData.first)->getValue(key);
							}
						}
					}
					break;
				}
				case ConfigurationType::ConfigIni :
				{
					return dynamic_cast<INIConfiguration*>(iter->second)
							->m_propertyTree->find(key)->second.data();
					break;
				}
				case ConfigurationType::ConfigJson : break;
				default: throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
					break;
			}
		}
	}
	
	#pragma GCC diagnostic pop
	
	void ConfigurationManager::Accessor::getValues(const string_t& root, const string_t& key, vector<ustring_t>& values) const
	{
		auto iter = m_manager->m_properties->find(root);
		
		if (iter != m_manager->m_properties->end())
		{
			switch (m_instance->m_factory->getType())
			{
				case ConfigurationType::ConfigXml :
				{
					auto __entries { dynamic_cast<XMLConfiguration*>(iter->second)->m_entries };
					for (const auto& __data : *__entries->get())
						__data.second->getValues(key, values);
					break;
				}
				case ConfigurationType::ConfigIni : break;
				case ConfigurationType::ConfigJson : break;
				default: throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
			}
		}
	}
	
	ConfigurationManager::Mutator::Mutator(ConfigurationManager* manager)
			: m_manager { manager }
	{ }
	
	void ConfigurationManager::Mutator::setValue(const string_t& root, const string_t& key, const ustring_t& value) const
	{
		scoped_lock<mutex> lockGuard(m_mutex);
		auto iter = m_manager->m_properties->find(root);
		
		if (iter != m_manager->m_properties->end())
		{
			switch (m_instance->m_factory->getType())
			{
				case ConfigurationType::ConfigXml :
				{
					dynamic_cast<XMLConfiguration*>(iter->second)
							->m_entries->setEntry(key, value);
				}
				case ConfigurationType::ConfigIni : break;
				case ConfigurationType::ConfigJson : break;
				default: throw IllegalArgumentException("%s:%d Configuration is not initialised.", __FILE__, __LINE__);
			}
		}
	}
	
	ConfigurationManager*
			ConfigurationManager::ConfigurationManager::m_instance = nullptr;
	
	set<ConfigurationManager*>*
			ConfigurationManager::ConfigurationManager::m_instances = new set<ConfigurationManager*>();
	
	bool ConfigurationManager::update()
	{
		bool success = false;
		try
		{
			auto __type = m_factory->getType();
			auto __paths = m_pathList;
			SAFE_RELEASE(m_instance)
			m_instance = new ConfigurationManager(__type, __paths);
			success = true;
		} catch (Exception& e)
		{
			printerr(e.getMessage())
		}
		if (success)
			printl("Updated")
		return success;
	}
	
	const ConfigurationType&
	ConfigurationManager::getType() const
	{
		return m_factory->getType();
	}
	
	bool ConfigurationManager::release()
	{
		SAFE_RELEASE(m_instance)
		return m_instance == nullptr;
	}
}