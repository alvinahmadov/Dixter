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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "NodeEntry.hpp"
#include "Commons.hpp"


namespace Dixter
{
	struct TNode;
	class TNodeData;
	struct IConfiguration;
	
	using PropertyTree          = boost::property_tree::basic_ptree<TString, TUString>;
	using PropertyTreePtr       = std::unique_ptr<PropertyTree>;
	using ConfigurationProperty = std::unordered_map<TString, std::shared_ptr<IConfiguration>>;
	
	/**
	 * \brief Enum that defines which configuration reader/writer to use.
	 * */
	enum class EConfiguration
	{
		None,
		INI,
		XML,
		JSON
	};
	
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 * \struct ConfigurationInterface
	 * \brief Interface for Configuration readers/writers.
	 * */
	struct IConfiguration
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
		
		virtual void set(const TString& key, const TUString& value) = 0;
		
		virtual void keys(std::list<TString>&) const = 0;
		
		virtual void get(const TString& key, std::vector<TUString>& values) const = 0;
		
		virtual TUString get(const TString& key, const TUString& byValue) const = 0;
		
		virtual TUString get(const TString& key) const = 0;
		
		virtual ~IConfiguration() = default;
		
	};
	
	class TConfigurationManager;
	
	/**
	 * @brief Class for reading and storing settings from ini file
	 * */
	class TConfigurationINI final : public IConfiguration, public NonCopyable
	{
	public:
		explicit TConfigurationINI(const TString& file) noexcept;
		
		virtual ~TConfigurationINI() noexcept override = default;
		
		void load() override;
		
		void save() override;
		
		void set(const TString& key, const TUString& value) override;
		
		void keys(std::list<TString>&) const override;
		
		TUString get(const TString& key) const override;
		
		void get(const TString& key, std::vector<TUString>& values) const override;
		
		TUString get(const TString& key, const TUString& byValue) const override;
	
	private:
		TString m_file;
		std::unique_ptr<PropertyTree> m_propertyTree;
		NodeEntryPtr m_entries;
	};
	
	/**
	 * \class XMLConfiguration
	 * \implements ConfigurationInterface
	 * \brief Class for reading and storing settings from xml file
	 * */
	class TConfigurationXML final : public IConfiguration, public NonCopyable
	{
	public:
		/**
		 * \class XMLConfiguration
		 * \brief ctor.
		 * \param file Absolute path to XML configuration xml file.
		 * */
		explicit TConfigurationXML(const TString& file) noexcept;
		
		/**
		 * \class XMLConfiguration
		 * \brief dtor.
		 * */
		virtual ~TConfigurationXML() noexcept override = default;
		
		/**
		 * \class XMLConfiguration
		 * \brief Loads data from configuration file to memory.
		 * */
		void load() override;
		
		/**
		 * \class XMLConfiguration
		 * \brief Saves data to configuration file.
		 * \note Not implemented for now.
		 * \throws NotImplementedException
		 * */
		void save() override;
		
		void set(const TString& key, const TUString& value) override;
		
		void keys(std::list<TString>&) const override;
		
		void get(const TString& key, std::vector<TUString>& values) const override;
		
		TUString get(const TString& key) const override;
		
		TUString get(const TString& key, const TUString& byValue) const override;
		
		/*
		UStringVector GetAttributes(const TUString &attributeName) const;
		
		TUString GetAttributeValue(const TUString &nodeValue, const TUString &attributeName) const;
		
		TUString GetAttributeValueBySibling(const TUString &siblingValue, const TUString &nodeName);
		*/
	private:
		TString m_rootNode;
		TString m_file;
		NodeEntryPtr m_entries;
		std::unique_ptr<PropertyTree> m_propertyTree;
		mutable std::mutex m_mutex;
	};
	
	class TConfigurationJSON : public IConfiguration
	{
		explicit TConfigurationJSON(dxMAYBE_UNUSED const TString& config_file = "") {};
		
		virtual ~TConfigurationJSON() = 0;
		
		virtual void load() = 0;
		
		virtual void save() = 0;
		
		virtual void getKey(std::list<TString>&) const = 0;
		
		virtual TUString get(const TString& nodeName) const = 0;
		
		virtual TUString getValue(const TUString& siblingValue, const TString& nodeName) const = 0;
		
		static TSize reference_count;
	private:
		TString m_rootNode;
		std::unordered_map<Int32, std::shared_ptr<TNodeData>>* m_pConfigMap;
		std::unique_ptr<PropertyTree> m_propertyTree;
	};
	
	class TConfigurationFactory : public NonCopyable
	{
	public:
		explicit TConfigurationFactory(const TString& configPath, EConfiguration type);
		
		~TConfigurationFactory() noexcept = default;
		
		void load();
		
		void save();
		
		void keys(std::list<TString>&) const;
		
		const std::shared_ptr<IConfiguration>&
		getConfiguration();
		
		EConfiguration getType() const;
	
	private:
		EConfiguration m_type;
		std::shared_ptr<IConfiguration> m_configuration;
	};
	
	/**
	 * \author Alvin Ahmadov
	 * \class ConfigurationManager
	 * \implements ConfigurationManagerInterface
	 * \namespace Dixter
	 * \brief Singleton class that manages configurations
	 * */
	class TConfigurationManager : public NonCopyable
	{
	public:
		using TSelf 			= TConfigurationManager;
		using TInstancePtr 		= std::shared_ptr<TSelf>;
		using TInstances 		= std::set<TSelf*>;
		using TConstIterator	= ConfigurationProperty::const_iterator;
		
	private:
		/**
		 * \author Alvin Ahmadov
		 * \class ConfigurationManager::Accessor
		 * \brief Class used as inner helper class to read data of ConfigurationManager.
		 *
		 * Provides easy access interfaces to data. Data is immutable.
		 * */
		class TAccessor : public NonCopyable
		{
		public:
			/**
			 * \brief Constructs TAccessor with non-owning TConfigurationManager pointer,
			 * which means it doesn't delete passed pointer.
			 * */
			explicit TAccessor(TConfigurationManager* manager) noexcept;
			
			~TAccessor() noexcept override = default;
			
			/**
		 * \class ConfigurationManager
		 * \brief Get value of node using key.
		 * \param key Node name of value.
		 * \returns Found value.
		 * \throws NotFoundException.
		 * */
			TUString getValue(const TString& key, const TString& root = "") const;
			
			/**
			 * \class XMLConfiguration
			 * \brief Get value of node with key and with sibling value.
			 * \param byValue Sibling value.
			 * \param key Node name of sibling value.
			 * \returns value
			 * \throws NotFoundException
			 * */
			TUString getValue(const TString& key, const TUString& byValue, const TString& root = "") const;
			
			/**
			 * \brief Get all values of node with specified name.
			 * \tparam Container Container to which save data.
			 * */
			const TAccessor* getValues(const TString& key, std::vector<TUString>& values,
									   const TString& root = "") const;
			
		private:
			static IConfiguration*
			get(TConfigurationManager* manager, const TString& key,
				const TString& root = TString());
		
		private:
			TConfigurationManager* m_manager;
		};
		
		/**
		 * \author Alvin Ahmadov
		 * \class ConfigurationManager::Mutator
		 * \brief Class used as inner helper class to write data of ConfigurationManager.
		 *
		 * Provides easy access interfaces to data. Data is mutable.
		 * */
		class TMutator : public NonCopyable
		{
		public:
			/**
			 * \brief Constructs TMutator with non-owning TConfigurationManager pointer,
			 * which means it doesn't delete passed pointer.
			 * */
			explicit TMutator(TConfigurationManager* manager) noexcept;
			
			~TMutator() noexcept override = default;
			
			/**
		 * \class ConfigurationManager
		 * \brief Get value of node using key.
		 * \param key Node name of value.
		 * \returns Found value.
		 * \throws NotFoundException.
		 * */
			const TMutator* setValue(const TString& key, const TUString& value,
									 const TString& root = "");
			
		private:
			IConfiguration* get(const TString& key, const TString& root = "");
		private:
			TConfigurationManager* m_manager;
			mutable std::mutex m_mutex;
		};
	
	private:
		void read(EConfiguration type, const TString& path);
		
		void write(EConfiguration type, const TString& path);
		
		void checkKey(const TConstIterator key, TString errorMsg = "") const;
		
		friend class std::shared_ptr<TSelf>;
		
		/**
		 * \class ConfigurationManager
		 * \brief ctor. Loads configuration for every configuration file.
		 * \param type Type of configuration to initialise.
		 * \param paths List of paths to configuration files.
		 * */
		TConfigurationManager(EConfiguration type, const std::set<TString>& paths);
		TConfigurationManager(const TSelf&) = delete;
		TSelf& operator=(const TSelf&) = delete;
	
	public:
		virtual ~TConfigurationManager() noexcept;
		
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
		static TInstancePtr&
		getManager(EConfiguration type, std::set<TString> paths = std::set<TString>());
		
		const TAccessor* getAccessor() const;
		
		TMutator* getMutator();
		
		/**
		 * \class ConfigurationManager
		 * \brief Gets the type of current instance.
		 * \returns Instance's type.
		 * */
		EConfiguration getType() const;
		
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
		static TInstancePtr m_instance;
		std::unique_ptr<TConfigurationFactory> m_factory;
		ConfigurationProperty m_properties;
		TAccessor* m_accessor;
		TMutator* m_mutator;
	};
} // namespace Dixter
