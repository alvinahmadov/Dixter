/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
 
#pragma once

#include <map>
#include <list>
#include <mutex>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Types.hpp"
#include "Constants.hpp"
#include "MethodCallback.hpp"
#include "Node.hpp"
#include "Utilities.hpp"
#include "Commons.hpp"

#define DEBUG_CONFIG 0

namespace Dixter
{
	struct Node;
	class NodeData;
	struct ConfigurationInterface;
	
	using PropertyTree          = boost::property_tree::basic_ptree<string_t, ustring_t>;
	using ConfigurationProperty = std::map<string_t, ConfigurationInterface*>;
	
	/**
	 * \namespace Dixter
	 * \brief Trigger for exception catching for NodeEntry class
	 */
	enum class ExceptionCase
	{
		Throw,
		DontThrow
	};
	
	/**
	 * \brief Global configuration data paths for application
	 * */
	const auto g_ConfigPath = std::list<string_t> {
			kLangConfigPath, kVoiceConfigPath, kPanelConfigPath
	};
	
	/**
	 * \brief Enum that defines which configuration reader/writer to use.
	 * */
	enum class ConfigurationType
	{
		ConfigIni,
		ConfigXml,
		ConfigJson,
		ConfigNone
	};
	
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 * \struct ConfigurationInterface
	 * \brief Interface for Configuration readers/writers.
	 * */
	struct ConfigurationInterface
	{
		/**
		 * \interface ConfigurationInterface
		 * \brief Abstract method for loading data
		 * from configuration file.
		 * */
		virtual void load() = 0;
		
		/**
		 * \interface ConfigurationInterface
		 * \brief Abstract method for writing data
		 * to configuration file.
		 * */
		virtual void save() = 0;
	};
	
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 * \brief Iterface class to store data configuration data.
	 *
	 * Stores data read from configuration reader/writer in an
	 * indexed map, to simplify data access.
	 * */
	class NodeEntry
	{
	public:
		using Entry = std::map<i32, std::shared_ptr<NodeData>>;
	public:
		/**
		 * \class NodeEntry
		 * \brief ctor.
		 * \param throw_ If data to return is null or empty,
		 * class methods will throw exceptions.
		 * */
		explicit NodeEntry(ExceptionCase throw_ = ExceptionCase::Throw);
		
		~NodeEntry();
		
		/**
		 * \class NodeEntry
		 * \brief Inserts data entry.
		 * \param nodeEntry  Entry to insert.
		 * \returns true if data was inserted, false otherwise.
		 * */
		bool insertEntry(NodeData* nodeEntry);
		
		bool setEntry(const string_t& key, const ustring_t& value);
		
		bool setEntry(size_t index, const ustring_t& value);
		
		/**
		 * \class NodeEntry
		 * \brief Removes data entry.
		 * \returns true if data was removed, false otherwise.
		 * */
		bool removeEntry();
		
		/**
		 * \class NodeEntry
		 * \brief Find matching entry by node name.
		 * \param key Node name as a key to find corresponding node.
		 * \returns Node.
		 *
		 * Depending on \c m_throw value throws exception if node
		 * haven't found else returns null.
		 * */
		std::shared_ptr<const Node>
		findEntry(const string_t& key) const;
		
		/**
		 * \class NodeEntry
		 * \brief Find matching entry by node value.
		 * \param value Node value as a key to find corresponding
		 * node.
		 * \returns Node.
		 *
		 * Depending on \c m_throw value throws exception if node
		 * haven't found else returns null.
		 * */
		std::shared_ptr<const Node>
		findEntry(const ustring_t& value) const;
		
		/**
		 * \class NodeEntry
		 * \brief Find node data by index.
		 * \param index Index of data.
		 * \returns Node Data.
		 *
		 * Depending on \c m_throw value throws exception if
		 * node data haven't found else returns null.
		 * */
		std::shared_ptr<const NodeData>
		findEntryData(i32 index) const;
		
		i32 findEntryIndex(const string_t& key) const;
		
		/**
		 * \class NodeEntry
		 * \brief Get number of data stored in NodeEntry.
		 * \returns Size of stored data.
		 * */
		size_t getSize() const;
		
		/**
		 * \class NodeEntry
		 * \brief Calls NodeData methods on stored data
		 * with arguments.
		 * \tparam R Return type of NodeData method.
		 * \tparam Args Argument types for NodeData method.
		 * \param method NodeData method to call.
		 * \param args Arguments of NodeData method.
		 * */
		template<typename R, typename ... Args>
		void forEach(R(NodeData::*method)(Args...), Args ... args);
		
		/**
		 * \class NodeEntry
		 * \brief Get all entries.
		 * \returns Entries.
		 * */
		const Entry* get() const;
		
		Entry* operator()();
		
		const NodeData* operator[](i32 index) const;
	
	private:
		bool checkKey(const string_t& key) const;
		
		bool checkIndex(size_t index) const;
	
	private:
		i32 m_index;
		ExceptionCase m_throw;
		Entry* m_nodeEntries;
		mutable std::mutex m_mutex;
	};
	
	
	class ConfigurationManager;
	
	/**
	 * @brief Class for reading and storing settings from ini file
	 * */
	class INIConfiguration : public ConfigurationInterface
	{
		friend class ConfigurationManager;
	
	public:
		explicit INIConfiguration(const string_t& file);
		
		~INIConfiguration();
		
		void load() dxDECL_OVERRIDE;
		
		void save() dxDECL_OVERRIDE;
		
		void getKey(std::list<string_t>&) const;
		
		ustring_t getValue(const string_t& key) const;
	
	private:
		std::list<std::shared_ptr<NodeData>> m_configurationList;
		std::unique_ptr<PropertyTree> m_propertyTree;
	};
	
	/**
	 * \class XMLConfiguration
	 * \implements ConfigurationInterface
	 * \brief Class for reading and storing settings from xml file
	 * */
	class XMLConfiguration : public ConfigurationInterface
	{
		friend class ConfigurationManager;
	
	public:
		/**
		 * \class XMLConfiguration
		 * \brief ctor.
		 * \param file Absolute path to XML configuration xml file.
		 * */
		explicit XMLConfiguration(const string_t& file);
		
		/**
		 * \class XMLConfiguration
		 * \brief dtor.
		 * */
		~XMLConfiguration();
		
		/**
		 * \class XMLConfiguration
		 * \brief Loads data from configuration file to memory.
		 * */
		void load() dxDECL_OVERRIDE;
		
		/**
		 * \class XMLConfiguration
		 * \brief Saves data to configuration file.
		 * \note Not implemented for now.
		 * \throws NotImplementedException
		 * */
		void save() dxDECL_OVERRIDE;
		
		void getKey(std::list<string_t>&) const;
		
		/*
		UStringVector GetAttributes(const ustring_t &attributeName) const;
		
		ustring_t GetAttributeValue(const ustring_t &nodeValue, const ustring_t &attributeName) const;
		
		ustring_t GetAttributeValueBySibling(const ustring_t &siblingValue, const ustring_t &nodeName);
		*/
	private:
		string_t m_rootNode;
		string_t m_file;
		NodeEntry* m_entries;
		PropertyTree* m_propertyTree;
		mutable std::mutex m_mutex;
	};
	
	class JSONConfiguration : public ConfigurationInterface
	{
		JSONConfiguration(const string_t& config_file) = delete;
		
		virtual ~JSONConfiguration() = 0;
		
		virtual void load() = 0;
		
		virtual void save() = 0;
		
		virtual void getKey(std::list<string_t>&) const = 0;
		
		virtual const ustring_t& getValue(const string_t& nodeName) const = 0;
		
		virtual const ustring_t& getValue(const ustring_t& siblingValue, const string_t& nodeName) const = 0;
		
		static size_t reference_count;
	private:
		string_t m_rootNode;
		std::map<i32, std::shared_ptr<NodeData>>* m_pConfigMap;
		std::unique_ptr<PropertyTree> m_propertyTree;
	};
	
	class ConfigurationFactory : public ConfigurationInterface, public DefaultNonCopyable
	{
	public:
		ConfigurationFactory(const string_t& configPath, ConfigurationType type);
		
		~ConfigurationFactory();
		
		void load() dxDECL_OVERRIDE;
		
		void save() dxDECL_OVERRIDE;
		
		void getKey(std::list<string_t>& keyList) const;
		
		ConfigurationInterface* getConfiguration();
		
		const ConfigurationType& getType() const;
	
	private:
		ConfigurationType m_type;
		ConfigurationInterface* m_configuration;
	};
	
	struct ConfigurationManagerInterface
	{
		using key_const_iterator = typename std::map<string_t, ConfigurationInterface*>::const_iterator;
		
		virtual void read(ConfigurationType type, const string_t& path) = 0;
		
		virtual void write(ConfigurationType type, const string_t& path) = 0;
		
		inline void checkKey(const key_const_iterator key, ConfigurationProperty* properties,
		                     string_t errorMsg) const;
	};
	
	/**
	 * \author Alvin Ahmadov
	 * \class ConfigurationManager
	 * \implements ConfigurationManagerInterface
	 * \namespace Dixter
	 * \brief Singleton class that manages configurations
	 * */
	class ConfigurationManager final : public ConfigurationManagerInterface
	{
	private:
		/**
		 * \author Alvin Ahmadov
		 * \class ConfigurationManager::Accessor
		 * \brief Class used as inner helper class to read data of ConfigurationManager.
		 *
		 * Provides easy access interfaces to data. Data is immutable.
		 * */
		class Accessor
		{
		public:
			Accessor(ConfigurationManager* manager);
			
			/**
		 * \class ConfigurationManager
		 * \brief Get value of node using key.
		 * \param key Node name of value.
		 * \returns Found value.
		 * \throws NotFoundException.
		 * */
			ustring_t getValue(const string_t& root, const string_t& key) const;
			
			/**
			 * \class XMLConfiguration
			 * \brief Get value of node with key and with sibling value.
			 * \param value Sibling value.
			 * \param key Node name of sibling value.
			 * \returns value
			 * \throws NotFoundException
			 * */
			ustring_t getValue(const string_t& root, const ustring_t& value, const string_t& key) const;
			
			/**
			 * \brief Get all values of node with specified name.
			 * \tparam Container Container to which save data.
			 * */
			void getValues(const string_t& root, const string_t& key, std::vector<ustring_t>& values) const;
		
		private:
			ConfigurationManager* m_manager;
		};
		
		/**
		 * \brief Helper used to change configuration value/values
		 * */
		
		/**
		 * \author Alvin Ahmadov
		 * \class ConfigurationManager::Mutator
		 * \brief Class used as inner helper class to write data of ConfigurationManager.
		 *
		 * Provides easy access interfaces to data. Data is mutable.
		 * */
		class Mutator
		{
		public:
			Mutator(ConfigurationManager* manager);
			
			/**
		 * \class ConfigurationManager
		 * \brief Get value of node using key.
		 * \param key Node name of value.
		 * \returns Found value.
		 * \throws NotFoundException.
		 * */
			void setValue(const string_t& root, const string_t& key, const ustring_t& value) const;
		
		private:
			ConfigurationManager* m_manager;
			mutable std::mutex m_mutex;
		};
	
	public:
		/**
		 * \class ConfigurationManager
		 * \brief Get instance of manager.
		 * \param type Configuration type.
		 * \param paths List of paths to configuration files.
		 * \returns Manager instance.
		 *
		 * If there is an initialised instance and its type is different,
		 * then the first instance will be added to a set, and new instance
		 * is initialised and used. Depending on type it will return the correct instance.
		 * */
		static ConfigurationManager* getManager(ConfigurationType type,
		                                        const std::list<string_t>& paths = g_ConfigPath);
		
		const Accessor* const getAccessor() const;
		
		Mutator* getMutator();
		
		/**
		 * \class ConfigurationManager
		 * \brief Gets configuration object and casts it to
		 * the derived Configuration class (e.g. \c XMLConfiguration, \c INIConfiguration)
		 * \tparam T Type of configuration derived from \c ConfigurationInterface.
		 * \param key Root name of configuration to find.
		 * \param type Cast type.
		 * \returns Casted type pointer.
		 * */
		template<class T>
		inline const T* getConfiguration(const string_t& key,
		                                 ConfigurationType type = ConfigurationType::ConfigNone)
		{
			try
			{
				if (type != ConfigurationType::ConfigNone)
				{
					if (m_instance->getType() != type)
					{
						for (auto& __instance : *m_instances)
						{
							if (__instance->getType() == type)
							{
								return getConfiguration<T>(key, ConfigurationType::ConfigNone);
							}
						}
					}
				} else
				{
					auto iter = m_properties->find(key);
					
					if (iter != m_properties->end())
					{
						return dynamic_cast<T*>(iter->second);
					}
					return nullptr;
				}
			} catch (std::exception& e)
			{
				printerr(e.what())
			}
			return nullptr;
		}
		
		/**
		 * \class ConfigurationManager
		 * \brief Get value of node using key.
		 * \param key Node name of value.
		 * \returns Found value.
		 * \throws NotFoundException.
		 * */
//		const ustring_t& getValue(const string_t& root, const string_t& key) const;
		
		/**
		 * \class ConfigurationManager
		 * \brief Gets the type of current instance.
		 * \returns Instance's type.
		 * */
		const ConfigurationType& getType() const;
		
		/**
		 * \class ConfigurationManager
		 * \brief Updates loaded configuration values.
		 * \returns True if values updated.
		 * */
		bool update();
		
		/**
		 * \class ConfigurationManager
		 * \brief Releases instance to update application's
		 * configuration. Used after \c getManager().
		 * \returns True if instance is deleted.
		 * */
		static bool release();
	
	private:
		/// dtor.
		virtual ~ConfigurationManager();
		
		void read(ConfigurationType type, const string_t& path) dxDECL_OVERRIDE;
		
		void write(ConfigurationType type, const string_t& path) dxDECL_OVERRIDE;
		
		void checkKey(const std::map<string_t, ConfigurationInterface*>::const_iterator key,
		              string_t errorMsg) const;
		
		/**
		 * \class ConfigurationManager
		 * \brief ctor. Loads configuration for every configuration file.
		 * \param type Type of configuration to initialise.
		 * \param paths List of paths to configuration files.
		 * */
		ConfigurationManager(ConfigurationType type, const std::list<string_t>& paths);
	
	private:
		std::list<string_t> m_pathList;
		ConfigurationFactory* m_factory;
		std::map<string_t, ConfigurationInterface*>* m_properties;
		Accessor* m_accessor;
		Mutator* m_mutator;
		/// Instance of manager
		static ConfigurationManager* m_instance;
		/// Set of instances with different types.
		static std::set<ConfigurationManager*>* m_instances;
	};
	
	template<typename return_t, typename ... args_t>
	void NodeEntry::forEach(return_t(NodeData::*method)(args_t ... args), args_t ... args)
	{
		auto __methodCallback = MethodCallback<NodeData, return_t, args_t ...>(method);
		std::for_each(m_nodeEntries->begin(), m_nodeEntries->end(),
		              [ &__methodCallback, &args ... ](std::pair<i32, std::shared_ptr<NodeData>>& pair)
		              {
			              if (pair.second != nullptr)
				              __methodCallback(pair.second, args ...);
		              });
	}
} // namespace Dixter
